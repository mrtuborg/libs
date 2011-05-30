#include <stdlib.h>

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include <extra/ortsTypes/ortsTypes.h>

#include "cronTask.h"

using namespace std;


void Tokenize(const  string & str,
              vector<string>& tokens,
              const  string & delimiters = " ")
{
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}


errType cronTask::create(unsigned mm, unsigned hh, unsigned set_mday, unsigned set_month, unsigned set_wday, unsigned long objId, unsigned long nextObjId, unsigned length_sec, std::string &set_command)
{
	   hour = hh;
	 minute = mm;
	   mday = set_mday;
	  month = set_month;
	   wday = set_wday;
	command = set_command;

	return err_result_ok;
}

errType cronTask::create(std::string &textLine)
{
	vector<string> tokens;
	vector<string>::const_iterator iter;
	for (iter=tokens.begin(); iter < tokens.end(); iter++) cout << *iter << endl;

	Tokenize(textLine, tokens, "\t#,");

	minute	= atoi(tokens[0].data());
	hour	= atoi(tokens[1].data());
	mday	= atoi(tokens[2].data());
	month	= atoi(tokens[3].data());
	wday	= atoi(tokens[4].data());
	command = tokens[5];

	return err_result_ok;
}

cronTask::cronTask(cronTask* cron)
{

}

bool cronTask::isValid()
{
	return false;
}
//errType cronTask::create(task* newTask)
//{
//  errType result = err_result_ok;
//  struct tm *ts;
//  time_t timeStart(newJob->get_dwTimeStart());
//  /// 1. Define start time for cron task
// // printf("\n !!!!! TimeStart %d", timeStart);
//  ts = localtime(&timeStart);
//  /// 2. Filling cron task
//  //WORD cmdLen(newJob->cmd()->getCmdLength());
//  in_addr in;
//  //	in.s_addr = htonl(inet_addr("127.0.0.1")); // ip_address of ss_Service_1
//  in.s_addr = inet_addr("127.0.0.1"); // ip_address of ss_Service_1
//
//  char *ipaddr = (inet_ntoa(in));
//  int uport(get_cpListenerPortNum()); // client udp_port of ss_Service_1
//
//  int delay = ts->tm_sec;
//
//  string strCmd(format("rcsSend -i%s -u%d -s%d -d\"", ipaddr, uport, delay));
//
//  //DWORD id = newJob->get_dwObjId();
//  //strCmd.append(format("%.8X", id));
//
//  //в качестве параметра - RcsCmd packet
//  rcsCmd *packet = new rcsCmd(1, 8);
//
//  BYTE data[5];
//  data[0] = 0; // TODO: Indetifier of packet data
//  *(DWORD*) (data + 1) = newJob->get_dwOpId();
//
//  WORD len = 5;
//
//  packet->encode(8, len, data);
//  packet->makeSign();
//
//  //BYTE* rcsCmdCommand = new BYTE[sizeof(BYTE) + sizeof(DWORD) + packet->get_func_paramsLength() + sizeof(WORD)]; //память под команду для расписания
//  BYTE* rcsCmdCommand = new BYTE[sizeof(BYTE) + sizeof(WORD)
//      + packet->get_func_paramsLength() + sizeof(WORD)]; //память под команду для расписания
//
//
//  *(BYTE*) rcsCmdCommand = packet->get_func_id();
//  *(DWORD*) (rcsCmdCommand + 1) = packet->get_func_paramsLength();
//  memcpy(rcsCmdCommand + 3, packet->get_func_paramsPtr(0),
//      packet->get_func_paramsLength());
//  *(WORD*) (rcsCmdCommand + 3 + packet->get_func_paramsLength())
//      = packet->get_crc_sign();
//
//  for (size_t i = 0; i < (sizeof(BYTE) + sizeof(WORD)
//      + packet->get_func_paramsLength() + sizeof(WORD)); i++)
//    {
//      strCmd.append(format("%.2X ", ((BYTE *) rcsCmdCommand)[i]));
//    }
//
//  delete packet;
//  delete rcsCmdCommand;
//
//  strCmd.append("\"");
//
//  /// 3. Create cronTask object
//  task->create(ts->tm_hour, ts->tm_min,
//      ts->tm_mday,
//      ts->tm_mon + 1,
//      ts->tm_wday, //+1 - у крона месяц 1..12, у unixtime 0..11
//      (unsigned long) newJob->get_dwOpId(),
//      (unsigned long) newJob->get_dwNextOpId(),
//      (unsigned) newJob->get_dwTimeLong(), strCmd);
//
//  cout << *task;
//
//  return result;
//}

cronTask::cronTask()
{

}

cronTask::~cronTask()
{

}


ostream &operator<<(ostream &stream, const cronTask &task)
{
		stream << "-------"   << endl;
		stream << task.hour   << ":" << task.minute << " on " << task.mday << "/" << task.month << " [" << task.wday << "]" << endl;
		stream << "command: " << task.command << endl;
		stream << "-------"   << endl;

		return stream;
}



istream &operator>>(istream &stream, cronTask &task)
{
		stream >> task.hour >> task.minute >> task.mday >> task.month >> task.wday >> task.command;
		return stream;
}



fstream &operator<<(fstream &stream, const cronTask &task)
{
		stream << task.hour << "\t" << task.minute << "\t" << task.mday << " \t"<< task.month << "\t*\t" << task.command ; //<< "#";
 // stream << "*\t*\t*\t*\t*\t" << task.command ; //<< "#";
	//	stream << task.meta;
		return stream;
}
