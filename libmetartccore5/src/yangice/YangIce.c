﻿//
// Copyright (c) 2019-2022 yanggaofeng
//

#include <yangice/YangIce.h>
#include <yangutil/yangavtype.h>
#include <yangutil/sys/YangEndian.h>
#include <yangutil/sys/YangLog.h>
#include <yangutil/sys/YangSocket.h>
#include <yangutil/sys/YangMath.h>
#include <yangutil/sys/YangCString.h>
#include <yangrtc/YangRtcConnection.h>

typedef struct{
	uint32_t ip;
	int32_t port;
	yangbool response;
	YangRtcStun* stun;
	YangIceServer* server;
}YangIceStunRequest;


YangTurnConnection* g_yang_turnConnection=NULL;
uint32_t g_yang_turnCount=0;

YangTurnConnection* yang_ice_getTurn(YangIceServer* server){

	if(g_yang_turnConnection==NULL){
		g_yang_turnConnection=(YangTurnConnection*)calloc(sizeof(YangTurnConnection),1);
		YangRtcUdp* udp=(YangRtcUdp*)calloc(sizeof(YangRtcUdp),1);
		int32_t port=10000+yang_random()%15000;
		yang_create_rtcudp(udp,port);
		yang_create_turnConnection(g_yang_turnConnection,server,udp,port);
	}
	g_yang_turnCount++;
	return g_yang_turnConnection;
}

void g_yang_ice_stun_receive(char *data, int32_t size, void *user) {
	if (user == NULL)		return;
	YangIceStunRequest* request=(YangIceStunRequest*)user;

	uint8_t bt=(uint8_t)data[0];
	if(size>0&&(bt==0x00||bt==0x01)){
		int32_t err = 0;

		YangStunPacket response;
		memset(&response,0,sizeof(YangStunPacket));
		if ((err = request->stun->decodeStunServer(&response,data, size)) != 0) {
            yang_error("decode stun packet failed");
			return;
		}

		request->ip= yang_get_be32((uint8_t*)(&response.mapped_address));
		request->port=response.mapped_port;

		request->response=yangtrue;
		uint8_t* ip=(uint8_t*)&request->ip;
		yang_trace("\nstun ip=%u.%u.%u.%u,stun port=%d",ip[0],ip[1],ip[2],ip[3],request->port);
	}

}

int32_t yang_ice_stun_request(YangIceServer* server,int32_t localport){

	char tmp[1024]={0};
	int32_t err=Yang_Ok;
	int32_t waitTime=0;
	YangRtcUdp* udp=(YangRtcUdp*)calloc(sizeof(YangRtcUdp),1);
	YangRtcStun* stun=(YangRtcStun*)calloc(sizeof(YangRtcStun),1);
	YangIceStunRequest request;
	request.server=server;
	request.stun=stun;
	request.response=yangfalse;

	yang_create_stun(stun);
	yang_create_rtcudp(udp,localport);
	char serverIp[64]={0};
	yang_getIp(server->serverIp,serverIp);

	udp->updateRemoteAddress(&udp->session,serverIp,server->serverPort);

	udp->session.receive=g_yang_ice_stun_receive;
	udp->session.user=&request;
	udp->start(&udp->session);


	YangBuffer buf;
	yang_init_buffer(&buf,tmp,1024);

	char* pwd = (char*) calloc(33, 1);
	yang_cstr_random(32, pwd);
	stun->encodeStunServer(&buf,udp,server->username,pwd);


	if((err=udp->sendData(&udp->session,buf.data, yang_buffer_pos(&buf)))!=Yang_Ok){
		yang_error("stun server request fail");
		goto ret;
	}

	while(!request.response&&waitTime<500){
		yang_usleep(YANG_UTIME_MILLISECONDS*2);
		waitTime++;
		if(waitTime%25==0){
			memset(tmp,0,sizeof(tmp));
			yang_init_buffer(&buf,tmp,sizeof(tmp));
			stun->encodeStunServer(&buf,udp,server->username,pwd);
			if((err=udp->sendData(&udp->session,buf.data, yang_buffer_pos(&buf)))!=Yang_Ok){
				yang_error("stun server request fail");
				goto ret;
			}
		}
	}

	if(request.response){
		server->stunIp=request.ip;
		server->stunPort=request.port;
	}else{
		err=ERROR_RTC_STUN;
	}

	ret:
		yang_free(pwd);
		udp->stop(&udp->session);
		yang_destroy_rtcudp(udp);
		yang_free(udp);
		yang_destroy_stun(stun);
		yang_free(stun);
		return err;
}

int32_t yang_ice_initTurn(YangIceSession *session){
	if(session->isTurnAllocated) return Yang_Ok;
	int32_t count=0;
	int32_t maxWaitCount=100;
	if(session->turnconn==NULL){
		session->turnconn=yang_ice_getTurn(&session->server);
	}

	session->turnconn->start(&session->turnconn->session);
	session->isTurnAllocated=yangtrue;
	while(count<maxWaitCount&&!session->turnconn->isAllocated(&session->turnconn->session)){
		yang_usleep(20000);
		count++;
	}

    session->server.relayIp = session->turnconn->session.relayIp;
    session->server.relayPort = session->turnconn->session.relayPort;

	if(count>=maxWaitCount) {
		session->isTurnAllocated=yangfalse;
		session->turnconn->stop(&session->turnconn->session);
		return ERROR_RTC_TURN;
	}
	return Yang_Ok;
}

