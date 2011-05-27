#ifndef RCSCMD_H
#define RCSCMD_H

#define MAX_RCS_PARAMS_LENGTH 1024
#include <iostream>
#include <extra/ortsTypes/ortsTypes.h>

//typedef struct rcsCmd_type {
//    BYTE func_id;
//    WORD func_paramsLength;
//    void *func_params;
//    WORD crc16_signature;
//
//} __attribute__ ((packed)) rcsCmd_type;

class rcsCmd {
	BYTE func_id;
	WORD func_paramsLength;
	void *func_params;
	WORD crc16_signature;

public:
 rcsCmd(rcsCmd* cmdSrc);
 rcsCmd(BYTE serviceId, BYTE funcId);
 rcsCmd();

 ~rcsCmd();

 BYTE get_func_id();
 WORD get_func_paramsLength();
 
 WORD getDataPos();
 WORD getSignPos();
 WORD getCmdLength();
 
 WORD get_crc_sign();
 const void* get_func_paramsPtr(WORD offset=0);


 void dbgPrint();
 friend std::ostream& operator<< (std::ostream& stream, rcsCmd &cmdRef);

 errType decode(BYTE* dataBlock);
 errType eraseParams();
 errType pushParam(OrtsType, const void*);
 const void* popParam(OrtsType);

 errType encode(const BYTE* dataBlock);
 errType encode(BYTE funcId, WORD len=0, const void* data=0);
 
 
 errType makeSign();

 bool checkSign();
} __attribute__ ((packed));

#endif
