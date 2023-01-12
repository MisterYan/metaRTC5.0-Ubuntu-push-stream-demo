﻿//
// Copyright (c) 2019-2022 yanggaofeng
//
#include <yangsdp/YangMediaDesc.h>
#include <yangutil/sys/YangLog.h>
#include <yangutil/sys/YangCString.h>
#include <yangsdp/YangMediaPayloadType.h>
#include <yangsdp/YangSSRCInfo.h>
#include <string.h>
#include <stdlib.h>
#define kCRLF  "\r\n"
#define Yang_Fetch_Failed 200
#define Yang_Fetch_Failed_WithDelim 200

yang_vector_impl(YangMediaDesc)
yang_vector_impl(YangCandidate)
yang_vector_impl(YangExtmap)

void yang_create_mediadesc(YangMediaDesc* desc){

}
void yang_destroy_mediadesc(YangMediaDesc* desc){
	if(desc==NULL) return;
	for(int i=0;i<desc->ssrc_groups.vsize;i++){
		yang_destroy_yangsdpintVector(&desc->ssrc_groups.payload[i].groups);
	}

	yang_destroy_YangCandidateVector(&desc->candidates);
	yang_destroy_YangExtmapVector(&desc->extmaps);
	for(int k=0;k<desc->payload_types.vsize;k++){
		yang_destroy_stringVector(&desc->payload_types.payload[k].rtcp_fb);
	}
	yang_destroy_YangMediaPayloadTypeVector(&desc->payload_types);
	yang_destroy_YangSSRCGroupVector(&desc->ssrc_groups);
	yang_destroy_YangSSRCInfoVector(&desc->ssrc_infos);
}
int32_t yang_encode_sessionInfo(YangSessionInfo *info, YangBuffer *os) {
	int32_t err = Yang_Ok;

	if (strlen(info->ice_ufrag)) {
		yang_write_cstring(os, "a=ice-ufrag:");
		yang_write_cstring(os, info->ice_ufrag);
		yang_write_cstring(os, kCRLF);

	}

	if (strlen(info->ice_pwd)) {
		yang_write_cstring(os, "a=ice-pwd:");
		yang_write_cstring(os, info->ice_pwd);
		yang_write_cstring(os, kCRLF);
	}

	// For ICE-lite, we never set the trickle.
	if (strlen(info->ice_options)) {
		yang_write_cstring(os, "a=ice-options:");
		yang_write_cstring(os, info->ice_options);
		yang_write_cstring(os, kCRLF);

	}

	if (strlen(info->fingerprint_algo) && strlen(info->fingerprint)) {
		yang_write_cstring(os, "a=fingerprint:");
		yang_write_cstring(os, info->fingerprint_algo);
		yang_write_cstring(os, " ");
		yang_write_cstring(os, info->fingerprint);
		yang_write_cstring(os, kCRLF);

	}

	if (strlen(info->setup)) {
		yang_write_cstring(os, "a=setup:");
		yang_write_cstring(os, info->setup);
		yang_write_cstring(os, kCRLF);
	}

	return err;
}

