#include "yangrtc/YangPeerConnection2.h"
#include "yangrtc/YangMetaConnection.h"
#include "yangutil/yangavinfotype.h"
#include "yangutil/yangavctype.h"
#include "yangutil/yangtype.h"
#include "yangavutil/video/YangNalu.h"

#include "yangpush/YangPushPublish.h"
#include "yangpush/YangRtcPublish.h"
#include "yangutil/buffer/YangMediaBuffer.h"
#include "yangp2p/YangP2pRtc.h"
#include "yangstream/YangStreamCapture.h"


#include <string>
#include <iostream>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif
#include <libswscale/swscale.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libavutil/time.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libavutil/hwcontext.h>
#include <libavutil/error.h>

#include "yangavutil/video/YangCMeta.h"
#include "yangutil/sys/YangEndian.h"
#include <yangutil/buffer2/YangVideoEncoderBuffer2.h>
#include "yangencoder/YangVideoEncoderHandle.h"

#ifdef __cplusplus
}
#endif

#include <time.h>
#include <stdint.h>
#include<sys/time.h>

uint64_t getCurrentTimeMicrosecond(){
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (uint64_t)((uint64_t)ts.tv_nsec / 1000 + (((uint64_t)ts.tv_sec) * 1000000));
}

uint32_t convertToRTPTimestamp()
{
	timeval tv;
	gettimeofday(&tv, NULL);
	uint32_t timestampIncrement = (90000 * tv.tv_sec);
	timestampIncrement += (uint32_t)((2.0 * 90000 * tv.tv_usec + 1000000.0) / 2000000);
	//UINT32 const rtpTimestamp =  timestampIncrement;  
	return timestampIncrement;
}


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

int getPpsSpsData(uint8_t *srcData, uint32_t srcLen, uint8_t* ppsSpsData, uint32_t* ppsSpsLen){
	uint32_t count = 0;
	for(int i=0; i<srcLen; i++){
        if(srcData[i] == 0 && srcData[i+1] == 0 && srcData[i+2] == 0 && srcData[i+3] == 1 && srcData[i+4] == 0x65){
        return 1;
    }
		if(srcData[i] == 0 && srcData[i+1] == 0 && srcData[i+2] == 0 && srcData[i+3] == 1){
        i+=3;
		continue;
    }
		ppsSpsData[count] = srcData[i];
		count++;
		*(ppsSpsLen) = count;
    }
	return 0;
}


unsigned char getNaluType(uint8_t *data, uint32_t len)
{
    unsigned char naluType;
	printf("H264 Framedata:");
	for(int i=0; i<len; i++){
		printf(" %x",data[i]);
	}
    printf("\n");

    for(int i=0; i<len; i++){
        if(data[i] == 0 && data[i+1] == 0 && data[i+2] == 0 && data[i+3] == 1){
        naluType = data[i+4];
        printf("getNaluType:[%x]\n",naluType);
        }
    }
    return naluType;
}

bool isKeyFrame(uint8_t* data, uint32_t size)
{
	if (size > 4) {
		//0x67:sps ,0x65:IDR, 0x68: pps
		if (data[4] == 0x67 || data[4] == 0x65 ||
			data[4] == 0x68) {
			return true;
		}
	}
	return false;
}


