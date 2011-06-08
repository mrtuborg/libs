/*
 * task.cpp
 *
 *  Created on: May 20, 2011
 *      Author: tuborg
 */

#include <arpa/inet.h>
#include <conv/stdformat/stdformat.h>
#include "task.h"

#define SS_SERVICE_COMAND_SELF_RUN 9

task::task() {

	//op = jobRequest;
	//memcpy(op, &jobRequest, jobRequest.size());

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

cronTask*  task::get_as_cronTask(unsigned int udp_port_num)
{
  cronTask* result = 0;
  /// 1. Define start time for cron task
  struct tm *ts;
  time_t timeStart(this->get_job()->get_dwTimeStart());
  ts = localtime(&timeStart);
  // printf("\n !!!!! TimeStart %d", timeStart);

  /// 2. Define IP address & UDP socket
  //WORD cmdLen(newJob->cmd()->getCmdLength());
  in_addr in;
  in.s_addr = inet_addr("127.0.0.1"); // ip_address of ss_Service_1
  char *ipaddr = (inet_ntoa(in));


  /// 3. Execution command
  int delay = ts->tm_sec;
  string strCmd(format("rcsSend -i%s -u%d -s%d -d\"", ipaddr, udp_port_num, delay));

  /// 3.1 Calling of 8 function in ss_Service_1
  rcsCmd *packet = new rcsCmd(1, 8);
  WORD len = 5;
  BYTE data[5];
  data[0] = 0; // TODO: Indetifier of packet data
  *(DWORD*) (data + 1) = get_job()->get_dwOpId();
  packet->encode(SS_SERVICE_COMAND_SELF_RUN, len, data);
  packet->makeSign();

  /// 4. Command space allocation
  BYTE* rcsCmdCommand = new BYTE[sizeof(BYTE) +
                                 sizeof(WORD) +
                                 packet->get_func_paramsLength() +
                                 sizeof(WORD)];
  /// 5. Filling command
  *(BYTE*) rcsCmdCommand = packet->get_func_id();
  *(DWORD*) (rcsCmdCommand + 1) = packet->get_func_paramsLength();
  memcpy(rcsCmdCommand + 3, packet->get_func_paramsPtr(0), packet->get_func_paramsLength());
  *(WORD*) (rcsCmdCommand + 3 + packet->get_func_paramsLength()) = packet->get_crc_sign();

  for (size_t i = 0; i < (sizeof(BYTE) + sizeof(WORD) + packet->get_func_paramsLength() + sizeof(WORD)); i++)
  {
 	  strCmd.append(format("%.2X ", ((BYTE *) rcsCmdCommand)[i]));
  }

  /// 6. Deallocating temporary values
  delete packet;
  delete rcsCmdCommand;


  strCmd.append("\"");

  /// 7. Create cronTask object
  result = new cronTask();
  result -> create (ts->tm_hour,
		  	  	    ts->tm_min,
		  	  	    ts->tm_mday,
		  	  	    ts->tm_mon + 1,
		  	  	    ts->tm_wday, //+1 - у крона месяц 1..12, у unixtime 0..11
 (unsigned long) get_job()->get_dwOpId(),
 (unsigned long) get_job()->get_dwNextOpId(),
  (unsigned int) get_job()->get_dwTimeLong(), strCmd);

  return result;
}

void task::setAnswer(BYTE* setAnswer, WORD length)
{

}

void task::dbgPrint()
{
	op->dbgPrint();
}

void task::setState(BYTE newState)
{

}

const task& task::operator= (const actionReq& jobRequest)
{
	op = new actionReq(jobRequest);
	memcpy (op, &jobRequest, jobRequest.size());
}

const task& task::operator= (const conditionReq& jobRequest)
{
	op = new conditionReq(jobRequest);
	memcpy (op, &jobRequest, jobRequest.size());

}
