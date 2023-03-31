﻿//
// Copyright (c) 2019-2022 yanggaofeng
//
#ifndef SRC_YANGMEETING_INCLUDE_YangP2pRtc_H_
#define SRC_YANGMEETING_INCLUDE_YangP2pRtc_H_

#include <yangutil/buffer/YangAudioEncoderBuffer.h>
#include <yangutil/buffer/YangVideoEncoderBuffer.h>
#include <yangutil/buffer/YangVideoDecoderBuffer.h>

#include <yangutil/sys/YangSysMessageI.h>
#include <yangutil/sys/YangThread2.h>
#include <yangutil/sys/YangCTimer.h>
#include <yangutil/sys/YangVector.h>

#include <yangstream/YangStreamCapture.h>
#include <yangavutil/video/YangNalu.h>

#include <yangrtc/YangPeerConnection.h>
#include <yangp2p/YangP2pCommon.h>

class YangP2pRtcI{
public:
	YangP2pRtcI(){};
	virtual ~YangP2pRtcI(){};
	virtual void removePlayBuffer(int32_t puid,int32_t playcount)=0;
	virtual void sendKeyframe()=0;
};

yang_vector_declare2(YangPeerConnection)
yang_vector_declare(yangint32)

class YangP2pRtc: public YangThread ,public YangP2pRtcBuffer,public YangMediaConfigCallback{
public:
	YangP2pRtc(YangContext *pcontext);
	virtual ~YangP2pRtc();

	int32_t connectPeer(int32_t nettype, char* server, int32_t localPort,int32_t pport,
			char* app,char* stream);
	int32_t addPeer(char* sdp,char* answer,char* remoteIp,int32_t localPort,int* phasplay);
	int32_t removePeer(int32_t uid);
	int32_t erasePeer(int32_t uid);

	int32_t connectMediaServer();
	int32_t disConnectPeer();

	void checkAlive();

	void setInVideoMetaData(YangVideoMeta *pvmd);
	void setInAudioList(YangAudioEncoderBuffer *pbuf);
	void setInVideoList(YangVideoEncoderBuffer *pbuf);
	void setOutAudioList(YangAudioEncoderBuffer *pbuf);
	void setOutVideoList(YangVideoDecoderBuffer *pbuf);



	YangVideoDecoderBuffer* getOutVideoList();
	void receiveAudio(YangFrame* audioFrame);
	void receiveVideo(YangFrame* videoFrame);
	void receiveMsg(YangFrame* videoFrame);
    int32_t publishMsg(YangFrame* msgFrame);
    void sendRtcMessage(int32_t puid,YangRtcMessageType mess);
	void setMediaConfig(int32_t puid, YangAudioParam *audio,YangVideoParam *video);
	int32_t stopPublishAudioData();
	int32_t stopPublishVideoData();

	int32_t m_netState;
	int32_t isPublished;
	int32_t m_isStart;

	YangP2pRtcI* m_p2pRtcI;
	void stop();
protected:
	void run();
	void handleError(int32_t perrCode);
	void startLoop();
	void startLoop_h265();
	int32_t publishVideoData(YangStreamCapture* data);
	int32_t publishAudioData(YangStreamCapture* data);
	void removeStream();
	YangContext *m_context;
	YangVideoMeta *m_vmd;
	YangVideoEncoderBuffer *m_in_videoBuffer;
	YangAudioEncoderBuffer *m_in_audioBuffer;

	int32_t m_isConvert;
	int32_t m_isInit;
	int32_t m_audioEncoderType;
	int32_t m_playCount;
	YangPeerConnectionVector2 m_pushs;
private:
	int32_t m_transType;
	int32_t notifyState;
	uint32_t m_uidSeq;
	int32_t m_clientUid;
	YangCTimer* m_5stimer;
	yang_thread_mutex_t m_mutex;
	yangint32Vector m_removeList;

	YangH264NaluData m_nalu;

	YangAudioEncoderBuffer *m_out_audioBuffer;
	YangVideoDecoderBuffer *m_out_videoBuffer;
};

#endif /* SRC_YANGMEETING_INCLUDE_YangP2pRtc_H_ */
