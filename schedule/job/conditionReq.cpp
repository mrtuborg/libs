/*
 * conditionReq.cpp
 *
 *  Created on: May 20, 2011
 *      Author: tuborg
 */
#include <string.h>
#include "conditionReq.h"

conditionReq::conditionReq(const cond_job_t &header, const rcsCmd& cmd, const cond_mask_t& condition):job(header, cmd), mask(condition)
{

}

conditionReq::conditionReq(const BYTE *array) {
	cond_job_t header;
	cond_mask_t cond;
	rcsCmd cmd;

	int offset = 0;
	memcpy(&header, array, sizeof(cond_job_t));
	offset += sizeof(cond_job_t);
	cmd.encode(array+offset);
	offset += cmd.getCmdLength();
	memcpy(&cond, array+offset,sizeof(cond_mask_t));
	conditionReq(header, cmd, cond);

}

conditionReq::~conditionReq() {
	// TODO Auto-generated destructor stub
}

int conditionReq::encode(BYTE* array)
{
	    job::encode(array);
	    array += job::size();
	    memcpy(array, &mask,   sizeof(cond_mask_t));
	    array += sizeof(cond_mask_t);
	    memcpy(array, &action, sizeof(DWORD));

	    return 0;
}

int conditionReq::decode(BYTE* array)
{
		job::decode(array);
		array += job::size();
		memcpy(&mask,   array, sizeof(cond_mask_t));
		array += sizeof(cond_mask_t);
		memcpy(&action, array, sizeof(DWORD));

		return 0;
}

std::ostream &operator<<(std::ostream &stream, const conditionReq &task)
{
	stream << (job&) task;
	return stream;
}



std::istream &operator>>(std::istream &stream, conditionReq &task)
{
	stream >> (job&) task;
	return stream;
}

