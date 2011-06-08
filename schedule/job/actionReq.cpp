/*
 * actionReq.cpp
 *
 *  Created on: May 20, 2011
 *      Author: tuborg
 */

#include <string.h>
#include "actionReq.h"


actionReq::actionReq(const actionReq& jobEntity)
{
	actionReq &entity = const_cast <actionReq&>  (jobEntity);

	job_type header = entity.get_header_Ptr();
	rcsCmd_type cmd = entity.get_entity_Ptr().get_cmd();
	actionReq(header, cmd);
}


actionReq::actionReq() {
	// TODO Auto-generated constructor stub

}

actionReq::~actionReq() {
	// TODO Auto-generated destructor stub
	printf("destructor\n");
}

actionReq::actionReq(const job_type &header, const rcsCmd_type & cmd):job(header, cmd)
{

}

//actionReq::actionReq(const BYTE* encode_array)
//{
//	job_type header;
//	rcsCmd cmd;
//	printf("------\n");
//	for(int i=0; i<28; i++) printf("%.2X ", encode_array[i]);
//	printf("\n------\n");
//	printf("ppt1\n");
//	int offset = 0;
//
//	memcpy(&header, encode_array, sizeof(job_type));
//
//	printf("ppt2\n");
//	offset += sizeof(job_type);
//	printf("length = %d\n", *(WORD*)(encode_array+offset+1));
//	cmd.encode(encode_array+offset);
//	printf("ppt3\n");
//	offset += cmd.getCmdLength();
//
//	job(header, cmd);
//	printf("ppt4\n");
//}

int actionReq::encode(BYTE* array)
{
	return job::encode(array);
}

int actionReq::decode(BYTE* array)
{
	return job::decode(array);
}

size_t actionReq::size() const
{
	return job::size();
}

rcsCmd& actionReq::get_entity_Ptr()
{
	return job::get_entity_Ptr();
}

void actionReq::dbgPrint()
{
	printf("actionReq\n");
}