int32_t yang_encode_mediadesc(YangMediaDesc *desc, YangBuffer *os) {
	int32_t err = Yang_Ok;
	char intstr[32];
	memset(intstr, 0, sizeof(intstr));
	yang_itoa(desc->port, intstr, 10);

	yang_write_cstring(os, "m=");
	yang_write_cstring(os, desc->type);
	yang_write_cstring(os, " ");
	yang_write_cstring(os, intstr);
	yang_write_cstring(os, " ");

	yang_write_cstring(os, desc->protos);


	int i = 0;
	for (i = 0; i < desc->payload_types.vsize; i++) {
		yang_write_cstring(os, " ");
		memset(intstr, 0, sizeof(intstr));
		yang_itoa(desc->payload_types.payload[i].payload_type, intstr, 10);
		yang_write_cstring(os, intstr);
	}
	yang_write_cstring(os, kCRLF);

	yang_write_cstring(os, "c=IN IP4 0.0.0.0");
	yang_write_cstring(os, kCRLF);
	if ((err = yang_encode_sessionInfo(&desc->session_info, os)) != Yang_Ok) {
		return printf("encode session info failed");
	}
	yang_write_cstring(os, "a=mid:");
	yang_write_cstring(os, desc->mid);
	yang_write_cstring(os, kCRLF);

	if (strlen(desc->msid)) {
		yang_write_cstring(os, "a=msid:");
		yang_write_cstring(os, desc->msid);
		if (strlen(desc->msid_tracker)) {
			yang_write_cstring(os, " ");
			yang_write_cstring(os, desc->msid_tracker);

		}
		yang_write_cstring(os, kCRLF);
	}


	for (i = 0; i < desc->extmaps.vsize; i++) {
		yang_write_cstring(os, "a=extmap:");
		memset(intstr, 0, sizeof(intstr));
		yang_itoa(desc->extmaps.payload[i].mapid, intstr, 10);
		yang_write_cstring(os, intstr);
		yang_write_cstring(os, " ");
		yang_write_cstring(os, desc->extmaps.payload[i].extmap);
		yang_write_cstring(os, kCRLF);
	}

	if (desc->sendonly) {
		yang_write_cstring(os, "a=sendonly");
		yang_write_cstring(os, kCRLF);

	}
	if (desc->recvonly) {
		yang_write_cstring(os, "a=recvonly");
		yang_write_cstring(os, kCRLF);

	}
	if (desc->sendrecv) {
		yang_write_cstring(os, "a=sendrecv");
		yang_write_cstring(os, kCRLF);

	}
	if (desc->inactive) {
		yang_write_cstring(os, "a=inactive");
		yang_write_cstring(os, kCRLF);

	}

	if (desc->rtcp_mux) {
		yang_write_cstring(os, "a=rtcp-mux");
		yang_write_cstring(os, kCRLF);
	}

	if (desc->rtcp_rsize) {
		yang_write_cstring(os, "a=rtcp-rsize");
		yang_write_cstring(os, kCRLF);

	}


	for (i = 0; i < desc->payload_types.vsize; i++) {
		if ((err = yang_encode_mediapayloadtype(&desc->payload_types.payload[i],
				os)) != Yang_Ok) {
			return yang_error_wrap(1, "encode media payload failed");
		}
	}

	for (i = 0; i < desc->ssrc_infos.vsize; i++) {
		if ((err = yang_encode_ssrcinfo(&desc->ssrc_infos.payload[i], os))
				!= Yang_Ok) {
			return yang_error_wrap(1, "encode ssrc failed");
		}
	}


	if (desc->sctp_port) {
		yang_write_cstring(os, "a=sctp-port:5000");
		yang_write_cstring(os, kCRLF);
	}

	int32_t foundation = 0;
	int32_t component_id = 1; /* RTP */

	for (i = 0; i < desc->candidates.vsize; i++) {
		// @see: https://tools.ietf.org/html/draft-ietf-ice-rfc5245bis-00#section-4.2
		uint32_t priority = (1 << 24) * (126) + (1 << 8) * (65535)
				+ (1) * (256 - component_id);
		yang_write_cstring(os, "a=candidate:");
		memset(intstr, 0, sizeof(intstr));
		yang_itoa(foundation++, intstr, 10);
		yang_write_cstring(os, intstr);
		yang_write_cstring(os, " ");
		//
		memset(intstr, 0, sizeof(intstr));
		yang_itoa(component_id, intstr, 10);
		yang_write_cstring(os, intstr);
		yang_write_cstring(os, " udp ");
		memset(intstr, 0, sizeof(intstr));
		yang_itoa(priority, intstr, 10);
		yang_write_cstring(os, intstr);
		yang_write_cstring(os, " ");
		//
		yang_write_cstring(os, desc->candidates.payload[i].ip);
		yang_write_cstring(os, " ");
		memset(intstr, 0, sizeof(intstr));
		yang_itoa(desc->candidates.payload[i].port, intstr, 10);
		yang_write_cstring(os, intstr);
		//
		yang_write_cstring(os, " typ ");
		yang_write_cstring(os, desc->candidates.payload[i].type);
		//
		yang_write_cstring(os, " generation 0");
		yang_write_cstring(os, kCRLF);
		// @see: https://tools.ietf.org/id/draft-ietf-mmusic-ice-sip-sdp-14.html#rfc.section.5.1


	}

	return err;
}
int32_t yang_mediadesc_update_msid(YangMediaDesc *desc, char *id) {
	int32_t err = Yang_Ok;

	for (int i = 0; i < desc->ssrc_infos.vsize; i++) {
		YangSSRCInfo *info = &desc->ssrc_infos.payload[i];
		memset(info->msid, 0, sizeof(info->msid));
		strcpy(info->msid, id);
		memset(info->mslabel, 0, sizeof(info->mslabel));
		strcpy(info->mslabel, id);
	}
	return err;
}
int32_t yang_getparseAttribute_index(char *data) {
	int32_t size = strlen(data);
	for (int i = 0; i < size; i++) {
		if (data[i] == '\n' || data[i] == '\r' || data[i] == ' ')
			return i + 1;
	}
	return 0;
}
int32_t yang_sessioninfo_parse_attribute(YangSessionInfo *info, char *attribute,
		char *value) {
	int32_t err = Yang_Ok;

	if (strcmp(attribute, "ice-ufrag") == 0) {
		if(strlen(info->ice_ufrag)>0) return err;
		memset(info->ice_ufrag, 0, sizeof(info->ice_ufrag));
		strcpy(info->ice_ufrag, value);

	} else if (strcmp(attribute, "ice-pwd") == 0) {
		if(strlen(info->ice_pwd)>0) return err;
		memset(info->ice_pwd, 0, sizeof(info->ice_pwd));
		strcpy(info->ice_pwd, value);

	} else if (strcmp(attribute, "ice-options") == 0) {
		if(strlen(info->ice_options)>0) return err;
		memset(info->ice_options, 0, sizeof(info->ice_options));
		strcpy(info->ice_options, value);

	} else if (strcmp(attribute, "fingerprint") == 0) {
		if(strlen(info->fingerprint_algo)>0) return err;
		memset(info->fingerprint_algo, 0, sizeof(info->fingerprint_algo));
		memset(info->fingerprint, 0, sizeof(info->fingerprint));
		char *p = strstr(value, " ");
		if(p){
			memcpy(info->fingerprint_algo, value, p - value);
			strcpy(info->fingerprint, p + 1);
		}


	} else if (strcmp(attribute, "setup") == 0) {
		if(strlen(info->setup)>0) return err;
		// @see: https://tools.ietf.org/html/rfc4145#section-4
		memset(info->setup, 0, sizeof(info->setup));
		strcpy(info->setup, value);
	} else {

	}

	return err;
}


