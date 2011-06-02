/*
 * actionReq.h
 *
 *  Created on: May 20, 2011
 *      Author: tuborg
 */

#ifndef ACTIONREQ_H_
#define ACTIONREQ_H_

#include <extra/ortsTypes/ortsTypes.h>
#include <iostream>
#include "job.h"

class actionReq: public job
{
public:
	actionReq(const job_type &header, const rcsCmd& cmd);
	actionReq(const BYTE* encode_array);
	actionReq();
	size_t size();
	virtual ~actionReq();
	
	//friend std::ostream& operator<< (std::ostream& stream, actionReq &actRef);


private:

	int encode(BYTE*);
	int decode(BYTE*);
};

#endif /* ACTIONREQ_H_ */
