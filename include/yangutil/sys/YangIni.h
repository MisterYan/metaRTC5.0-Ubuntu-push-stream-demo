//
// Copyright (c) 2019-2022 yanggaofeng
//
#ifndef __YYangIni__
#define __YYangIni__
#include <yangutil/yangavinfotype.h>
#ifndef __ANDROID__

typedef struct{
	char* filename;
	void (*initAvinfo)(char* filename,YangAVInfo* avinfo);
	void (*initVideo)(char* filename,YangVideoInfo *video);
	void (*initAudio)(char* filename,YangAudioInfo *audio);
	void (*initSys)(char* filename,YangSysInfo *sys);
	void (*initEnc)(char* filename,YangVideoEncInfo *enc);
    void (*initRtc)(char* filename,YangRtcInfo *rtc);
    int32_t (*readStringValue)(char* filename,const char *section, const char *key,char *val, const char *p_defaultStr);
    int32_t (*readIntValue)(char* filename,const char *section, const char *key,	int32_t p_defaultInt);
}YangIni;

#ifdef __cplusplus
extern "C"{
#endif
void yang_create_ini(YangIni* ini,const char *p_filename);
void yang_destroy_ini(YangIni* ini);
#ifdef __cplusplus
}
#endif
#endif
#endif
