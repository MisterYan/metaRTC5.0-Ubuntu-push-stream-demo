//
// Copyright (c) 2019-2022 yanggaofeng
//
#ifndef SRC_YANGRTP_YANGRTPFUAPAYLOAD2_H_
#define SRC_YANGRTP_YANGRTPFUAPAYLOAD2_H_
#include <yangrtp/YangRtp.h>
#include <yangutil/yangavtype.h>
#include <yangutil/yangavtype_h265.h>
typedef struct Fua2H264DataCache{
    int32_t start;
    int32_t end;
    YangAvcNaluType nri;
    YangAvcNaluType nalu_type;
}Fua2H264DataCache;
typedef struct Fua2H265DataCache{
    int32_t start;
    int32_t end;
    YangHevcNaluType nri;
    YangHevcNaluType nalu_type;
}Fua2H265DataCache;
typedef struct YangFua2H264Data{
    // The FUA header.
	int32_t start;
	int32_t end;
    YangAvcNaluType nri;
    YangAvcNaluType nalu_type;
    int32_t nb;
    char* payload;
}YangFua2H264Data;
typedef struct YangFua2H265Data{
    // The FUA header.
	int32_t start;
	int32_t end;
    YangHevcNaluType nri;
    YangHevcNaluType nalu_type;
    int32_t nb;
    char* payload;
}YangFua2H265Data;
#ifdef __cplusplus
extern "C"{
#endif
int32_t yang_decode_h264_fua2(YangBuffer* buf,Fua2H264DataCache* pkt);
int32_t yang_decode_h265_fua2(YangBuffer* buf,Fua2H265DataCache* pkt);

int32_t yang_encode_h264_fua2(YangBuffer* buf,YangFua2H264Data* pkt);
int32_t yang_encode_h265_fua2(YangBuffer* buf,YangFua2H265Data* pkt);
#ifdef __cplusplus
}
#endif
#endif /* SRC_YANGRTP_YANGRTPFUAPAYLOAD2_H_ */
