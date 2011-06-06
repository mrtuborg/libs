/*
 * actionReq.cpp
 *
 *  Created on: May 20, 2011
 *      Author: tuborg
 */

#include <string.h>
#include "actionReq.h"

actionReq::actionReq() {
	// TODO Auto-generated constructor stub

}

actionReq::~actionReq() {
	// TODO Auto-generated destructor stub
}

actionReq::actionReq(const job_type &header, const rcsCmd& cmd):job(header, cmd)
{

}

actionReq::actionReq(const BYTE* encode_array)
{
	job_type header;
	rcsCmd cmd;

	int offset = 0;
	memcpy(&header, encode_array, sizeof(job_type));
	offset += sizeof(job_type);
	cmd.encode(encode_array+offset);
	offset += cmd.getCmdLength();

	actionReq(header, cmd);
}

int actionReq::encode(BYTE* array)
{
	return job::encode(array);
}

int actionReq::decode(BYTE* array)
{
	return job::decode(array);
}

size_t actionReq::size()
{
	return job::size();
}

