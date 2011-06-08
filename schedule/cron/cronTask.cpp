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

cronTask::cronTask(const cronTask& cron)
{
		this->hour    = cron.get_hour();
		this->minute  = cron.get_minute();
		this->command = cron.get_command();
		this->mday    = cron.get_mday();
		this->wday    = cron.get_wday();
		this->month   = cron.get_month();
}

bool cronTask::isValid()
{
	// TODO: Add validation of cron!
	return true;
}


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
