/*
 * task.cpp
 *
 *  Created on: May 20, 2011
 *      Author: tuborg
 */

#include "task.h"

task::task(job& jobRequest) {
	op = &jobRequest;

}

task::~task() {
	delete op;
}

BYTE task::getState()
{
	return state;
}

BYTE* task::lastAnswer(BYTE** result, WORD* len)
{
	*len = this->workResultLength;
	*result = (BYTE*) malloc(*len);
	if (*result) memcpy(*result, workResult, *len);
	return *result;
}

int task::run()
{
//	op->get_dwIPaddr(&inIP);
//	strIPAddr = inet_ntoa(inIP);
//
//	// 1) Prepare sockets for sending and reading
//	udpAction *send_action = new udpAction(ACTION_SEND, op->get_wUdpPort() + 1, strIPAddr);
//
//	udpAction *recv_action = new udpAction(ACTION_RECEIVE, op->get_wUdpPort() + 1, (const char*)"127.0.0.1", 2);
//
//	// 2) Prepare packet for sending
//	rcsCmd *packet = new rcsCmd();
//	BYTE* data = (BYTE*) op->get_cmd_paramsPtr();
//	WORD len = op->get_cmd_paramsLength();
//	packet->encode(op->get_cmd_btFuncId(), len, data);
//	packet->makeSign();
//
//	// 3) Send packet
//	send_action->writeDataAsCmd(packet);
//	send_action->processAction();
	return 0;
}


job* task::get_job()
{
	return op;
}
cronTask*  task::get_as_cronTask()
{
	return 0;
}

void task::setAnswer(BYTE* setAnswer, WORD length)
{

}

void task::dbgPrint()
{

}

void task::setState(BYTE newState)
{

}
