﻿//
// Copyright (c) 2019-2022 yanggaofeng
//
#ifndef YangWinAudioApiRender_H
#define YangWinAudioApiRender_H
#ifdef _WIN32
#include <yangutil/buffer/YangAudioPlayBuffer.h>
#include <yangaudiodev/win/YangWinAudioApi.h>
#include <yangaudiodev/YangAudioPlay.h>

#include <yangavutil/audio/YangResample.h>
#include <yangavutil/audio/YangAudioMix.h>
#include <yangutil/yangavinfotype.h>

#include <audioclient.h>
#include <vector>

class YangWinAudioApiRender:public YangWinAudioApi,public YangAudioPlay
{
public:
    YangWinAudioApiRender(YangContext *pcontext);
virtual ~YangWinAudioApiRender();
public:
    int initRender();
    void setAec(YangRtcAec *paec);

    int playThread();
    int startRender();
    int stopRender();
    int render_10ms();
    int render_aec_10ms();

    int m_contextt;


    int init();
    int m_hasRenderEcho;
    int32_t m_loops;

protected:

    YangFrame m_audioFrame;
private:

    int32_t m_audioPlayCacheNum;

    int32_t m_size;
    YangResample m_res;

    uint8_t* m_resBuf;
    uint8_t* m_resTmp;


    int32_t m_bufLen;
    uint32_t m_blockSize;



protected:
    virtual void startLoop();
    void stopLoop();
   // void run();

    YangRtcAec *m_aec;
    HANDLE m_samplesReadyEvent;
private:

    IMMDeviceCollection* m_renderCollection;
    IMMDevice* m_deviceOut;
    IAudioClient* m_clientOut;
    IAudioRenderClient* m_renderClient;
    ISimpleAudioVolume* m_renderSimpleVolume;


    UINT32 m_bufferLength;

    int getSpeakerVolume(int& volume);
    int setSpeakerVolume(int volume);
    int getSpeakerMute(bool& enabled);
    int setSpeakerMute(bool enable);

    int initSpeaker(int pind);
    int initPlay(int pind);
    int setFormat(WAVEFORMATEX *pwfx);


   int  m_outputDeviceIndex;


   BYTE* m_dataBufP ;
   DWORD flags ;
   UINT32 padding;
   uint32_t framesAvailable;
   bool keepPlaying;

};
#endif
#endif // YangWinAudioApiRender_H