int main(int argc, char *argv[]){
    YangContext configContext;
    configContext.init((char*)"./yang_config.ini");

    configContext.avinfo.video.videoEncoderFormat=YangI420;
    configContext.avinfo.enc.createMeta=0;
    configContext.avinfo.sys.mediaServer=Yang_Server_Srs;//Yang_Server_Srs/Yang_Server_Zlm
    configContext.avinfo.video.videoEncoderType=Yang_VED_264;//Yang_VED_265;
  
	
	configContext.avinfo.video.width = 1920;
	configContext.avinfo.video.height = 1080;
	configContext.avinfo.video.outWidth = 1920;
	configContext.avinfo.video.outHeight = 1080;
	configContext.avinfo.video.rate = 1200;
    configContext.avinfo.video.frame = 25;
    configContext.avinfo.video.videoCacheNum = 50;
    configContext.avinfo.video.evideoCacheNum = 50;
    configContext.avinfo.video.vIndex = 0;

	//srs do not use audio fec
    if(configContext.avinfo.sys.mediaServer==Yang_Server_Srs){
        configContext.avinfo.audio.enableAudioFec=yangfalse;
        configContext.avinfo.rtc.enableDatachannel=yangfalse;
    }

    // connect to SRS 
    int netType = 2; //#define Yang_Websocket_Http 2
    int port = 1985;
    std::string server = "192.168.2.133";
    std::string app = "live";
    std::string stream = "livestream";

	//connect to ZLM
	// int netType = 2; //#define Yang_Websocket_Http 2
	// int port = 80;
	// std::string server = "192.168.2.133";
	// std::string app = "live";
	// std::string stream = "test";

    YangStreamConfig streamconfig;
	memset(&streamconfig,0,sizeof(YangStreamConfig));
	strcpy(streamconfig.app,app.c_str());
	streamconfig.streamOptType=Yang_Stream_Publish;

	strcpy(streamconfig.remoteIp,server.c_str());
	streamconfig.remotePort=port;

	strcpy(streamconfig.stream,stream.c_str());
	streamconfig.uid=0;
	streamconfig.isServer=0;
	streamconfig.localPort=configContext.avinfo.sys.rtcLocalPort;

	streamconfig.recvCallback.receiveMsg=g_rtc_receiveMsg;
	memcpy(&streamconfig.rtcCallback,&configContext.rtcCallback,sizeof(YangRtcCallback));

    YangPeerConnection2* sh=new YangPeerConnection2(&configContext.avinfo,&streamconfig);
    sh->init();
    sh->connectSfuServer();
    sleep(1);
	configContext.streams.sendRequest(0,0,Yang_Req_Connected);
	
	YangFrame sendFrame;
	memset(&sendFrame,0,sizeof(YangFrame));
	
	int sendRet = Yang_Ok;

	uint8_t spsPpsBuf[1024] = {0};
	uint32_t spsPpsLen = 0;
	uint8_t spsPpsBuf_2[27] = {0x17, 0x00, 0x00, 0x00, 0x00, 0x01, 0x42, 0xc0, 0x1f, 0xff, 0xe1, 0x00, 0x0e, 0x67, 0x42, 0x40, 0x32, 0x96, 0x54, 0x03, 0xc0, 0x16, 0xb2, 0x68, 0xce, 0x3c, 0x80};
	
	
	AVFormatContext *input_ctx = NULL;
	AVCodecContext *decoder_ctx = NULL;
	AVCodec *decoder = NULL;
	
	const char *in_file = "./gstreamerRecord.h264";
	int video_stream = 0, ret;
	AVPacket packet;

	unsigned int frameCount = 0;
	/* open the input file */
	if(avformat_open_input(&input_ctx, in_file, NULL, NULL) != 0) {
		printf("Cannot open input file '%s'\n",in_file);
		return -1;
	}

	/* find the video stream information */
	ret = av_find_best_stream(input_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
	if(ret < 0) {
		printf("Cannot find a video stream in the input file\n");
		return -1;
	}

	uint32_t timestampBase = convertToRTPTimestamp();
	while (ret >= 0)
	{
		if ( ( ret = av_read_frame(input_ctx,&packet) ) < 0 )//逐个包从文件读取数据
		{
			break;
		}

		frameCount++;
		
		printf( "Read frame count:%d \n", frameCount);
		if (video_stream == packet.stream_index)//打印帧大小
		{
			printf( " frame size:%d \n" , packet.size );
		}
		
		if(isKeyFrame(packet.data,packet.size)){
			// getNaluType(packet.data,packet.size);
			getPpsSpsData(packet.data,packet.size,spsPpsBuf,&spsPpsLen);
			std::cout << "spsPpsLen= " << spsPpsLen << std::endl;
			sendFrame.nb = spsPpsLen;
			sendFrame.payload = spsPpsBuf;
			// sendFrame.nb = 27;
			// sendFrame.payload = spsPpsBuf_2;
			sendFrame.frametype=YANG_Frametype_Spspps;
            sendFrame.pts=sendFrame.dts= convertToRTPTimestamp();
			// sendFrame.pts= convertToRTPTimestamp() - timestampBase;
			// sendFrame.pts= 0;
			// sendFrame.dts=0;
			sendFrame.uid = streamconfig.uid;
			sendFrame.mediaType = 0;
			std::cout << "spsPpsLen==== " << std::endl;

			std::cout << "videoFrame mediaType: " << sendFrame.mediaType << std::endl;
			std::cout << "videoFrame uid: " << sendFrame.uid << std::endl;
			std::cout << "videoFrame frametype: " << sendFrame.frametype << std::endl;
			std::cout << "videoFrame nb: " << sendFrame.nb << std::endl;
			std::cout << "videoFrame pts: " << sendFrame.pts << std::endl;
			std::cout << "videoFrame dts: " << sendFrame.dts << std::endl;
			// printf("H264 Framedata: ");
			// for(int i=0; i<sendFrame.nb; i++){
			// 	printf(" %x",sendFrame.payload[i]);
			// }
			// printf("\n");
			if(sh->isConnected()){
				sendRet = sh->on_video(&sendFrame);
				std::cout << "send video ret: " << sendRet  << " --isConnected(): " << sh->isConnected() << std::endl;
				if(sendRet != Yang_Ok){
					std::cout << "sh->on_video() failed" << std::endl;
					break;
				}
			}else{
				std::cout << "sh->isConnected() failed" << std::endl;
				break;
			}

			usleep(40*1000);
			sendFrame.nb = (packet.size-spsPpsLen-4-8);
			sendFrame.payload = packet.data+4+spsPpsLen+8;
			sendFrame.frametype=YANG_Frametype_I;
			sendFrame.pts=sendFrame.dts= convertToRTPTimestamp();
			// sendFrame.pts= convertToRTPTimestamp() - timestampBase;
			// sendFrame.pts= 0;
			// sendFrame.dts=0;
			sendFrame.uid = streamconfig.uid;
			sendFrame.mediaType = 0;

			std::cout << "spsPpsLen====1 " << std::endl;

			std::cout << "videoFrame mediaType: " << sendFrame.mediaType << std::endl;
			std::cout << "videoFrame uid: " << sendFrame.uid << std::endl;
			std::cout << "videoFrame frametype: " << sendFrame.frametype << std::endl;
			std::cout << "videoFrame nb: " << sendFrame.nb << std::endl;
			std::cout << "videoFrame pts: " << sendFrame.pts << std::endl;
			std::cout << "videoFrame dts: " << sendFrame.dts << std::endl;
			// printf("H264 Framedata: ");
			// for(int i=0; i<sendFrame.nb; i++){
			// 	printf(" %x",sendFrame.payload[i]);
			// }
			// printf("\n");
			if(sh->isConnected()){
				sendRet = sh->on_video(&sendFrame);
				std::cout << "send video ret: " << sendRet  << " --isConnected(): " << sh->isConnected() << std::endl;
				if(sendRet != Yang_Ok){
					std::cout << "sh->on_video() failed" << std::endl;
					break;
				}
			}else{
				std::cout << "sh->isConnected() failed" << std::endl;
				break;
			}

			continue;
			// sendFrame.nb = (packet.size-4);
			// sendFrame.payload = packet.data+4;
			// sendFrame.frametype=YANG_Frametype_I;
			// // sendFrame.pts=sendFrame.dts= getCurrentTimeMicrosecond();
			// sendFrame.pts=sendFrame.dts= convertToRTPTimestamp();
			// sendFrame.uid = streamconfig.uid;
		}else{
			sendFrame.nb=(packet.size-4);
			sendFrame.payload=packet.data+4;
			sendFrame.frametype=YANG_Frametype_P;
			// sendFrame.pts=sendFrame.dts= getCurrentTimeMicrosecond();
			sendFrame.pts=sendFrame.dts= convertToRTPTimestamp();
			// sendFrame.pts= convertToRTPTimestamp() - timestampBase;
			// sendFrame.pts= 0;
			// sendFrame.dts=0;
			sendFrame.uid = streamconfig.uid;
			sendFrame.mediaType = 0;
		}

		std::cout << "videoFrame mediaType: " << sendFrame.mediaType << std::endl;
		std::cout << "videoFrame uid: " << sendFrame.uid << std::endl;
		std::cout << "videoFrame frametype: " << sendFrame.frametype << std::endl;
		std::cout << "videoFrame nb: " << sendFrame.nb << std::endl;
		std::cout << "videoFrame pts: " << sendFrame.pts << std::endl;
		std::cout << "videoFrame dts: " << sendFrame.dts << std::endl;
		// printf("H264 Framedata: ");
		// for(int i=0; i<sendFrame.nb; i++){
		// 	printf(" %x",sendFrame.payload[i]);
		// }
		// printf("\n");
		if(sh->isConnected()){
			sendRet = sh->on_video(&sendFrame);
			std::cout << "send video ret: " << sendRet  << " --isConnected(): " << sh->isConnected() << std::endl;
			if(sendRet != Yang_Ok){
				std::cout << "sh->on_video() failed" << std::endl;
				break;
			}
		}else{
			std::cout << "sh->isConnected() failed" << std::endl;
			break;
		}

		av_packet_unref(&packet);
		usleep(40*1000);
	}
	avformat_close_input(&input_ctx);
  
	std::cout << "close peerconnection......" << std::endl;
	sh->close();
    yang_delete(sh);

}
