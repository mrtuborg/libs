#include <stdio.h>
#include <string.h>
#include <extra/ortsTypes/ortsTypes.h>
#include "rcsCmd.h"
#include <math/crc/crc16.h>

rcsCmd::rcsCmd(BYTE serviceId, BYTE _funcId): func_id(_funcId),  func_paramsLength(0), func_params(0)
{
     
    // serviceId - for future purpose
}

rcsCmd::rcsCmd():func_id(0), func_paramsLength(0), func_params(0)
{

}

rcsCmd::rcsCmd(rcsCmd* cmdSrc)
{
	encode(cmdSrc->get_func_id(), cmdSrc->get_func_paramsLength(), cmdSrc->get_func_paramsPtr());
	makeSign();
}

rcsCmd::~rcsCmd()
{
}



BYTE rcsCmd::get_func_id()
{
    return func_id;
}

void rcsCmd::dbgPrint()
{
  printf("Функция #%d", func_id);
  printf(", Параметры [");
  if (func_paramsLength==0) printf (" ОТСУТСТВУЮТ ");
  else {
	for (WORD i=0; i<func_paramsLength; i++){
	    printf("%.2X ",*((BYTE*)func_params+i));
	}
  }
  printf("],  КС: %.4X\n", crc16_signature);
}

errType rcsCmd::decode(BYTE* data)
{
	errType result=err_not_init;

	if (func_paramsLength > MAX_RCS_PARAMS_LENGTH) result = err_params_decode;
	else {
		//1. Copyng static part of cmd type:
	    /*printf("size=%d\n",getCmdLength());
	    printf("param=[");
	    for (int i=0; i< getCmdLength(); i++) printf(" %.2X", ((BYTE*)cmd)[i]);
	    printf("]\n");
	   */
	    memcpy(data, &func_id, sizeof(BYTE));
	    data += sizeof(BYTE);
	    	memcpy(data, &func_paramsLength, sizeof(WORD));
	    	data += sizeof(WORD);
	    
 	//2. Decoding dynamic part from dataBlock:
	    memcpy(data, &func_params, func_paramsLength);
	    memcpy(data, &crc16_signature, sizeof(WORD));

	    result = err_result_ok;
	}


	return result;
}

errType rcsCmd::encode(const BYTE* data)
{
	errType result=err_not_init;
	
	func_id=data[0];
	func_paramsLength=*(WORD*)(data+sizeof(BYTE));

	if (func_paramsLength <= MAX_RCS_PARAMS_LENGTH) {
	    func_params=new BYTE[func_paramsLength];
	    memcpy(func_params, data+getDataPos(), func_paramsLength);
	 }
	 
	crc16_signature=*(WORD*)((BYTE*)data+getSignPos());
        
    return result;
}

errType rcsCmd::eraseParams()
{
	errType result=err_not_init;
	if (func_paramsLength!=0)
	{
	    func_paramsLength=0;
	    delete (BYTE*)func_params;
	    result=err_result_ok;
	}
	
	return result;
}


