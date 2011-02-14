#include <stdio.h>
#include <string.h>
#include <time.h>
#include <extra/ortsTypes/ortsTypes.h>
#include <rcsLib/rcsCmd/rcsCmd.h>

#include "job.h"


job::job(DWORD id, DWORD IPAddr, WORD UdpPort)
{
	jobReference = new job_type();
	jobReference->objId=id;
	jobReference->serviceIPaddr=IPAddr;
	jobReference->serviceUdpPort=UdpPort;
	jobEntity=new rcsCmd();
}

job::~job()
{


}

void job::encode(BYTE *data)
{
    WORD jobReferenceSize=sizeof(job_type);

    memcpy(jobReference, data, jobReferenceSize);
    jobEntity->encode(data+jobReferenceSize);
}

void job::decode(BYTE *data)
{

    WORD jobReferenceSize=sizeof(job_type);
    memcpy(data,jobReference, jobReferenceSize);
    jobEntity->decode(data+jobReferenceSize);
}


WORD job::getLength()
{
    return sizeof(job_type)+jobEntity->getCmdLength();
}

void job::dbgPrint()
{                       
    printf("object Id = %d\n", jobReference->objId);
    printf("next object Id = %d\n", jobReference->nextObjId);
    printf("start time = %d\n", jobReference->timeStart);
    printf("finish time = %d\n", jobReference->timeEnd);
    printf("calling service = %d\n", jobReference->service_id);
    printf("command:\n");
    jobEntity->dbgPrint();
}

errType job::set_dwNextJobID(DWORD id)
{
		jobReference->nextObjId=id;
		return err_result_ok;
}

errType job::set_dwStartTime(DWORD time)
{
		jobReference->timeStart=time;
		return err_result_ok;
}

errType job::set_dwFinishTime(DWORD time)
{
		jobReference->timeEnd=time;
		return err_result_ok;
}

errType job::set_btServiceId(BYTE id)
{
		jobReference->service_id=id;
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


DWORD job::get_dwObjId()   { return jobReference->objId;    }
DWORD job::get_dwNextObjId()   { return jobReference->nextObjId;}
WORD job::get_dwTimeStart() { return jobReference->timeStart; }
WORD job::get_dwTimeEnd()   { return jobReference->timeEnd;   }
BYTE job::get_btServId()   { return jobReference->service_id;}
BYTE job::get_btFuncId()   { return jobEntity->get_func_id();}
const void* job::get_paramsPtr(DWORD offset) { return jobEntity->get_func_paramsPtr(offset);}
WORD job::get_paramsLength(){return jobEntity->get_func_paramsLength();}
rcsCmd *job::cmd()    { return jobEntity;       }

DWORD job::get_dwServiceIPaddr(){ return jobReference->serviceIPaddr; }
WORD job::get_wServiceUdpPort(){ return jobReference->serviceUdpPort; }
