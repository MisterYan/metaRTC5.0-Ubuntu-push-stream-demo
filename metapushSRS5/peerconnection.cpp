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
#ifdef __cplusplus
}
#endif


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

/* 用removeSEINal 函数处理掉SEI NAL数据再解码 */
int removeSEINal(unsigned char *pBuf,const uint8_t *data, uint32_t len, bool *IskeyFrame, char* NaluType)
{
  unsigned char naluType;
  uint32_t dataLen = 0;
  bool drop = false;
  int NalNum = 0;

  for(int i=0; i<len; i++){
    if(data[i] == 0 && data[i+1] == 0 && data[i+2] == 1){
      naluType = data[i+3] & 0x1F;
      *NaluType = naluType;
      printf("Remove 00 00 01 format SEI Nal: naluType[%x]\n",naluType);
      switch (naluType)
      {
        case 0x06:
          drop = true;
          break;
        case 0x07:
        case 0x08:  
          NalNum++;
          drop = false;
          break;
        default:
          drop = false;
          break;
      }
    }

    if(data[i] == 0 && data[i+1] == 0 && data[i+2] == 0 && data[i+3] == 1){
      naluType = data[i+4] & 0x1F;
      *NaluType = naluType;
      printf("Remove 00 00 00 01 format SEI Nal: naluType[%x]\n",naluType);
      switch (naluType)
      {
        case 0x06:
          drop = true;
          break;
        case 0x07:
        case 0x08:  
          NalNum++;
          drop = false;
          break;
        default:
          drop = false;
          break;
      }
    }

    if(drop){
      continue;
    }else{
      if(NalNum > 0){
        *IskeyFrame = true;
      }else{
        *IskeyFrame = false;
      }
      *pBuf++ = data[i];
      dataLen++;
    }
  }
  return dataLen;
}


int main(int argc, char *argv[]){
  YangContext configContext;
  configContext.init((char*)"./yang_config.ini");

  configContext.avinfo.video.videoEncoderFormat=YangI420;
  configContext.avinfo.enc.createMeta=0;
  configContext.avinfo.sys.mediaServer=Yang_Server_Zlm;//Yang_Server_Srs/Yang_Server_Zlm
  configContext.avinfo.video.videoEncoderType=Yang_VED_264;//Yang_VED_265;
  //srs do not use audio fec
  if(configContext.avinfo.sys.mediaServer==Yang_Server_Srs){
    configContext.avinfo.audio.enableAudioFec=yangfalse;
    configContext.avinfo.rtc.enableDatachannel=yangfalse;
  }

  // connect to SRS 
  // int netType = 2; //#define Yang_Websocket_Http 2
  // int port = 1985;
  //std::string server = "192.168.2.133";
  // std::string app = "live";
  // std::string stream = "livestream";

	//connect to ZLM
	int netType = 2; //#define Yang_Websocket_Http 2
	int port = 8000;
	std::string server = "192.168.2.133";
	std::string app = "live";
	std::string stream = "test";

  YangStreamConfig streamconfig;
	memset(&streamconfig,0,sizeof(YangStreamConfig));
	strcpy(streamconfig.app,app.c_str());
	streamconfig.streamOptType=Yang_Stream_Publish;

	strcpy(streamconfig.remoteIp,server.c_str());
	streamconfig.remotePort=port;

	strcpy(streamconfig.stream,stream.c_str());
	streamconfig.uid=0;

	streamconfig.localPort=configContext.avinfo.sys.rtcLocalPort;

	streamconfig.recvCallback.receiveMsg=g_rtc_receiveMsg;
	memcpy(&streamconfig.rtcCallback,&configContext.rtcCallback,sizeof(YangRtcCallback));

  YangPeerConnection2* sh=new YangPeerConnection2(&configContext.avinfo,&streamconfig);
  sh->init();
  sh->connectSfuServer();
  
	configContext.streams.sendRequest(0,0,Yang_Req_Connected);

	YangFrame videoFrame;

	AVFormatContext *input_ctx = NULL;
	AVCodecContext *decoder_ctx = NULL;
	AVCodec *decoder = NULL;
	
	const char *in_file = "./FPV.h264";
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

		// unsigned char *decodeBuf = new unsigned char[packet.size];
		// int len;
		// bool keyFrame;
		// char NaluType = 0;
		// len = removeSEINal(decodeBuf,packet.data,packet.size,&keyFrame,&NaluType);

		// /* send h264 frame to srs */
		// memset(&videoFrame,0,sizeof(YangFrame));
		// videoFrame.payload = decodeBuf;
		// videoFrame.nb = len;

		/* send h264 frame to zlm */
		memset(&videoFrame,0,sizeof(YangFrame));
		videoFrame.payload = packet.data;
		videoFrame.nb = packet.size;

		int sendRet = sh->on_video(&videoFrame);
		std::cout << "send video ret: " << sendRet << std::endl;
		// delete[] decodeBuf;

		av_packet_unref(&packet);
	}
	avformat_close_input(&input_ctx);
  
	sh->close();
  yang_delete(sh);

}
