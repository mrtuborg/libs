/*
 * cronTab.cpp
 *
 *  Created on: Jan 22, 2011
 *      Author: tuborg
 */
#include <stdio.h>
#include <time.h>

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>

#include <extra/ortsTypes/ortsTypes.h>
#include <schedule/cron/cronTask.h>
#include "cronTab.h"

//  .---------------- minute (0 - 59)
//  |  .------------- hour (0 - 23)
//  |   |   .---------- day of month (1 - 31)
//  |   |   |   .------- month (1 - 12) OR jan,feb,mar,apr ...
//  |   |   |   |  .----- day of week (0 - 6) (Sunday=0 or 7)  OR sun,mon,tue,wed,thu,fri,sat
//  |   |   |   |  |
//  *   *   *   *  *  command to be executed





cronTab::cronTab() {

        cronFile.open("cronfile.txt", ios::in | ios::out);

	cronFile.seekp(0, ios::end);
	writePosition=cronFile.tellp();

	cronFile.seekg(0, ios::beg);
	readPosition=cronFile.tellg();
	cronFile.close();

	system("crontab -r");
	this->clearCronFile();
}

cronTab::~cronTab() {


}

//errType cronTab::setCommand(struct tm *ts, DWORD objID, DWORD nextObjID, WORD length, char* newCommand)
//{
//	minute = ts->tm_min;
//	hour   = ts->tm_hour;
//	mday   = ts->tm_mday;
//	month  = ts->tm_mon+1;
//	wday   = ts->tm_wday;
//	length_sec = length;
//
//	sprintf(command, "ssProxy %s #objID=%d,nextObjID=%d,length_sec=%d", newCommand, objID, nextObjID, length_sec);
//
//	return err_result_ok;
//}

errType cronTab::addTask(cronTask* task)
{
	taskList.push_back(task);
	return err_result_ok;
}

//errType cronTab::getCommand(struct tm *ts, DWORD objID, DWORD nextObjID, DWORD finishTime, char* newCommand){
//
//	//fscanf();
//	sprintf(minute,"%d",ts->tm_min);
//	sprintf(hour, "%d", ts->tm_hour);
//	sprintf(day_of_month, "%d", ts->tm_mday);
//	sprintf(month, "%d", ts->tm_mon+1);
//	sprintf(day_of_week, "%d", ts->tm_wday);
//
//
//	sprintf(command, "ssProxy %s #objID=%d,nextObjID=%d,finishTime=%d", newCommand, objID, nextObjID, finishTime);
//
//	return err_result_ok;
//}

errType cronTab::clearCronFile()
{
	cronFile.open("cronfile.txt",ios::out | ios::trunc);
	cronFile.close();

	return err_result_ok;
}


errType cronTab::addToCronFile()
{
	errType result (err_not_init);
	cronTask *task=taskList.front();
	list <cronTask*>::iterator iter;

	int wr=0;

        cronFile.open("cronfile.txt", ios::in | ios::out | ios::trunc);
	if (cronFile.is_open()) {

		for (iter=taskList.begin(); iter!=taskList.end(); ++iter)
		{

			printf("wr=%d\n",wr++);
			task=(*iter);
			cout << *task << endl;
			cronFile << *task << endl;


		}


		writePosition=cronFile.tellp();
		cronFile.close();
		system("crontab cronfile.txt");
		result=err_result_ok;
	}

	cout << "writePosition = " << writePosition << endl;
	return result;
}

int cronTab::getFromCronFile()
{
	errType result (err_not_init);
	cronFile.open("/var/spool/cron/root", ios::in);
	int pos=0;
	//char textLine[256];
	//textLine[255]=0;
	string textLine;

	if (cronFile) {
		//cout << "readPosition to set = " << readPosition << endl;

		cronFile.clear();
		cronFile.seekg(readPosition, ios::beg);

		getline(cronFile, textLine);
		cronTask *task=new cronTask();
		task->create(textLine);
		cout << *task << endl;

		//dbgPrint();
		//fscanf(cronFile,"%s\n", command);
		//printf("readed: %s\n", command);
		readPosition=cronFile.tellg();

		if (readPosition >= writePosition) {
			readPosition=0;
			pos=-1;
		}
		//cout << "readPosition=" << readPosition << endl;
		cronFile.close();
		result=err_result_ok;
	}
	pos=readPosition;
	//printf("want to exec: %s/%s (%s) %s:%s command: %s\n", day_of_month, month, day_of_week, hour, minute, command);

	return pos;
}

void cronTab::dbgPrint()
{
		//cout << hour << ":" << minute << ", " << mday << "/" << month << "[" << wday << "]"<< " cmd: " << command << endl;

}

