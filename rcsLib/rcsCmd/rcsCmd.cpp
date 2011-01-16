#include <stdio.h>
#include <string.h>
#include "extra/ortsTypes/ortsTypes.h"
#include "rcsCmd.h"
#include "math/crc/crc16.h"

rcsCmd::rcsCmd(BYTE serviceId, BYTE funcId)
{
    cmd=new rcsCmd_type;
    cmd->func_id=funcId;
    
    cmd->func_params=0;
    cmd->func_paramsLength=0;
     
    // serviceId - for future purpose
}

rcsCmd::rcsCmd()
{
    cmd=new rcsCmd_type;
}


rcsCmd::~rcsCmd()
{
    delete cmd;
}

BYTE rcsCmd::get_func_id()
{
    return cmd->func_id;
}

void rcsCmd::dbgPrint()
{
  printf("Функция №%d", cmd->func_id);
  printf(", Параметры [");
  if (cmd->func_paramsLength==0) printf (" ОТСУТСТВУЮТ ");
  else {
	for (DWORD i=0; i<cmd->func_paramsLength; i++){
	    printf("%.2X ",*((BYTE*)cmd->func_params+i));
	}
  }
  printf("],  КС: %.4X\n", cmd->crc16_signature);
}

errType rcsCmd::decode(BYTE* data)
{
	errType result=err_not_init;
        //1. Copyng static part of cmd type:
	    /*printf("size=%d\n",getCmdLength());
	    printf("param=[");
	    for (int i=0; i< getCmdLength(); i++) printf(" %.2X", ((BYTE*)cmd)[i]);
	    printf("]\n");
	      */
	    memcpy(data, cmd, sizeof(cmd->func_id)+sizeof(cmd->func_paramsLength));
	    
 	//2. Decoding dynamic part from dataBlock:
	    memcpy(data+sizeof(cmd->func_id)+sizeof(cmd->func_paramsLength), cmd->func_params, cmd->func_paramsLength);
	    memcpy(data+sizeof(cmd->func_id)+sizeof(cmd->func_paramsLength)+cmd->func_paramsLength, &cmd->crc16_signature, sizeof(cmd->crc16_signature));
	return result;
}

errType rcsCmd::encode(const BYTE* data)
{
	errType result=err_not_init;
	
	cmd->func_id=data[0];
	cmd->func_paramsLength=*(DWORD*)(data+sizeof(cmd->func_id));
	
	if (cmd->func_paramsLength>0) {
	    cmd->func_params=new BYTE[cmd->func_paramsLength];                                                            
	    memcpy(cmd->func_params, data+getDataPos(), cmd->func_paramsLength);                                              
	 }
	 
	cmd->crc16_signature=*(WORD*)((BYTE*)data+getSignPos());
        
        return result;
}

errType rcsCmd::eraseParams()
{
	errType result=err_not_init;
	if (cmd->func_paramsLength!=0) 
	{
	    cmd->func_paramsLength=0;
	    delete (BYTE*)cmd->func_params;
	    result=err_result_ok;
	}
	
	return result;
}


const void* rcsCmd::popParam(OrtsType type)
{
	BYTE* result=0;
	WORD size=0;
	BYTE* dataPtr=(BYTE*)cmd->func_params;
	
	bool isVector=((type & 0xf0)!=0);
	
	
	/*printf("popParam=[");
	for (int i=0; i< getCmdLength(); i++) printf(" %.2X", dataPtr[i]);
	printf("]\n");
	*/
	
	if (isVector) {
	   // printf ("vector\n") ;
	    size=*(WORD*)dataPtr+sizeof(WORD); // if vector type
	    //dataPtr+=2;
	} else {
	    //printf ("scalar\n") ;
	    size=lenOrtsTypes[type];
	}
	
	//printf("pop size=%d\n",size);
	result=new BYTE[size];
	
	memcpy(result, dataPtr, size);
	BYTE* offset=((BYTE*)cmd->func_params)+size;
	if (size>getCmdLength()) {
	    cmd->func_params=0;
	    cmd->func_paramsLength=0;
	} else {
	    cmd->func_params=offset;
	    cmd->func_paramsLength-=size;
	}
	return result;
}

