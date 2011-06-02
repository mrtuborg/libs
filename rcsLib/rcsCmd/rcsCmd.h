#ifndef RCSCMD_H
#define RCSCMD_H

#define MAX_RCS_PARAMS_LENGTH 1024

typedef struct rcsCmd_type {
    BYTE func_id;
    WORD func_paramsLength;
    void *func_params;
    WORD crc16_signature;

} __attribute__ ((packed)) rcsCmd_type;

class rcsCmd {
    rcsCmd_type* cmd;
    
public:
 //rcsCmd(rcsCmd* cmdSrc);
 explicit rcsCmd(const rcsCmd& src);

 rcsCmd(BYTE serviceId, BYTE funcId);
 rcsCmd();

 ~rcsCmd();

 BYTE get_func_id() const;
 WORD get_func_paramsLength() const;
 
 WORD getDataPos();
 WORD getSignPos();
 WORD getCmdLength() const;
 
 WORD get_crc_sign() const;
void* get_func_paramsPtr(WORD offset=0) const;


 void dbgPrint();
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