int32_t yang_ice_requestTurnServer(YangIceSession *session,void* rtcSession,yang_turn_receive receive,char* remoteIp,int32_t remotePort){
	if(session->turnconn==NULL){
		yang_ice_initTurn(session);
	}else if(!session->isTurnAllocated){
		return ERROR_RTC_TURN;
	}
	int32_t count=0;
	int32_t maxWaitCount=100;
	session->turnconn->addPeer(&session->turnconn->session,session->uid,rtcSession,receive,remoteIp,remotePort);

	session->isTurnReady=yangfalse;
	while(count<maxWaitCount&&!session->turnconn->isReady(&session->turnconn->session)){
		yang_usleep(20000);
		count++;
	}

	if(count>=maxWaitCount) {
		session->isTurnReady=yangfalse;
		return ERROR_RTC_TURN;
	}

	session->isTurnReady=yangtrue;
	return Yang_Ok;
}


int32_t yang_ice_requestStunServer(YangIceSession *session){

	int32_t err=Yang_Ok;
	if(session->candidateType!=YangIceStun) return 1;
	if((err=yang_ice_stun_request(&session->server,session->localPort))!=Yang_Ok){
		return yang_error_wrap(err,"stun request fail!");
	}

	return err;
}


void yang_ice_closeTurn(YangIceSession *session){

   yang_destroy_turnConnection(g_yang_turnConnection);
   yang_free(g_yang_turnConnection);

    session->turnconn=NULL;
    g_yang_turnCount=0;
}

void yang_ice_initIce(YangIceSession *session){
	switch(session->candidateType){
		case YangIceHost:
			break;
		case YangIceStun:
			if(yang_ice_requestStunServer(session)!=Yang_Ok){
				yang_error("request stun server fail,change to host");
				session->candidateType=YangIceHost;
			}
			break;
		case YangIceTurn:
			if(yang_ice_initTurn(session)!=Yang_Ok) {
				yang_error("request turn server fail,change to stun");
				session->candidateType=YangIceStun;
				if(yang_ice_requestStunServer(session)!=Yang_Ok){
                    yang_ice_closeTurn(session);
					yang_error("request stun server fail,change to host");
					session->candidateType=YangIceHost;
				}

			}
			break;

	}
}

YangIceCandidateType yang_ice_handle(YangIceSession* session,void* prtcSession,yang_turn_receive receive,char* remoteIp,int32_t remotePort){
	YangRtcSession* rtcSession=(YangRtcSession*)prtcSession;
	switch(session->candidateType){
			case YangIceHost:
				break;
			case YangIceStun:
                if(session->server.stunPort==0&&rtcSession->isServer&&yang_ice_requestStunServer(session)!=Yang_Ok) {//
					yang_error("request stun server fail,change to host");
					session->candidateType=YangIceHost;
				}
				break;
			case YangIceTurn:
				if(yang_ice_requestTurnServer(session,prtcSession,receive,remoteIp,remotePort)!=Yang_Ok) {
                    yang_ice_closeTurn(session);
					yang_error("request turn server fail, change to stun");
					session->candidateType=YangIceStun;
					if(yang_ice_requestStunServer(session)!=Yang_Ok){
						yang_error("request stun server fail,change to host");
						session->candidateType=YangIceHost;
					}

				}
				break;
		}
	return session->candidateType;
}


void yang_create_ice(YangIce* ice,YangStreamConfig* config,YangAVInfo* avinfo){
	if(ice==NULL) return;
	YangIceSession* session=&ice->session;
	session->uid=config->uid;
	yang_create_stun(&session->stun);
	strcpy(session->server.serverIp,avinfo->rtc.iceServerIP);
	session->server.serverPort=avinfo->rtc.iceServerPort;
	session->candidateType=(YangIceCandidateType)avinfo->rtc.iceCandidateType;
	session->isTurnAllocated=yangfalse;
	session->isTurnReady=yangfalse;
	session->localPort=config->localPort;
	strcpy(session->server.username,avinfo->rtc.iceUserName);
	strcpy(session->server.password,avinfo->rtc.icePassword);


	ice->initIce=yang_ice_initIce;
	ice->iceHandle=yang_ice_handle;
}

void yang_destroy_ice(YangIce* ice){
	if(ice==NULL) return;
	YangIceSession* session=&ice->session;
	yang_destroy_stun(&session->stun);

	if(session->turnconn){
		session->turnconn->removePeer(&session->turnconn->session,session->uid);

		g_yang_turnCount--;

		if(g_yang_turnCount==0){
			yang_destroy_turnConnection(g_yang_turnConnection);
			yang_free(g_yang_turnConnection);
		}
		session->turnconn=NULL;
	}
}