errType rcsCmd::pushParam(OrtsType type, const void* param)
{
	errType result=err_result_ok;
	WORD size=0;
	
	if (!param) return err_not_init;
	
	bool isVector=((type & 0xf0)!=0);
	
	if (isVector) {
	    size=*(WORD*)param; // if vector type
	    param=(BYTE*)param+2;
	} else {
	    size=lenOrtsTypes[type];
	}
/*	
	printf("size=%d\n",size);
	printf("param=[");
	for (int i=0; i< size; i++) printf(" %.2X", ((BYTE*)param)[i]);
	printf("]\n");
	
	printf("old param length=%d\n",cmd->func_paramsLength);
*/	
	
	
	BYTE* newParams;
	if (!isVector) //Scalar type
	{
	    newParams=new BYTE[cmd->func_paramsLength+size];
	    memcpy(newParams, cmd->func_params, cmd->func_paramsLength);
	    memcpy(newParams+cmd->func_paramsLength, param, size);
	    cmd->func_paramsLength+=size;
	}
	else { // Vector type
	    newParams=new BYTE[cmd->func_paramsLength+size+sizeof(WORD)];
	
	    memcpy(newParams, cmd->func_params, cmd->func_paramsLength);
	
	    *(newParams+cmd->func_paramsLength)=size;
	    memcpy(newParams+sizeof(WORD), param, size);
	    cmd->func_paramsLength+=size+sizeof(WORD);
	
	}
	
	delete (BYTE*)cmd->func_params;
	cmd->func_params=newParams;
	
	/*printf("new param length=%d\n",cmd->func_paramsLength);

	printf("all params=[");
	for (int i=0; i< cmd->func_paramsLength; i++) printf(" %.2X", ((BYTE*)cmd->func_params)[i]);
	printf("]\n");
	*/
	return result;
}

errType rcsCmd::encode(BYTE func_num, DWORD par_length, const BYTE* data)
{
	errType result=err_not_init;
	
        //1. Copyng static part of cmd type:
	     cmd->func_id=func_num;
	     cmd->func_paramsLength=par_length;
	     cmd->crc16_signature=0;
	     if (par_length>0) {
	//2. Creating dynamic part of cmd type:
	        cmd->func_params=new BYTE[par_length];
	//3. Decoding dynamic part from dataBlock:
	        memcpy(cmd->func_params, data, par_length);
	    }
	return result;
}


DWORD rcsCmd::get_func_paramsLength()
{
     return cmd->func_paramsLength;
}


DWORD rcsCmd::getDataPos()
{
     return sizeof(cmd->func_id)+sizeof(cmd->func_paramsLength);
}
  
DWORD rcsCmd::getSignPos()
{
     return cmd->func_paramsLength+sizeof(cmd->func_id)+sizeof(cmd->func_paramsLength);
}

DWORD rcsCmd::getCmdLength()
{
     return cmd->func_paramsLength+sizeof(cmd->func_id)+sizeof(cmd->crc16_signature)+sizeof(cmd->func_paramsLength);
}
  
const void* rcsCmd::get_func_paramsPtr(DWORD offset)
{
    return ((BYTE*)cmd->func_params+offset);
}

WORD rcsCmd::get_crc_sign()
{
    return cmd->crc16_signature;
}

errType rcsCmd::makeSign()
{
    cmd->crc16_signature=0;
    if (cmd->func_paramsLength>0) cmd->crc16_signature=CRC16_eval((BYTE*)cmd->func_params,cmd->func_paramsLength);
    cmd->crc16_signature=CRC16_eval((BYTE*)&cmd->func_paramsLength,sizeof(cmd->func_paramsLength),cmd->crc16_signature);
    cmd->crc16_signature=CRC16_eval((BYTE*)&cmd->func_id,sizeof(cmd->func_id),cmd->crc16_signature);
    	     
    return err_result_ok;
}

bool rcsCmd::checkSign()
{
    bool result=false;
    rcsCmd test_cmd;
    WORD test_sign;
    test_cmd.encode(cmd->func_id, cmd->func_paramsLength, (const BYTE*)cmd->func_params);
    test_cmd.makeSign();
    test_sign=test_cmd.get_crc_sign();
    
///*    if (verbose_level) {
	printf("Расчётная подпись: %.4X\n",test_sign);
	printf("Принятая подпись: %.4X\n",this->get_crc_sign());
//    }
//  */  
    if (test_sign==get_crc_sign()) result=true;
    return result;
}