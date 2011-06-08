/*
 * conditionReq.cpp
 *
 *  Created on: May 20, 2011
 *      Author: tuborg
 */
#include <string.h>
#include "conditionReq.h"

conditionReq::conditionReq()
{

}

conditionReq::conditionReq(const job_type    &header,
                           const cond_job_t  &cond_header,
                           const rcsCmd_type &cmd,
                           const cond_mask_t &condition):

        job(header, cmd),
cond_labels(cond_header),
mask(condition)
{

}

//conditionReq::conditionReq(const BYTE *array) {
//	job_type		header;
//	cond_job_t  cond_header;
//	cond_mask_t cond;
//	rcsCmd cmd;
//
//	int offset = 0;
//	memcpy(&header, array, sizeof(job_type));
//	offset += sizeof(job_type);
//	memcpy(&cond_header, array, sizeof(cond_job_t));
//	offset += sizeof(cond_job_t);
//	cmd.encode(array+offset);
//	offset += cmd.getCmdLength();
//	memcpy(&cond, array+offset,sizeof(cond_mask_t));
//
//	conditionReq(header, cond_header, cmd, cond);
//
//}


conditionReq::~conditionReq() {
	// TODO Auto-generated destructor stub
}

int conditionReq::encode(BYTE* array)
{
	    job::encode(array);
	    array += job::size();
	    memcpy(array, &cond_labels, sizeof(cond_job_t));
	    array += sizeof(cond_job_t);
	    memcpy(array, &mask,   sizeof(cond_mask_t));
	    return 0;
}

int conditionReq::decode(BYTE* array)
{
		job::decode(array);
		array += job::size();
		memcpy(&cond_labels,   array, sizeof(cond_job_t));
		array += sizeof(cond_job_t);
		memcpy(&mask, array, sizeof(cond_mask_t));

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

bool conditionReq::makeTest(char *value)
{
	char* test_ptr = value + mask.test_mask_offset;
	char* result_value = new char[mask.test_mask_len];
	bool result = false;

	if (mask.test_mask_len > sizeof(DWORD)) mask.test_mask_len = sizeof(DWORD);
	switch(mask.test_mask_action)
	{
		case mask_AND:
			for(DWORD i = 0; i < mask.test_mask_len; i++)
			{
				result_value[i] = test_ptr[i] & (&mask.test_mask_def)[i];
				result |= result_value[i];
			}

			break;
		case mask_OR:
			for(DWORD i = 0; i < mask.test_mask_len; i++)
			{
				result_value[i] = test_ptr[i] | (&mask.test_mask_def)[i];
				result |= result_value[i];
			}

			break;

		default:
			break;
	}
	delete []result_value;
	return result;

}

int conditionReq::define_test_mask   (cond_mask_t &new_mask)
{
	mask = new_mask;
	return 0;
}

int conditionReq::define_test_mask   (DWORD mask_offset, DWORD mask_len, DWORD mask_value)
{
	mask.test_mask_offset = mask_offset;
	mask.test_mask_len    = mask_len;
	mask.test_mask_def    = mask_value;
	return 0;
}

int conditionReq::define_test_action ( mask_action_t mask_action)
{
	mask.test_mask_action = mask_action;
	return 0;
}

size_t conditionReq::size() const
{
	   return job::size()+sizeof(conditionReq);
}

rcsCmd& conditionReq::get_entity_Ptr()
{
		return job::get_entity_Ptr();
}

void conditionReq::dbgPrint()
{
	printf("conditionReq\n");
}
