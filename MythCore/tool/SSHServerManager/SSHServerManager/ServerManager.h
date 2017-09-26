#pragma once
#include "Thread.h"
class CSSH2;

class CServerManager : public CThread
{
public:
	CServerManager(void);
	virtual ~CServerManager(void);

public:
	virtual void Run(); 
	virtual void PrepareStop();

public:
	void RestartServer();
	void StopServer();
	void MonitorServer();
	void SetServerTime();
	void UpdateServer();
	void UpdateServerDB();
	void ChmodScript();
	void OnServerStateChange(TmServerState eServerState);
	void OnServerManagerTypeChange(TmServerManagerType eServerManagerType);

public:	
	bool ConnectSSHServer(CSSH2* pSSH2, const char* pIP, const char* pUserName, char* pPassWord, bool bOpenTerm);
	void SetManagerData(TmServerManagerType eType, const char* pID, const char* pInternetIP, const char* pLocalIP, const char* pUserName, const char* pPassWord);
	void SetTime(char* pTime);

public:
	TmServerManagerType GetServerManagerType(){return mManagerType;}
	void				SetServerManagerType(TmServerManagerType eServerManagerType, bool bNotify)
	{
		if (mManagerType != eServerManagerType && bNotify)
		{
			OnServerManagerTypeChange(eServerManagerType);
		}
		mManagerType = eServerManagerType;
	}
	
	void				SetServerState(TmServerState eServerState)
	{
		if (mServerState != eServerState)
		{
			OnServerStateChange(eServerState);
		}
		mServerState = eServerState;
	}
	TmServerState		GetServerState(){return mServerState;}

	int					GetMonitorIndex(){return mMonitorIndex;}
	void				SetMonitorIndex(int nMonitorIndex){mMonitorIndex = nMonitorIndex;}

	bool				GetBatch(){return mBatch;}
	void				SetBatch(bool bBatch){mBatch = bBatch;}

private:
	char				mID[32];				// ������ID
	char				mInternetIP[32];		// ����IP
	char				mLocalIP[32];			// ����IP
	char				mUserName[32];			// �û���
	char				mPassWord[32];			// ����
	TmServerManagerType mManagerType;				// ��������
	TmServerState		mServerState;				// ������״̬
	int					mMonitorIndex;				// �������
	bool				mBatch;						// ����
};