YangMediaPayloadType* yang_mediadesc_find_media_with_payload_type(
		YangMediaDesc *desc, int32_t payload_type) {
	for (size_t i = 0; i < desc->payload_types.vsize; ++i) {
		if (desc->payload_types.payload[i].payload_type == payload_type) {
			return &desc->payload_types.payload[i];
		}
	}

	return NULL;
}
int32_t yang_mediadesc_find_encoding_name(YangMediaDesc *desc,
		char *encoding_name) {

	for (size_t i = 0; i < desc->payload_types.vsize; ++i) {
		if (yang_strcmp(desc->payload_types.payload[i].encoding_name,
				encoding_name) == 0) {
			return 1;
		}
	}

	return 0;
}


int32_t yang_mediadesc_parse_attr_extmap(YangMediaDesc *desc, char *value) {
	int32_t err = Yang_Ok;

	char *p = strstr(value, " ");
	if (p == NULL)
		return ERROR_RTC_SDP_DECODE;
	char intstr[32];
	memset(intstr, 0, sizeof(intstr));
	memcpy(intstr, value, p - value);
	int32_t id = atoi(intstr);

	for (int i = 0; i < desc->extmaps.vsize; i++) { //= desc->extmaps.find(id)) {
		if (desc->extmaps.payload[i].mapid == id)
			return yang_error_wrap(ERROR_RTC_SDP_DECODE, "duplicate ext id: %d",
					id);
	}
	YangExtmap ext;
	memset(&ext, 0, sizeof(ext));
	ext.mapid = id;
	strcpy(ext.extmap, p + 1);
	yang_insert_YangExtmapVector(&desc->extmaps, &ext);

	return err;
}

