﻿//
// Copyright (c) 2019-2022 yanggaofeng
//
#ifndef ___YangRecAudioCaptureHandlee__
#define ___YangRecAudioCaptureHandlee__

#include <yangaudiodev/YangAudioCaptureHandle.h>
#include <yangutil/buffer/YangAudioBuffer.h>
#ifdef _WIN32

#include "qedit.h"

class YangRecAudioCaptureHandle: public ISampleGrabberCB,public YangAudioCaptureHandle
{
public:
    YangRecAudioCaptureHandle(YangContext *pcontext);
	virtual ~YangRecAudioCaptureHandle(void);
	int32_t   readStart;

	STDMETHODIMP_(ULONG) AddRef();
	    STDMETHODIMP_(ULONG) Release();
	    STDMETHODIMP QueryInterface(REFIID riid, void **ppvObject);
		STDMETHODIMP SampleCB(double Time, IMediaSample *pSample);
	    STDMETHODIMP BufferCB(double Time, BYTE *pBuffer, long BufferLen);
private:
	    uint8_t *m_buf;
	    int32_t m_len;

};
#endif
#endif
