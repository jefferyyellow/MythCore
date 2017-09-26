
#ifndef __SERVER_RESTART_TIMER_H__
#define __SERVER_RESTART_TIMER_H__

#include <iostream>
#include <fstream>
#include "list"
using namespace std;

#define SERVER_RESTART_TIME 180
class  CConfigFile;

class CServerRestartTimer
{
public:
	int nFd;			// socket句柄
	int nServerNum;		// 重启的服务器
	int nStartTime;		// 重启时间
};

class CRestartManager
{
public:
	CRestartManager()
	{
		Init();
	}

	void Init()
	{
		mServerTimerList.clear();
	}

	void AddServerRestart(int nFd, int nServerNum, int nStartTime);

	void CheckRestartTimer(int * nFd, int *nServerNum, int & nSize, int *nOutFd, int* nOutServerNum, int & nOutSize, CConfigFile & config);

	bool CheckSucessRestart(int nServerNum, CConfigFile & config);
	bool CheckRestartOutTime(int nRestartTime);
private:
	list<CServerRestartTimer> mServerTimerList;
//	CServerRestartTimer mServerTimer[MAX_SOCKET_NUM];
};

#endif
