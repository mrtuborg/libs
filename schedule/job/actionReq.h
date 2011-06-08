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
	actionReq(const actionReq& jobEntity);
	actionReq(const job_type &header, const rcsCmd_type& cmd);
	actionReq(const BYTE* encode_array);
	actionReq();
	size_t size() const;
	virtual ~actionReq();
	
	//friend std::ostream& operator<< (std::ostream& stream, actionReq &actRef);
	rcsCmd& get_entity_Ptr();

	void dbgPrint();

//private:

	int encode(BYTE*);
	int decode(BYTE*);
};

#endif /* ACTIONREQ_H_ */
