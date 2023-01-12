#include "yangrtc/YangPeerConnection2.h"
#include "yangrtc/YangMetaConnection.h"
#include "yangutil/yangavinfotype.h"
#include "yangutil/yangavctype.h"
#include "yangutil/yangtype.h"
#include "yangpush/YangPushPublish.h"
#include "yangpush/YangRtcPublish.h"
#include "yangutil/buffer/YangMediaBuffer.h"
#include "yangp2p/YangP2pRtc.h"

#include <string.h>
#include <iostream>
#include <stdlib.h>


int main(int argc, char *argv[]){

YangContext configContext;
configContext.init((char*)"yang_config.ini");

configContext.avinfo.video.videoEncoderFormat=YangI420;
configContext.avinfo.enc.createMeta=0;
configContext.avinfo.sys.mediaServer=Yang_Server_Srs;//Yang_Server_Srs/Yang_Server_Zlm
configContext.avinfo.video.videoEncoderType=Yang_VED_264;//Yang_VED_265;
//srs do not use audio fec
if(configContext.avinfo.sys.mediaServer==Yang_Server_Srs){
	configContext.avinfo.audio.enableAudioFec=yangfalse;
	configContext.avinfo.rtc.enableDatachannel=yangfalse;
}

YangPushPublish pushSRS(&configContext);
std::cout << "pushSRS.startCamera()>>>>>" << std::endl;
pushSRS.startCamera();

std::cout << "pushSRS.initVideoEncoding()>>>>>" << std::endl;
pushSRS.initVideoEncoding();

// connect to SRS 
int netType = 2; //#define Yang_Websocket_Http 2
int port = 1985;
std::string server = "192.168.2.133";
std::string app = "live";
std::string stream = "livestream";

// connect to ZLM
// int netType = 2; //#define Yang_Websocket_Http 2
// int port = 8000;
// std::string server = "192.168.2.133";
// std::string app = "live";
// std::string stream = "test";

YangRtcPublish rtcPub(&configContext);
pushSRS.setRtcNetBuffer(&rtcPub);
pushSRS.startVideoEncoding();
std::cout << "rtcPub.init()>>>>>" << std::endl;
rtcPub.init(netType,(char*)server.c_str(),port,(char*)app.c_str(),(char*)stream.c_str());
std::cout << "rtcPub.start()>>>>>" << std::endl;
rtcPub.start();
pushSRS.startVideoCaptureState();


char input=0;
while (true)
{
	std::cin >> input;
	if(input == 'q'){
		rtcPub.disConnectMediaServer();
		pushSRS.stopAll();
		std::cout << " Exit program " << std::endl;
    break;
	}
	sleep(1);
}

}