﻿
//
// Copyright (c) 2019-2022 yanggaofeng
//
#ifndef _WIN32
#include <yangaudiodev/linux/YangAudioCaptureLinux.h>
#include <yangavutil/audio/YangMakeWave.h>
#include <yangavutil/audio/YangAudioUtil.h>
#include "stdlib.h"
#include <stdio.h>
#include "memory.h"
#include "malloc.h"

YangAudioCaptureLinux::YangAudioCaptureLinux(YangContext *pcontext) //:YangAudioCapture(pcontext)
		{

	m_context = pcontext;
    m_ahandle = new YangAudioCaptureHandle(pcontext);
	aIndex = 0;
	m_size = 0;
	m_loops = 0;
	m_handle = NULL;
	m_buffer = NULL;

	m_channel = pcontext->avinfo.audio.channel;
	m_sample = pcontext->avinfo.audio.sample;
	if(pcontext->avinfo.audio.audioEncoderType==Yang_AED_AAC){
		m_frames = 1024;
	}else{
		m_frames=m_sample/50;
	}


	onlySupportSingle = 0;

}

YangAudioCaptureLinux::~YangAudioCaptureLinux() {
	if (m_loops) {
		stop();
		while (m_isStart) {
			yang_usleep(500);
		}
	}
	if (m_handle) {

		snd_pcm_close(m_handle);
		m_handle = NULL;
	}

	yang_delete(m_buffer);
	yang_delete(m_ahandle);
}
void YangAudioCaptureLinux::setCatureStart() {
	m_ahandle->isBuf = 1;
}
void YangAudioCaptureLinux::setCatureStop() {
	m_ahandle->isBuf = 0;
}
void YangAudioCaptureLinux::setOutAudioBuffer(YangAudioBuffer *pbuffer) {
	m_ahandle->setOutAudioBuffer(pbuffer);
}
void YangAudioCaptureLinux::setPlayAudoBuffer(YangAudioBuffer *pbuffer) {
	m_ahandle->m_aecPlayBuffer = pbuffer;
}
void YangAudioCaptureLinux::setAec(YangRtcAec *paec) {
	m_ahandle->m_aec = paec;
}
void YangAudioCaptureLinux::setInAudioBuffer(vector<YangAudioPlayBuffer*> *pal) {

}
void YangAudioCaptureLinux::setPreProcess(YangPreProcess *pp) {

}


int32_t YangAudioCaptureLinux::init() {
	int32_t dir = 0;
	snd_pcm_hw_params_t *hw_params;
	int32_t err = 0;
	char device_name[64] = { 0 };
	if (m_context->avinfo.audio.aIndex > -1)
		sprintf(device_name, "hw:%d,%d", m_context->avinfo.audio.aIndex, m_context->avinfo.audio.aSubIndex);

	if ((err = snd_pcm_open(&m_handle,
			m_context->avinfo.audio.aIndex == -1 ? "default" : device_name,
			SND_PCM_STREAM_CAPTURE, 0)) < 0) {

		yang_error("unable to open pcm device: %s\n", snd_strerror(err));
		_exit(1);
	}
	if ((err = snd_pcm_hw_params_malloc(&hw_params)) < 0) {

		yang_error("cannot allocate hardware parameter structure (%s)\n",
				snd_strerror(err));
		_exit(1);
	}

	if ((err = snd_pcm_hw_params_any(m_handle, hw_params)) < 0) {

		yang_error("cannot initialize hardware parameter structure (%s)\n",
				snd_strerror(err));
		_exit(1);
	}

	if ((err = snd_pcm_hw_params_set_access(m_handle, hw_params,
			SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {

		yang_error("cannot set access type (%s)\n", snd_strerror(err));
		_exit(1);
	}

	if ((err = snd_pcm_hw_params_set_format(m_handle, hw_params,
			SND_PCM_FORMAT_S16_LE)) < 0) {

		yang_error("cannot set sample format (%s)\n", snd_strerror(err));
		_exit(1);
	}


	if ((err = snd_pcm_hw_params_set_rate_near(m_handle, hw_params, &m_sample,
			0)) < 0) {

		yang_error("cannot set sample rate (%s)\n", snd_strerror(err));
		_exit(1);
	}

	if ((err = snd_pcm_hw_params_set_channels(m_handle, hw_params, m_channel))
			< 0) {
		yang_error("cannot set double channel  (%s)\n", snd_strerror(err));
		err = snd_pcm_hw_params_set_channels(m_handle, hw_params, 1);
		if (err < 0) {
			yang_error("cannot set single channel  (%s)\n", snd_strerror(err));
			_exit(1);
		}
		onlySupportSingle = 1;
		//_exit(1);
	}

	if ((err = snd_pcm_hw_params_set_period_size_near(m_handle, hw_params,
			&m_frames, &dir)) < 0) {

		yang_error("cannot set period size (%s)\n", snd_strerror(err));
		_exit(1);
	}

	if ((err = snd_pcm_hw_params(m_handle, hw_params)) < 0) {

		yang_error("cannot set parameters (%s)\n", snd_strerror(err));
		_exit(1);
	}

	snd_pcm_hw_params_free(hw_params);
	m_size = m_frames * 2 * m_channel; // 2 bytes/sample, 2 channels
	m_buffer = (uint8_t*) malloc(m_size);
	return Yang_Ok;
}

void YangAudioCaptureLinux::startLoop() {

	m_loops = 1;
	unsigned long status = 0;

	uint8_t *tmp = NULL;
	if (onlySupportSingle) {
		tmp = new uint8_t[m_frames * 2 * 2];
	}
	if ((status = snd_pcm_prepare(m_handle)) < 0) {

		yang_error("cannot prepare audio interface for use (%s)\n",
				snd_strerror(status));
		_exit(1);
	}
	int32_t audiolen = m_frames * m_channel * 2;
	while (m_loops == 1) {
		if ((status = snd_pcm_readi(m_handle, m_buffer, m_frames))
				!= m_frames) {

			yang_error("read from audio interface failed (%s)\n",
					snd_strerror(status));
			//   exit (1);
		}
		if (status == -EAGAIN) {
			//snd_pcm_wait(m_handle, 2 * m_channel);
			yang_error("An overrun has occured: %s\n", snd_strerror(status));
			status = 0;
		} else if (status < 0) {
			status = snd_pcm_recover(m_handle, status, 0);
			if (status < 0) {
				yang_error("ALSA read failed (unrecoverable): %s\n",
						snd_strerror(status));
				//return;
			}
			continue;
		}

		if (onlySupportSingle) {
			MonoToStereo((int16_t*) m_buffer, (int16_t*) tmp, m_frames);
			m_ahandle->putBuffer2(tmp,audiolen);
		} else {
			m_ahandle->putBuffer2(m_buffer,audiolen);
		}


	}

	snd_pcm_close(m_handle);
	yang_deleteA(tmp);
	free(m_buffer);
	m_handle = NULL;
	m_buffer = NULL;

}

void YangAudioCaptureLinux::stopLoop() {
	m_loops = 0;
}
#endif
