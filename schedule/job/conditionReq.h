/*
 * conditionReq.h
 *
 *  Created on: May 20, 2011
 *      Author: tuborg
 */

#ifndef CONDITIONREQ_H_
#define CONDITIONREQ_H_
#include <iostream>

#include "job.h"

enum mask_action_t { mask_NOP = 0, mask_AND = 1, mask_OR = 2 };

typedef struct cond_job_t: job_type
{
	DWORD timeout_watchdog_sec;
	DWORD next_test_success_id;
	DWORD next_test_fail_id;
	DWORD next_test_timeout;
} cond_job_t;

typedef struct cond_mask_t
{
	mask_action_t test_mask_action;   // NOP = 0, AND = 1, OR = 2
	DWORD test_mask_offset;
    DWORD test_mask_len; // length of applying mask
    DWORD test_mask_def; // pattern of applying mask
} cond_mask_t;

class conditionReq: public job {
public:
	conditionReq(DWORD id, DWORD timeout_watchdog_sec, DWORD next_test_success_id, DWORD next_test_fail_id, DWORD next_test_timeout);
	conditionReq(const job_type &header, const cond_job_t &cond_header, const rcsCmd& cmd, const cond_mask_t& condition);
	conditionReq(const BYTE* encode_array);
//  conditionReq(const conditionReq& src);
	virtual ~conditionReq();

	bool makeTest(char* value);

	int define_test_mask   (cond_mask_t &mask);
	int define_test_mask   (DWORD mask_offset, DWORD mask_len, DWORD mask_value);
	int define_test_action ( mask_action_t mask_action);

// OPERATIONS:
//	conditionReq& operator=(const conditionReq& src);
//	bool operator== (const conditionReq& rvalue);
//	bool operator!= (const conditionReq& rvalue);

	friend std::ostream& operator>> (std::ostream& stream,  const conditionReq &task);
	friend std::istream& operator<< (std::istream& stream,  conditionReq &task);

	int encode(BYTE*);
	int decode(BYTE*);

private:
	DWORD action; 				//  transit to opID, if opId == curr_opId state++;
	 bool transmit_masked_value; // transmit to the next operation result value
	 cond_job_t  cond_labels;
	 cond_mask_t mask;

};

#endif /* CONDITIONREQ_H_ */