int32_t yang_mediadesc_parse_attr_rtpmap(YangMediaDesc *desc, char *value) {
	int32_t err = Yang_Ok;
	// @see: https://tools.ietf.org/html/rfc4566#page-25
	// a=rtpmap:<payload type> <encoding name>/<clock rate> [/<encoding parameters>]


	char *p = strstr(value, " ");
	if (p == NULL)
		return ERROR_RTC_SDP_DECODE;
	char intstr[32];
	memset(intstr, 0, sizeof(intstr));
	memcpy(intstr, value, p - value);
	int32_t payload_type = atoi(intstr);



	YangMediaPayloadType *payload = yang_mediadesc_find_media_with_payload_type(
			desc, payload_type);
	if (payload == NULL) {
		return yang_error_wrap(ERROR_RTC_SDP_DECODE,
				"can not find payload %d when pase rtpmap", payload_type);
	}

	YangStrings str;
	yang_cstr_split(p + 1, "/", &str);

	if (str.vsize < 2) {
		yang_destroy_strings(&str);
		return yang_error_wrap(ERROR_RTC_SDP_DECODE, "invalid rtpmap line=%s",
				value);
	}

	strcpy(payload->encoding_name, str.str[0]);
	payload->clock_rate = atoi(str.str[1]);

	if (str.vsize == 3) {
		strcpy(payload->encoding_param, str.str[2]);
	}
	yang_destroy_strings(&str);
	return err;
}

int32_t yang_mediadesc_parse_attr_rtcp(YangMediaDesc *desc, char *value) {
	int32_t err = Yang_Ok;

	// TODO:parse rtcp attribute

	return err;
}

int32_t yang_mediadesc_parse_attr_rtcp_fb(YangMediaDesc *desc, char *value) {
	int32_t err = Yang_Ok;
	// @see: https://tools.ietf.org/html/rfc5104#section-7.1

	char *p = strstr(value, " ");
	if (p == NULL)
		return ERROR_RTC_SDP_DECODE;
	char intstr[32];
	memset(intstr, 0, sizeof(intstr));
	memcpy(intstr, value, p - value);
	int32_t payload_type = atoi(intstr);
	YangMediaPayloadType *payload = yang_mediadesc_find_media_with_payload_type(
			desc, payload_type);
	if (payload == NULL) {
		return yang_error_wrap(ERROR_RTC_SDP_DECODE,
				"can not find payload %d when pase rtcp-fb", payload_type);
	}

	yang_insert_stringVector(&payload->rtcp_fb, p + 1);


	return err;
}

int32_t yang_mediadesc_parse_attr_fmtp(YangMediaDesc *desc, char *value) {
	int32_t err = Yang_Ok;
	// @see: https://tools.ietf.org/html/rfc4566#page-30
	// a=fmtp:<format> <format specific parameters>
	YangStrings str;
	yang_cstr_split(value, " ", &str);

	if (str.vsize < 1) {
		yang_destroy_strings(&str);
		return yang_error_wrap(ERROR_RTC_SDP_DECODE,
				"can not find payload when pase fmtp");
	}

	int32_t payload_type = atoi(str.str[0]);

	YangMediaPayloadType *payload = yang_mediadesc_find_media_with_payload_type(
			desc, payload_type);
	if (payload == NULL) {
		yang_destroy_strings(&str);
		return yang_error_wrap(ERROR_RTC_SDP_DECODE,
				"can not find payload %d when pase fmtp", payload_type);
	}

	strcpy(payload->format_specific_param, str.str[1]);

	yang_destroy_strings(&str);
	return err;
}

int32_t yang_mediadesc_parse_attr_mid(YangMediaDesc *desc, char *value) {
	// @see: https://tools.ietf.org/html/rfc3388#section-3
	int32_t err = Yang_Ok;

	strcpy(desc->mid, value);

	return err;
}

