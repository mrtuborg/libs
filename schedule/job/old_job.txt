#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <extra/ortsTypes/ortsTypes.h>
#include <rcsLib/rcsCmd/rcsCmd.h>



#include "job.h"
job::job()
{
	jobEntity = new rcsCmd();
	ansEntity = new rcsCmd();

	state         = 0;
	workResult    = new BYTE[2]; // BYTE_VECTOR = NULL
	workResult[0] = 0;
	workResult[1] = 0;
}

/*job::job(DWORD id, DWORD IPAddr, WORD UdpPort)
{
	jobReference                 = new job_type();
	jobReference->opId           = id;
	jobReference->serviceIPaddr  = IPAddr;
	jobReference->answerIPaddr   = 0;
	jobReference->serviceUdpPort = UdpPort;
	jobReference->answerUdpPort  = 0;

	jobEntity = new rcsCmd();
	ansEntity = new rcsCmd();

	state         = 0;
	workResult    = new BYTE[2]; // BYTE_VECTOR = NULL
	workResult[0] = 0;
	workResult[1] = 0;
}*/

job::~job()
{
	delete jobReference;
	delete jobEntity;
	delete ansEntity;
	delete []workResult;

}

void job::encode(BYTE *data)
{
    WORD jobReferenceSize = sizeof (job_type);
    WORD rcsCmdSize       = sizeof (rcsCmd);

    memcpy(jobReference, data, jobReferenceSize);
    jobEntity->encode(data+jobReferenceSize);
    ansEntity->encode(data+jobReferenceSize+rcsCmdSize);
}

void job::decode(BYTE *data)
{

    WORD jobReferenceSize = sizeof(job_type);
    WORD rcsCmdSize       = sizeof (rcsCmd);

    memcpy(data,jobReference, jobReferenceSize);
    jobEntity->decode(data+jobReferenceSize);
    ansEntity->decode(data+jobReferenceSize+rcsCmdSize);
}


WORD job::size()
{
    return sizeof(job_type)+jobEntity->getCmdLength()+ansEntity->getCmdLength();
}

void job::dbgPrint()
{                       
    printf("object Id = %d\n", jobReference->opId);
    printf("next object Id = %d\n", jobReference->nextOpId);
    printf("start time = %d\n", jobReference->timeStart);
    printf("finish time = %d\n", jobReference->timeStart+jobReference->timeLong);
    printf("command:\n");
    jobEntity->dbgPrint();
}

errType job::set_dwNextJobID(DWORD id)
{
		jobReference->nextOpId=id;
		return err_result_ok;
}

errType job::set_dwStartTime(DWORD time)
{
		jobReference->timeStart=time;
		return err_result_ok;
}

errType job::set_dwLongTime(DWORD time)
{
		jobReference->timeLong=time;
		return err_result_ok;
}

errType	job::setJobCmd(BYTE func_id, WORD param_len, void* args)
{
		errType result=err_result_ok;
		jobEntity->encode(func_id, param_len, args);
		jobEntity->makeSign();
		return result;
}

errType	job::setJobCmd(BYTE* cmd)
{
		errType result=err_result_ok;

		jobEntity->encode(cmd);
		jobEntity->makeSign();

		return result;
}

errType	job::setAnsCmd(BYTE func_id, WORD param_len, void* args)
{
		errType result (err_result_ok);

		ansEntity->encode(func_id, param_len, args);
		ansEntity->makeSign();

		return result;
}

errType	job::setAnsCmd(BYTE* cmd)
{
		errType result (err_result_ok);

		ansEntity->encode(cmd);
		ansEntity->makeSign();

		return result;
}

DWORD job::get_dwOpId()   { return jobReference->opId;    }
DWORD job::get_dwNextOpId()   { return jobReference->nextOpId;}
DWORD job::get_dwTimeStart() { return jobReference->timeStart; }
WORD job::get_dwTimeLong()   { return jobReference->timeLong;   }

BYTE job::get_btFuncId()   { return jobEntity->get_func_id();}
BYTE job::get_ans_btFuncId()   { return ansEntity->get_func_id();}

void job::get_dwAnswerIPaddr(struct in_addr *ip) {
	DWORD hIPaddr = jobReference->answerIPaddr;
	memcpy(&(ip->s_addr),&(hIPaddr), sizeof(in_addr));
}

WORD job::get_wAnswerUdpPort() { return jobReference->answerUdpPort; }

const void* job::get_paramsPtr(DWORD offset) { return jobEntity->get_func_paramsPtr(offset);}
WORD job::get_paramsLength(){return jobEntity->get_func_paramsLength();}

const void* job::get_ans_paramsPtr(DWORD offset) { return ansEntity->get_func_paramsPtr(offset);}
WORD job::get_ans_paramsLength(){ return ansEntity->get_func_paramsLength();}

rcsCmd *job::cmd()    { return jobEntity;       }
rcsCmd *job::ans_cmd()    { return ansEntity;       }


void job::get_dwServiceIPaddr(struct in_addr *outIP)
{
	//DWORD hIPaddr = htonl(
			DWORD hIPaddr = jobReference->serviceIPaddr;
//	printf("%p\n",jobReference);
//	printf("IP = %d",jobReference->serviceIPaddr);
	memcpy(&(outIP->s_addr),&(hIPaddr), sizeof(jobReference->serviceIPaddr));

}
WORD  job::get_wServiceUdpPort(){ return jobReference->serviceUdpPort; }

BYTE job::getState() { return state; };
void job::lastAnswer(BYTE** answer, WORD *length) { *answer = workResult; *length = workResultLength; }

void job::setState(BYTE newState) { state = newState; }
void job::setAnswer(BYTE* setAnswer, WORD length)
{
	delete []workResult;
	workResult = new BYTE [length];
	memcpy (workResult, setAnswer, length);
	workResultLength = length;
}


BYTE job::checkAnswerByMask()
{
	BYTE* data = NULL;
	BYTE* res;
	BYTE result = 0;

	if (state != 1) return false;
	data = (BYTE*) ansEntity->get_func_paramsPtr(jobReference->mask_offset);
	res = new BYTE[jobReference->mask_len];

	for (int i = 0; i < jobReference->mask_len; i++)
	{
		res[i] = (data[i] & jobReference->mask_def );
		res[i] = res[i] - jobReference->mask_def;

		result += res[i];

	}

	return result;
}

void job::set_mask (DWORD offset, DWORD len, DWORD def)
{
	 jobReference->mask_offset = offset;
	 jobReference->mask_len    = len;
	 jobReference->mask_def    = def;
}
