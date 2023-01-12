﻿//
// Copyright (c) 2019-2022 yanggaofeng
//

#ifndef YANGCAPTURE_SRC_YANGALSAHANDLE_H_
#define YANGCAPTURE_SRC_YANGALSAHANDLE_H_


#ifndef _WIN32
#include <alsa/asoundlib.h>
#include "yangavutil/audio/YangAudioMix.h"
#include "yangavutil/audio/YangPreProcess.h"
#include "yangutil/sys/YangThread.h"
#include <yangutil/yangavinfotype.h>
#include <yangaudiodev/YangAudioCapture.h>
#include <yangaudiodev/YangAudioCaptureHandle.h>


class YangAlsaHandle: public YangAudioCapture {
public:
	YangAlsaHandle(YangContext *pcontext);
	virtual ~YangAlsaHandle();
public:

	int32_t aIndex;
	YangAudioCaptureHandle *m_ahandle;
	void initRead();
	void initWrite();

	void setPreProcess(YangPreProcess *pp);
	int32_t init();
	void setInAudioBuffer(vector<YangAudioPlayBuffer*> *pal);
	void setCatureStart();
	void setCatureStop();
    void setOutAudioBuffer(YangAudioBuffer *pbuffer);
	void setPlayAudoBuffer(YangAudioBuffer *pbuffer);
    void setAec(YangRtcAec *paec);

protected:
	void run();
	 void startLoop();
	 void stopLoop();
private:

	int32_t m_readInit;
	int32_t m_writeInit, m_writeRet;
	vector<YangAudioPlayBuffer*> *m_in_audioBuffer;

	int32_t m_size;
	int32_t m_loops;
	int32_t m_channel;
	uint32_t  m_sample;
	snd_pcm_uframes_t m_frames;
	uint8_t *m_buffer;
	snd_pcm_t *m_readHandle;
	snd_pcm_t *m_writeHandle;

	int32_t hasData();

};
#endif
#endif /* YANGCAPTURE_SRC_YANGALSAHANDLE_H_ */
