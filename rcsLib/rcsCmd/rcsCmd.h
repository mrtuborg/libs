
typedef struct rcsCmd_type {
    BYTE func_id;
    DWORD func_paramsLength;
    void *func_params;
    WORD crc16_signature;

} __attribute__ ((packed)) rcsCmd_type;

class rcsCmd {
    rcsCmd_type* cmd;
    
public:
 rcsCmd(BYTE serviceId, BYTE funcId);
 rcsCmd();
 ~rcsCmd();

 BYTE get_func_id();
 DWORD get_func_paramsLength();
 
 DWORD getDataPos();
 DWORD getSignPos();
 DWORD getCmdLength();
 
 WORD get_crc_sign();
 const void* get_func_paramsPtr(DWORD offset=0);


 void dbgPrint();
 errType decode(BYTE* dataBlock);
 errType eraseParams();
 errType pushParam(OrtsType, const void*);
 const void* popParam(OrtsType);
 
 errType encode(const BYTE* dataBlock);
 errType encode(BYTE funcId, DWORD len=0, const void* data=0);
 
 
 errType makeSign();

 bool checkSign();
} __attribute__ ((packed));
