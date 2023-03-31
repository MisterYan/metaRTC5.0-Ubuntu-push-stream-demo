//
// Copyright (c) 2019-2022 yanggaofeng
//
#ifndef __YangVideoEncoderBuffer__
#define __YangVideoEncoderBuffer__

#include <yangutil/buffer/YangMediaBuffer.h>

class YangVideoEncoderBuffer:public YangMediaBuffer
{
public:
	YangVideoEncoderBuffer(int32_t pcachenum);
	~YangVideoEncoderBuffer(void);


	void getEVideo(YangFrame* pframe);
	uint8_t * getEVideoRef(YangFrame* frame);
	void putEVideo(YangFrame* pframe);
private:

};
#endif
