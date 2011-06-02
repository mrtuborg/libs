#include <stdio.h>
#include <string.h>
#include <iostream>
#include "job.h"


job::job(const job_type &header, const rcsCmd& cmd): jobEntity(cmd)
{
	jobReference.packetNum = header.packetNum;
	jobReference.opId = header.opId;
	jobReference.nextOpId = header.nextOpId;
	jobReference.timeStart = header.timeStart;
	jobReference.timeLong = header.timeLong;
	jobReference.serviceIPaddr = header.serviceIPaddr;
	jobReference.serviceUdpPort = header.serviceUdpPort;
}


job::job(DWORD id)
{
	jobReference.opId = id;
}

job::~job()
{
}

int job::define_init_service(const sockaddr_in& service_addr)
{
	jobReference.serviceIPaddr = ntohl(service_addr.sin_addr.s_addr);
	jobReference.serviceUdpPort = ntohs(service_addr.sin_port);
	return 0;
}

size_t job::size()
{
	    return sizeof(job_type)+jobEntity.getCmdLength();
}

BYTE  job::get_cmd_btFuncId(void)
{
	  return jobEntity.get_func_id();
}

const BYTE* job::get_cmd_paramsPtr(size_t offset)
{
	  return (const BYTE*)jobEntity.get_func_paramsPtr(offset);
}

WORD  job::get_cmd_paramsLength(void)
{
	  return jobEntity.get_func_paramsLength();
}

DWORD job::get_dwIPaddr(struct in_addr* out)
{
	return jobReference.serviceIPaddr;
}

//void job::dbgPrint()
//{
//		printf("object Id = %d\n", jobReference.opId);
//	    printf("next object Id = %d\n", jobReference.nextOpId);
//	    printf("start time = %d\n", jobReference.timeStart);
//	    printf("finish time = %d\n", jobReference.timeStart+jobReference.timeLong);
//	    printf("command:\n");
//	    jobEntity.dbgPrint();
//}

int job::encode(BYTE* array)
{
		WORD jobReferenceSize = sizeof (job_type);
	    WORD rcsCmdSize       = sizeof (rcsCmd);

	    memcpy(&jobReference, array, jobReferenceSize);
	    jobEntity.encode(array+jobReferenceSize);
	    return 0;
}

int job::decode(BYTE* array)
{
		WORD jobReferenceSize = sizeof(job_type);
	    WORD rcsCmdSize       = sizeof (rcsCmd);

	    memcpy(array,&jobReference, jobReferenceSize);
	    jobEntity.decode(array+jobReferenceSize);
	    return 0;
}

WORD job::get_wUdpPort()
{
	return jobReference.serviceUdpPort;
}
BYTE job::get_bPacketNum()
{
	return jobReference.packetNum;
}

DWORD job::get_dwTimeStart()
{
	   return this->jobReference.timeStart;
}

DWORD job::get_dwTimeLong()
{
	   return this->jobReference.timeLong;
}

DWORD job::get_dwOpId()
{
		return this->jobReference.opId;
}

DWORD job::get_dwNextOpId()
{
		return this->jobReference.nextOpId;
}

rcsCmd& job::rcscmd() // TODO: need return a const !!!
{
	  return jobEntity;
}

std::istream& operator>> (std::istream& stream, job &jobRef)
{

	return stream;
}

std::ostream& operator<< (std::ostream& stream, job &jobRef)
{
	stream << "packetNum      = " << jobRef.get_bPacketNum()						 << std::endl;
	stream << "object Id      = " << jobRef.get_dwOpId()                             << std::endl;
	stream << "next object Id = " << jobRef.get_dwNextOpId()                         << std::endl;
	stream << "start time     = " << jobRef.get_dwTimeStart()                        << std::endl;
	stream << "finish time    = " << jobRef.get_dwTimeStart()+jobRef.get_dwTimeLong()<< std::endl;
	stream << "IP			  = " << jobRef.get_dwIPaddr()							 << std::endl;
	stream << "UDP			  = " << jobRef.get_wUdpPort()							 << std::endl;
	
//	stream << jobRef.rcscmd();

	return stream;
}
