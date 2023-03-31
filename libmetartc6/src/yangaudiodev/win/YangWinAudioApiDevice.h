﻿//
// Copyright (c) 2019-2022 yanggaofeng
//
#ifndef SRC_YANGCAPTURE_SRC_WIN_YANGWINAUDIOAPI_H_
#define SRC_YANGCAPTURE_SRC_WIN_YANGWINAUDIOAPI_H_

#ifdef _WIN32
#include <yangaudiodev/win/YangWinAudioApiCapture.h>
#include <yangaudiodev/win/YangWinAudioApiRender.h>
#include <yangaudiodev/YangCaptureCallback.h>
#include <yangaudiodev/YangAudioCapture.h>
#include <yangavutil/audio/YangAudioMix.h>
#include <yangavutil/audio/YangPreProcess.h>
#include <yangutil/buffer/YangAudioPlayBuffer.h>

#include <yangutil/sys/YangLoadLib.h>
#include <vector>

#include "YangWinAudioCaptureHandle.h"


class YangWinAudioApiDevice: public YangAudioCapture,public YangCaptureCallback {
public:
    YangWinAudioApiDevice(YangContext *pcontext,bool isRecord,bool usingBuiltinAec=false);
	 ~YangWinAudioApiDevice();
	int init();
	void setInAudioBuffer(std::vector<YangAudioPlayBuffer*> *pal);
	void setPreProcess(YangPreProcess *pp);
	void setCatureStart();
	void setCatureStop();
	void setOutAudioBuffer(YangAudioBuffer *pbuffer);
	void setPlayAudoBuffer(YangAudioBuffer *pbuffer);
	void setAec(YangRtcAec *paec);
	void caputureAudioData(YangFrame* audioFrame);
	YangAudioCaptureHandle *m_ahandle;

protected:
	void startLoop();
	void stopLoop();

private:
    int m_loops;
	int m_isInit;
	int m_outLen;


private:
	bool m_usingBuiltinAec;
	YangAudioApiCapture *m_capture;
    YangWinAudioApiRender* m_player;
};

#endif /* SRC_YANGCAPTURE_SRC_WIN_YANGWINAUDIOAPI_H_ */
#endif
