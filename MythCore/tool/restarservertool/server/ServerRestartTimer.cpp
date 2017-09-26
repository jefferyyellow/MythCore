#include "ServerRestartTimer.h"
#include "EpollTcp.h"

void CRestartManager::CheckRestartTimer(int * nFd, int *nServerNum, int & nSize, int *nOutFd, int* nOutServerNum, int & nOutSize, CConfigFile & config)
{
	//传入的数组应保证能存放最大值
	if (nFd == NULL || nServerNum == NULL
		|| nOutFd == NULL || nOutServerNum == NULL)
	{
		return;
	}

	nSize = 0;
	nOutSize = 0;

	list<CServerRestartTimer>::iterator it = mServerTimerList.begin();
	for (; it != mServerTimerList.end(); )
	{
		if (CheckSucessRestart(it->nServerNum, config))
		{
			nFd[nSize] = it->nFd;
			nServerNum[nSize] = it->nServerNum;
			++nSize;
			it = mServerTimerList.erase(it);
		}
		else if (CheckRestartOutTime(it->nStartTime))
		{	
			ofstream out("log/serverreset.log", ios::out);

			if (out)
			{
				int nCurrTime = time(NULL);
				out << "currtime :  " << nCurrTime << "starttime : " << it->nStartTime << "diff : " << nCurrTime - it->nStartTime << endl;   //把a即写到文件里

				out.close();    //关闭文件
			}

			nOutFd[nOutSize] = it->nFd;
			nOutServerNum[nOutSize] = it->nServerNum;
			++nOutSize;
			it = mServerTimerList.erase(it);	
		}
		else
		{
			++it;
		}
	}
}

bool CRestartManager::CheckSucessRestart(int nServerNum, CConfigFile & config)
{
	char szConfigSection[MAX_STR_NUM] = { 0 };
	snprintf(szConfigSection, sizeof(szConfigSection), "serverinfo_%d", nServerNum);

	char szPath[MAX_STR_NUM] = { 0 };
	config.GetItemValue(szConfigSection, "gameserverpath", szPath, sizeof(szPath));

	fstream _file;
	_file.open(szPath, ios::in);
	if (!_file)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool CRestartManager::CheckRestartOutTime(int nRestartTime)
{
	int nCurrTime = time(NULL);
	return ((nCurrTime - nRestartTime)> SERVER_RESTART_TIME);
}

void CRestartManager::AddServerRestart(int nFd, int nServerNum, int nStartTime)
{
	CServerRestartTimer tTimer;
	tTimer.nFd = nFd;
	tTimer.nServerNum = nServerNum;
	tTimer.nStartTime = nStartTime;
	
	list<CServerRestartTimer>::iterator it = mServerTimerList.begin();
	for (; it != mServerTimerList.end();++it)
	{
		if (it->nFd == nFd && it->nServerNum == nServerNum)
		{
			mServerTimerList.erase(it);
			break;
		}
	}

	mServerTimerList.push_back(tTimer);
}
