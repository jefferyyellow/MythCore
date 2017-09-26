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
	char				mID[32];				// 服务器ID
	char				mInternetIP[32];		// 外网IP
	char				mLocalIP[32];			// 内网IP
	char				mUserName[32];			// 用户名
	char				mPassWord[32];			// 密码
	TmServerManagerType mManagerType;				// 管理类型
	TmServerState		mServerState;				// 服务器状态
	int					mMonitorIndex;				// 监控索引
	bool				mBatch;						// 批量
};