int32_t yang_mediadesc_parse_attr_msid(YangMediaDesc *desc, char *value) {
	// @see: https://tools.ietf.org/id/draft-ietf-mmusic-msid-08.html#rfc.section.2
	// TODO: msid and msid_tracker
	int32_t err = Yang_Ok;
	YangStrings str;
	yang_cstr_split(value, " ", &str);
	if (str.vsize > 1) {

		strcpy(desc->msid, str.str[0]);
		strcpy(desc->msid_tracker, str.str[1]);
	}

	yang_destroy_strings(&str);
	return err;
}

int32_t yang_mediadesc_parse_attr_ssrc_group(YangMediaDesc *desc, char *value) {
	int32_t err = Yang_Ok;
	// @see: https://tools.ietf.org/html/rfc5576#section-4.2
	// a=ssrc-group:<semantics> <ssrc-id> ...

	YangStrings str;
    yang_cstr_split(value, " ", &str);
    if(str.vsize<3){
        yang_destroy_strings(&str);
        return yang_error_wrap(ERROR_RTC_SDP_DECODE,
                "invalid ssrc-group line=%s", value);
    }

	YangSSRCGroup group;
	memset(&group, 0, sizeof(group));
    strcpy(group.semantic,str.str[0]);
	yang_create_yangsdpintVector(&group.groups);

    for (size_t i = 1; i < str.vsize; ++i) {
		uint32_t ssrc = atoi(str.str[i]);
		yang_insert_yangsdpintVector(&group.groups, &ssrc);
	}
	yang_insert_YangSSRCGroupVector(&desc->ssrc_groups, &group);

	yang_destroy_strings(&str);
	return err;
}

YangSSRCInfo* yang_mediadesc_fetch_or_create_ssrc_info(YangMediaDesc *desc,
		uint32_t ssrc) {
	for (size_t i = 0; i < desc->ssrc_infos.vsize; ++i) {
		if (desc->ssrc_infos.payload[i].ssrc == ssrc) {
			return &desc->ssrc_infos.payload[i];
		}
	}

	YangSSRCInfo ssrc_info;
	memset(&ssrc_info, 0, sizeof(YangSSRCInfo));
	ssrc_info.ssrc = ssrc;
	yang_insert_YangSSRCInfoVector(&desc->ssrc_infos, &ssrc_info);

	return &desc->ssrc_infos.payload[desc->ssrc_infos.vsize - 1];
}