const void* rcsCmd::popParam(OrtsType type)
{
	BYTE* result=0;
	WORD size=0;
	BYTE* dataPtr=(BYTE*)func_params;
	
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
	//printf("popParam: size=%d, func_paramsLength=%d\n",size , cmd->func_paramsLength);
	//printf("pop size=%d\n",size);
	if (size <= func_paramsLength) { // we reduces func_paramsLength in every pop
		result=new BYTE[size];
		memcpy(result, dataPtr, size);
		BYTE* offset=((BYTE*)func_params)+size;
		if (size>getCmdLength()) {
			func_params=0;
			func_paramsLength=0;
		} else {
			func_params=offset;
			func_paramsLength-=size;// we reduces func_paramsLength in every pop
		}
	} else {

		result=0;
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
*/

	BYTE *newParams, *oldParams;
	if (!isVector) //Scalar type
	{
	    newParams=new BYTE[func_paramsLength+size];
	    memcpy(newParams, func_params, func_paramsLength);
	    memcpy(newParams + func_paramsLength, param, size);
	    func_paramsLength += size;
	}
	else { // Vector type
	    newParams=new BYTE[func_paramsLength+size+sizeof(WORD)];
	
	    memcpy(newParams, func_params, func_paramsLength); // Add old part of cmd
	
	    oldParams=newParams;
	    newParams=newParams+func_paramsLength;
	     // Create vector
	    *(WORD*)(newParams)=(WORD)size;
	    memcpy(newParams+sizeof(WORD), param, size); // Copy entity of vector
	    func_paramsLength+=size+sizeof(WORD); // Update common paramsLength

	    newParams=oldParams;
	  /*  printf("param=[");
	   	for (int i=0; i< cmd->func_paramsLength; i++) printf(" %.2X", ((BYTE*)newParams)[i]);
	   	printf("], size=%d\n", size);
	*/

	}
	
	delete (BYTE*)func_params;
	func_params=newParams;
	
	/*printf("new param length=%d\n",cmd->func_paramsLength);
	printf("all params=[");
	for (int i=0; i< cmd->func_paramsLength; i++) printf(" %.2X", ((BYTE*)cmd->func_params)[i]);
	printf("]\n");*/

	return result;
}

errType rcsCmd::encode(BYTE func_num, WORD par_length, const void* data)
{
	errType result=err_not_init;
	
        //1. Copyng static part of cmd type:
	     func_id=func_num;
	     func_paramsLength=par_length;
	     crc16_signature=0;
	     if (par_length <= MAX_RCS_PARAMS_LENGTH) {
	//2. Creating dynamic part of cmd type:
	        func_params=new BYTE[par_length];
	//3. Decoding dynamic part from dataBlock:
	        memcpy(func_params, data, par_length);
	    }
	return result;
}


WORD rcsCmd::get_func_paramsLength()
{
     return func_paramsLength;
}


WORD rcsCmd::getDataPos()
{
     return sizeof(func_id)+sizeof(func_paramsLength);
}
  
WORD rcsCmd::getSignPos()
{
     return func_paramsLength+sizeof(func_id)+sizeof(func_paramsLength);
}

WORD rcsCmd::getCmdLength()
{
     return func_paramsLength+sizeof(func_id)+sizeof(crc16_signature)+sizeof(func_paramsLength);
}
  
const void* rcsCmd::get_func_paramsPtr(WORD offset)
{
    return ((BYTE*)func_params+offset);
}

WORD rcsCmd::get_crc_sign()
{
    return crc16_signature;
}

errType rcsCmd::makeSign()
{
    crc16_signature=0;
    if (func_paramsLength>0) crc16_signature=CRC16_eval((BYTE*)func_params, func_paramsLength);
    crc16_signature=CRC16_eval((BYTE*)&func_paramsLength,sizeof(func_paramsLength),crc16_signature);
    crc16_signature=CRC16_eval((BYTE*)&func_id,sizeof(func_id),crc16_signature);
    	     
    return err_result_ok;
}

bool rcsCmd::checkSign()
{
    bool result=false;
    rcsCmd test_cmd;
    WORD test_sign;
    test_cmd.encode(func_id, func_paramsLength, (const BYTE*)func_params);
    test_cmd.makeSign();
    test_sign=test_cmd.get_crc_sign();
    
///*    if (verbose_level) {
//	printf("Расчётная подпись: %.4X\n",test_sign);
//	printf("Принятая подпись: %.4X\n",this->get_crc_sign());
//    }
//  */  
    if (test_sign==get_crc_sign()) result=true;
    return result;
}


std::ostream& operator<< (std::ostream& stream, rcsCmd &cmd)
{

	 stream << "Функция #"     << (int) cmd.get_func_id();
	 stream << ", Параметры [";
	 if (cmd.get_func_paramsLength() == 0) stream << " ОТСУТСТВУЮТ ";
	  else {
		for (WORD i=0; i<cmd.get_func_paramsLength(); i++){
		    stream << "%.2X " << *((BYTE*)cmd.get_func_paramsPtr(i));
		}
	  }
	  stream << "],  КС: %.4X\n" << cmd.get_crc_sign() << std::endl;

	  return stream;
}
