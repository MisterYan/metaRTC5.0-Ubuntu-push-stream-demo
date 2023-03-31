#include "yangrtc/YangPeerConnection2.h"
#include "yangrtc/YangMetaConnection.h"
#include "yangutil/yangavinfotype.h"
#include "yangutil/yangavctype.h"
#include "yangutil/yangtype.h"
#include "yangpush/YangPushPublish.h"
#include "yangpush/YangRtcPublish.h"
#include "yangpush/YangPushCommon.h"
#include "yangutil/buffer/YangMediaBuffer.h"
#include "yangp2p/YangP2pRtc.h"

#include <string.h>
#include <iostream>
#include <stdlib.h>

void g_rtc_receiveMsg(void* user,YangFrame *msgFrame){
	if(msgFrame==NULL) {
		std::cout << "Receive datachannel message failed" << std::endl;
		return;
	}
	
	std::cout << "Receive datachannel: " << msgFrame->payload << std::endl;
	std::cout << "Receive-datachannel-mediaType: " << msgFrame->mediaType << std::endl;
	std::cout << "Receive-datachannel-uid: " << msgFrame->uid << std::endl;
	std::cout << "Receive-datachannel-frametype: " << msgFrame->frametype << std::endl;
	std::cout << "Receive-datachannel-nb: " << msgFrame->nb << std::endl;
	std::cout << "Receive-datachannel-pts: " << msgFrame->pts << std::endl;
	std::cout << "Receive-datachannel-dts: " << msgFrame->dts << std::endl;
	std::cout << "Receive-datachannel-payload: " << msgFrame->payload << std::endl;
}

int main(int argc, char *argv[]){

YangContext configContext;
configContext.init((char*)"yang_config.ini");

configContext.avinfo.video.videoEncoderFormat=YangI420;
configContext.avinfo.enc.createMeta=0;
configContext.avinfo.sys.mediaServer=Yang_Server_Zlm;//Yang_Server_Srs/Yang_Server_Zlm
configContext.avinfo.video.videoEncoderType=Yang_VED_264;//Yang_VED_265;
//srs do not use audio fec
if(configContext.avinfo.sys.mediaServer==Yang_Server_Srs){
	configContext.avinfo.audio.enableAudioFec=yangfalse;
	configContext.avinfo.rtc.enableDatachannel=yangfalse;
}

//zlm enable datachannel
if(configContext.avinfo.sys.mediaServer==Yang_Server_Zlm){
	configContext.avinfo.rtc.enableDatachannel=yangtrue;
	configContext.channeldataRecv.receiveData=g_rtc_receiveMsg;
}

YangPushPublish pushSRS(&configContext);
pushSRS.setCaptureType(Yang_VideoSrc_Camera);
pushSRS.setVideoInfo(&configContext.avinfo.video);
std::cout << "pushSRS.startCamera()>>>>>" << std::endl;
pushSRS.startCamera();

std::cout << "pushSRS.initVideoEncoding()>>>>>" << std::endl;
pushSRS.initVideoEncoding();

//connect to SRS 
// int netType = 2; //#define Yang_Websocket_Http 2
// int port = 1985;
// std::string server = "192.168.2.133";
// std::string app = "live";
// std::string stream = "livestream";

//connect to ZLM
int netType = 2; //#define Yang_Websocket_Http 2
int port = 80;
std::string server = "192.168.2.133";
std::string app = "live";
std::string stream = "test";

YangRtcPublish rtcPub(&configContext);
pushSRS.setRtcNetBuffer(&rtcPub);
pushSRS.startVideoEncoding();
std::cout << "rtcPub.init()>>>>>" << std::endl;
rtcPub.init(netType,(char*)server.c_str(),port,(char*)app.c_str(),(char*)stream.c_str());
std::cout << "rtcPub.start()>>>>>" << std::endl;
rtcPub.start();
pushSRS.startVideoCaptureState();


char const *sendData = "mySendMessage";

char charInput=0;
while (true)
{
	

	std::cin >> charInput;
	if(charInput == 'a'){
		YangFrame sendDataFrame;
    memset(&sendDataFrame,0,sizeof(YangFrame));
    sendDataFrame.mediaType=YANG_DATA_CHANNEL_STRING;
    sendDataFrame.payload=(uint8_t*)sendData;
    sendDataFrame.nb=strlen(sendData);
		std::cout << "Send datachannel: " << sendData << std::endl;
    rtcPub.publishMsg(&sendDataFrame);
	}
	
	sleep(1);
}


rtcPub.disConnectMediaServer();
pushSRS.stopAll();


}