int32_t yang_mediadesc_parse_attr_ssrc(YangMediaDesc *desc, char *value) {
	int32_t err = Yang_Ok;
	// @see: https://tools.ietf.org/html/rfc5576#section-4.1


	char intstr[32];
	memset(intstr, 0, sizeof(intstr));
	uint32_t ssrc = 0;


	char *p = strstr(value, ":");
	memcpy(intstr, value, p - value);
	ssrc = atoi(intstr);

	char ssrc_attr[16];
	memset(ssrc_attr, 0, sizeof(ssrc_attr));

	p = strstr(value, " ");

	char *p1 = strstr(p + 1, ":");
	memcpy(ssrc_attr, p + 1, p1 - p - 1);

	char *ssrc_value = p1 + 1;

	YangSSRCInfo *ssrc_info = yang_mediadesc_fetch_or_create_ssrc_info(desc,
			ssrc);

	if (strcmp(ssrc_attr, "cname") == 0) {
		// @see: https://tools.ietf.org/html/rfc5576#section-6.1
		strcpy(ssrc_info->cname, ssrc_value);
		ssrc_info->ssrc = ssrc;
	} else if (strcmp(ssrc_attr, "msid") == 0) {
		// @see: https://tools.ietf.org/html/draft-alvestrand-mmusic-msid-00#section-2
		YangStrings str;
		yang_cstr_split(ssrc_value, " ", &str);

		if (str.vsize == 0) {
			return yang_error_wrap(ERROR_RTC_SDP_DECODE, "invalid ssrc line=%s",
					value);
		}

		strcpy(ssrc_info->msid, str.vsize == 1 ? ssrc_value : str.str[0]);

		if (str.vsize > 1) {
			strcpy(ssrc_info->msid_tracker, str.str[1]);
		}
		yang_destroy_strings(&str);
	} else if (strcmp(ssrc_attr, "mslabel") == 0) {
		strcpy(ssrc_info->mslabel, ssrc_value);
	} else if (strcmp(ssrc_attr, "label") == 0) {
		strcpy(ssrc_info->label, ssrc_value);
	}

	return err;
}
int32_t yang_mediadesc_parse_attr_candidate(YangMediaDesc *desc, char *value) {
	int32_t err = Yang_Ok;
	YangStrings str;
	yang_cstr_split(value, " ", &str);
	if(str.vsize<8) {
		yang_destroy_strings(&str);
		return yang_error_wrap(ERROR_RTC_SDP_DECODE, "invalid candidate line=%s",value);
	}
	YangCandidate candiate;
	memset(&candiate,0,sizeof(YangCandidate));

		strcpy(candiate.ip,str.str[4]);
		candiate.port=atoi(str.str[5]);
		strcpy(candiate.type,str.str[7]);

	yang_insert_YangCandidateVector(&desc->candidates, &candiate);

	yang_destroy_strings(&str);
	return err;
}
int32_t yang_mediadesc_parse_attribute(YangMediaDesc *desc, char *content) {
	int32_t err = Yang_Ok;
	char attribute[128];
	char value[128];
	memset(attribute, 0, sizeof(attribute));
	memset(value, 0, sizeof(value));

	char *p = strstr(content, ":");
	if (p) {
		memcpy(attribute, content, p - content);
		memcpy(value, p + 1, strlen(p + 1));
	} else {
		strcpy(attribute, content);
	}


	if (strcmp(attribute, "extmap") == 0) {
		return yang_mediadesc_parse_attr_extmap(desc, value);
	} else if (strcmp(attribute, "rtpmap") == 0) {
		return yang_mediadesc_parse_attr_rtpmap(desc, value);
	} else if (strcmp(attribute, "rtcp") == 0) {
		return yang_mediadesc_parse_attr_rtcp(desc, value);
	} else if (strcmp(attribute, "rtcp-fb") == 0) {
		return yang_mediadesc_parse_attr_rtcp_fb(desc, value);
	} else if (strcmp(attribute, "fmtp") == 0) {
		return yang_mediadesc_parse_attr_fmtp(desc, value);
	} else if (strcmp(attribute, "mid") == 0) {
		return yang_mediadesc_parse_attr_mid(desc, value);
	} else if (strcmp(attribute, "msid") == 0) {
		return yang_mediadesc_parse_attr_msid(desc, value);
	} else if (strcmp(attribute, "ssrc") == 0) {
		return yang_mediadesc_parse_attr_ssrc(desc, value);
	} else if (strcmp(attribute, "ssrc-group") == 0) {
		return yang_mediadesc_parse_attr_ssrc_group(desc, value);
	} else if (strcmp(attribute, "candidate") == 0) {
		return yang_mediadesc_parse_attr_candidate(desc, value);
	} else if (strcmp(attribute, "rtcp-mux") == 0) {
		desc->rtcp_mux = true;
	} else if (strcmp(attribute, "rtcp-rsize") == 0) {
		desc->rtcp_rsize = true;
	} else if (strcmp(attribute, "recvonly") == 0) {
		desc->recvonly = true;
	} else if (strcmp(attribute, "sendonly") == 0) {
		desc->sendonly = true;
	} else if (strcmp(attribute, "sendrecv") == 0) {
		desc->sendrecv = true;
	} else if (strcmp(attribute, "inactive") == 0) {
		desc->inactive = true;
	} else {
		return yang_sessioninfo_parse_attribute(&desc->session_info, attribute,	value);
	}

	return err;
}

int32_t yang_mediadesc_parse_line(YangMediaDesc *desc, char *line) {
	int32_t err = Yang_Ok;
	char *content = line + 2;

	switch (line[0]) {
	case 'a': {
		return yang_mediadesc_parse_attribute(desc, content);
	}
	case 'c': {
		// TODO: process c-line
		break;
	}
	default: {
		yang_trace("ignore media line=%s", line);
		break;
	}
	}

	return err;
}
