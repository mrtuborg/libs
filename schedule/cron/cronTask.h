#ifndef CRONTASK_H
#define CRONTASK_H


using namespace std;

class cronTask{
public:
	bool isValid();

	errType create(std::string &textLine);

	errType create(unsigned hh,
			 unsigned mm,
			 unsigned mday,
			 unsigned month,
			 unsigned wday,
			 unsigned long objId,
			 unsigned long nextObjId,
			 unsigned length_sec,
			 std::string &command);

	cronTask(cronTask* cron);

	cronTask();
	~cronTask();

	friend ostream &operator<<(ostream &stream, const cronTask &task);
	friend istream &operator>>(istream &stream, cronTask &task);

	friend fstream &operator<<(fstream &stream, const cronTask &task);
	//friend ifstream &operator>>(ifstream &stream, cronTask &task);


private:
		int hour, minute, mday, month, wday;
		string command;
};

#endif
