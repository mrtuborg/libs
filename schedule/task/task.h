/*
 * task.h
 *
 *  Created on: May 20, 2011
 *      Author: tuborg
 */

#ifndef TASK_H_
#define TASK_H_

#include <schedule/job/actionReq.h>
#include <schedule/job/conditionReq.h>
#include <schedule/cron/cronTask.h>

class task {

	  job* op;

      BYTE* workResult;
      WORD  workResultLength;
      BYTE  state; // 0 - initialized, 1 - running, 2 - completed
public:
	task(job& jobRequest);
	virtual ~task();

	void encode(BYTE*);
	void decode(BYTE*);

	int run();

	BYTE getState();
	BYTE* lastAnswer(BYTE** result, WORD* len);

	void setState(BYTE newState);
	void setAnswer(BYTE* setAnswer, WORD length);

	job* get_job();

	cronTask*  get_as_cronTask();

	void dbgPrint();

};

#endif /* TASK_H_ */