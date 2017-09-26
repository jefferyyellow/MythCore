#include "StdAfx.h"
#include "ServerManager.h"
#include "SSH2.h"
#include "SSHServerManagerDlg.h"

CServerManager::CServerManager(void)
{
	mID[0] = '\0';
	mInternetIP[0] = '\0';
	mLocalIP[0] = '\0';
	mUserName[0] = '\0';
	mPassWord[0] = '\0';
	mManagerType = emServerManagerType_None;
	mServerState = emServerState_None;
}

CServerManager::~CServerManager(void)
{
}

bool CServerManager::ConnectSSHServer(CSSH2* pSSH2, const char* pIP, const char* pUserName, char* pPassWord, bool bOpenTerm)
{
	if (NULL == pSSH2 || NULL == pIP || NULL == pUserName || NULL == pPassWord)
	{
		return false;
	}
	bool bResult = pSSH2->InitSocket(pIP);
	if (!bResult)
	{
		wchar_t acBuffer[256] = {0};
		_snwprintf(acBuffer, sizeof(acBuffer), _T("%s　%s"), s2ws(string(mID)).c_str(),_T("连接服务器时创建套接字错误!"));
		::AfxMessageBox(acBuffer);
		return false;
	}

	bResult = pSSH2->CreateSession(pIP, pUserName, pPassWord);
	if (!bResult)
	{
		wchar_t acBuffer[256] = {0};
		_snwprintf(acBuffer, sizeof(acBuffer), _T("%s　%s"), s2ws(string(mID)).c_str(),_T("连接服务器时创建会话错误!"));
		::AfxMessageBox(acBuffer);
		return false;
	}

	bResult = pSSH2->OpenChannel();
	if (!bResult)
	{
		wchar_t acBuffer[256] = {0};
		_snwprintf(acBuffer, sizeof(acBuffer), _T("%s　%s"), s2ws(string(mID)).c_str(),_T("连接服务器时打开频道错误!"));
		::AfxMessageBox(acBuffer);
		return false;
	}

	if (bOpenTerm)
	{
		bResult = pSSH2 ->OperTerm();
		if (!bResult)
		{
			wchar_t acBuffer[256] = {0};
			_snwprintf(acBuffer, sizeof(acBuffer), _T("%s　%s"), s2ws(string(mID)).c_str(),_T("连接服务器时打开终端失败!"));
			::AfxMessageBox(acBuffer);
			return false;
		}
	}

	return true;
}



void CServerManager::SetManagerData(TmServerManagerType eType, const char* pID, const char* pInternetIP, const char* pLocalIP, const char* pUserName, const char* pPassWord)
{
	bool bNotify = false;
	if (eType == emServerManagerType_Monitor)
	{
		bNotify = true;
	}

	SetServerManagerType(eType, bNotify);
	mServerState = emServerState_None;
	strncpy(mID, pID, sizeof(mID));
	strncpy(mInternetIP, pInternetIP, sizeof(mInternetIP));
	strncpy(mLocalIP, pLocalIP, sizeof(mLocalIP));
	strncpy(mUserName, pUserName, sizeof(mUserName));
	strncpy(mPassWord, pPassWord, sizeof(mPassWord));
}

void CServerManager::Run()
{
	switch(mManagerType)
	{
	case emServerManagerType_Stop:
		{
			StopServer();
			StopThread(0, NULL);
			break;
		}
	case emServerManagerType_Restart:
		{
			RestartServer();
			StopThread(0, NULL);
			break;
		}
	case emServerManagerType_Monitor:
		{
			MonitorServer();
			break;
		}
	case emServerManagerType_SetTime:
		{
			SetServerTime();
			StopThread(0, NULL);
			break;
		}
	case emServerManagerType_Update:
		{
			UpdateServer();
			StopThread(0, NULL);
			break;
		}
	case emServerManagerType_UpdateDB:
		{
			UpdateServerDB();
			StopThread(0, NULL);
			break;
		}
	case emServerManagerType_ChmodScript:
		{
			ChmodScript();
			StopThread(0, NULL);
			break;
		}
	}
}

void CServerManager::PrepareStop()
{
	
}

void CServerManager::RestartServer()
{
	CSSH2 ssh2;
	if (!ConnectSSHServer(&ssh2, mInternetIP, mUserName, mPassWord, true))
	{
		SetServerManagerType(emServerManagerType_None, false);
		return;
	}
	//const char * pCommand = "sudo su\n cd\n source .bash_profile\n cd /data/wwwroot/qxzcode/server/bin\n ./restart.sh\n";
	//const char * pCommand = "cd server/server/bin\n ./restart.sh\n";
	ssh2.Write(gRestartCommand);
	char acBuffer[4096] = {0}; 
	char acMessage[256] = {0};

	bool bProxyServer = false;
	bool bDBServer = false;
	bool bSceneServer = false;
	bool bPlatServer = false;
	if (!gServerType)
	{
		bPlatServer = true;
	}
	
	int nCount = gRestartWaitTime;
	int nReceiveCount = 0;
	bool bSuccess = false;
	while(true)
	{
		int nResult = ssh2.Read(acBuffer + nReceiveCount, sizeof(acBuffer) - nReceiveCount - 1);
		if (nResult > 0)
		{
			nReceiveCount += nResult;
			if(!bProxyServer && strstr(acBuffer, "-proxyserver startup success"))
			{
				bProxyServer = true;
			}
			if(!bDBServer && strstr(acBuffer, "dataserver startup success"))
			{
				bDBServer = true;
			}
			if(!bSceneServer && strstr(acBuffer, "HerosLegend startup success"))
			{
				bSceneServer = true;
			}
			if (!bPlatServer && strstr(acBuffer, "GamePlatForm startup success"))
			{
				bPlatServer = true;
			}
			if (bProxyServer && bDBServer && bSceneServer && bPlatServer)
			{
				bSuccess = true;
				break;
			}

			nCount = gRestartWaitTime;
		}
		else
		{
			if (nResult == LIBSSH2_ERROR_EAGAIN)
			{
				Sleep(1000);
				nCount --;;
			}
			else
			{
				bSuccess = false;
				break;
			}
		}

		if (nCount <= 0)
		{
			break;
		}
	}

	if (bSuccess)
	{
		if(!mBatch)
		{
			::AfxMessageBox(_T("服务器重启完成!"));
		}
		else
		{
			strncpy(acMessage, "服务器重启完成!", sizeof(acMessage) - 1);
		}
	}
	else
	{
		strncpy(acMessage, "ERROR:服务器重启失败!", sizeof(acMessage) - 1);
	}

	if (mBatch)
	{
		CSSHServerManagerDlg *dlg = (CSSHServerManagerDlg*)AfxGetApp()->m_pMainWnd;
		if (NULL != dlg)
		{
			dlg->OnBatchComplete(bSuccess);
		}
	}
	else
	{
		CSSHServerManagerDlg *dlg = (CSSHServerManagerDlg*)AfxGetApp()->m_pMainWnd;
		if (NULL != dlg)
		{
			dlg->BatchEnableCtrl(true);
		}
	}
	acBuffer[nReceiveCount] = '\0';
	snprintf(acBuffer,sizeof(acBuffer) - 1, "%s\n\n\n%s%s\n\n\n", acBuffer, mID, acMessage);
	LOG_INFO(acBuffer);
	if (!bSuccess)
	{
		printf("%s", acBuffer);
	}

	SetServerManagerType(emServerManagerType_None, false);
}

void CServerManager::StopServer()
{
	CSSH2 ssh2;
	if (!ConnectSSHServer(&ssh2, mInternetIP, mUserName, mPassWord, true))
	{
		SetServerManagerType(emServerManagerType_None, false);
		return;
	}

	const char * pCommand = "sudo su\n cd /data/wwwroot/qxzcode/server/bin\n ./stop.sh\n";
	//const char * pCommand = "cd server/server/bin\n ./stop.sh\n";

	ssh2.Write(gStopCommand);

	bool bServerStop = false;
	int nCount = gStopWaitTime;
	char acBuffer[1024] = {0}; 
	char acMessage[256] = {0};
	int nReceiveCount = 0;

	while(true)
	{
		int nResult = ssh2.Read(acBuffer + nReceiveCount, sizeof(acBuffer) - nReceiveCount - 1);
		if (nResult > 0)
		{
			nReceiveCount += nResult;
			if(!bServerStop && strstr(acBuffer, "Delete share memory"))
			{
				bServerStop = true;
				break;
			}
			nCount = gStopWaitTime;
		}
		else
		{
			if (nResult == LIBSSH2_ERROR_EAGAIN)
			{
				Sleep(1000);
				nCount --;;
			}
			else
			{
				bServerStop = false;
				break;
			}
		}

		if (nCount <= 0)
		{
			break;
		}
	}

	if(bServerStop)
	{
		if(!mBatch)
		{
			::AfxMessageBox(_T("服务器关闭完成!"));
		}
		else
		{
			strncpy(acMessage, "服务器关闭完成", sizeof(acMessage) -1);
		}
	}
	else
	{
		strncpy(acMessage, "ERROR:服务器关闭失败", sizeof(acMessage) -1);
	}
	if (mBatch)
	{
		CSSHServerManagerDlg *dlg = (CSSHServerManagerDlg*)AfxGetApp()->m_pMainWnd;
		if (NULL != dlg)
		{
			dlg->OnBatchComplete(bServerStop);
		}
	}
	else
	{
		CSSHServerManagerDlg *dlg = (CSSHServerManagerDlg*)AfxGetApp()->m_pMainWnd;
		if (NULL != dlg)
		{
			dlg->BatchEnableCtrl(true);
		}
	}
	acBuffer[nReceiveCount] = '\0';
	snprintf(acBuffer,sizeof(acBuffer) - 1, "%s\n\n\n%s%s\n\n\n", acBuffer, mID, acMessage);
	LOG_INFO(acBuffer);

	SetServerManagerType(emServerManagerType_None, false);
	if (!bServerStop)
	{
		printf("%s", acBuffer);
	}
}

void CServerManager::MonitorServer()
{
	CSSH2 ssh2;
	if (!ConnectSSHServer(&ssh2, mInternetIP, mUserName, mPassWord, true))
	{
		SetServerManagerType(emServerManagerType_None, true);
		return;
	}
	//const char * pCommand1 = "sudo su\n cd /data/wwwroot/qxzcode/server/bin\n";
	ssh2.Write(gMonitor1Command);
	
	char acBuffer[1024] = {0}; 
	
	//const char * pCommand2="./monitor.sh\n";
	int nLength = strlen(gMonitor2Command) + 1;
	int nCount = 5;
	char *pSearch = NULL;

	int nControlCount = 0;

	while(true)
	{
		if (nControlCount <= 0)
		{
			ssh2.Write(gMonitor2Command);
			int nResult = ssh2.Read(acBuffer, sizeof(acBuffer));
			if (nResult > 0)
			{
				acBuffer[nResult] = '\0';
				pSearch = strstr(acBuffer, "./monitor.sh");
				printf("%s", acBuffer);
				if (NULL != pSearch)
				{
					nCount = 5;
					nControlCount = 200;
					int nPID = atoi(pSearch + nLength);
					if (nPID > 0)
					{
						SetServerState(emServerState_Running);
					}
					else
					{
						// 再一次查找
						pSearch = strstr(pSearch + nLength, "./monitor.sh");
						if (NULL != pSearch)
						{
							int nPID = atoi(pSearch + nLength);
							if (nPID > 0)
							{
								SetServerState(emServerState_Running);
							}
							else
							{
								nControlCount = 200;
								SetServerState(emServerState_Stop);
							}
						}
						else
						{
							SetServerState(emServerState_Stop);
						}
					}
				}
			
				else
				{
					nCount --;
					nControlCount = 10;
					if (nCount <= 0)
					{
						nCount = 5;
						nControlCount = 200;
						SetServerState(emServerState_Stop);
					}
				}
			}
			else
			{
				if (nResult == LIBSSH2_ERROR_EAGAIN)
				{
					nCount --;
					nControlCount = 10;
					if (nCount <= 0)
					{
						nCount = 5;
						nControlCount = 200;
						SetServerState(emServerState_Stop);
					}
				}
				else
				{					
					SetServerManagerType(emServerManagerType_None, true);
					LOG_INFO("ERROR:%s 服务器监控失败!", mID);
					break;
				}
			}
		}
		if (emThreadState_Stopping == GetThreadState())
		{
			break;
		}
		nControlCount --;
		Sleep(3000);
	}

	SetServerManagerType(emServerManagerType_None, true);
	SetServerState(emServerState_None);
	//acBuffer[nReceiveCount] = '\0';
	//snprintf(acBuffer,sizeof(acBuffer) - 1, "%s\n\n\n%s%s\n\n\n", acBuffer, mID, acMessage);
	//LOG_INFO(acBuffer);
}

void CServerManager::OnServerStateChange(TmServerState eServerState)
{
	CSSHServerManagerDlg *dlg = (CSSHServerManagerDlg*)AfxGetApp()->m_pMainWnd;
	if (NULL == dlg)
	{
		return;
	}
	dlg->OnServerStateChange(mMonitorIndex, eServerState);
}

void CServerManager::OnServerManagerTypeChange(TmServerManagerType eServerManagerType)
{
	CSSHServerManagerDlg *dlg = (CSSHServerManagerDlg*)AfxGetApp()->m_pMainWnd;
	if (NULL == dlg)
	{
		return;
	}
	dlg->OnServerManagerTypeChange(mMonitorIndex, eServerManagerType);
}

void CServerManager::SetServerTime()
{
	CSSH2 ssh2;
	if (!ConnectSSHServer(&ssh2, mInternetIP, mUserName, mPassWord, false))
	{
		SetServerManagerType(emServerManagerType_None, false);
		return;
	}

	const char * pCommand="date -s";
	char acBuffer[MAX_PATH] = {0};
	snprintf(acBuffer, sizeof(acBuffer), "%s '%s'", pCommand, gTime);
	ssh2.Exec(acBuffer);
	AfxMessageBox(_T("时间修改成功!"));

	SetServerManagerType(emServerManagerType_None, false);
}

void CServerManager::UpdateServer()
{
	CSSH2 ssh2;
	if (!ConnectSSHServer(&ssh2, mInternetIP, mUserName, mPassWord, true))
	{
		SetServerManagerType(emServerManagerType_None, false);
		return;
	}

	//const char * pCommand="sudo su\n cd /data/wwwroot/qxzcode/server\n svn up\n";
	//const char * pCommand="sudo su\n cd /data/wwwroot/qxzcode/server/bin\n chmod +x monitor.sh\n chmod +x restart.sh\n chmod +x updatedbtable.sh\n";
	ssh2.Write(gUpdateCommand);
	int nReceiveCount = 0;


	char acBuffer[4096] = {0};
	char acMessage[256] = {0};
	bool bSuccess = false;
	int nCount = gUpdateWaitTime;
	while(true)
	{
		int nResult = ssh2.Read(acBuffer + nReceiveCount, sizeof(acBuffer) - nReceiveCount - 1);
		if (nResult > 0)
		{
			nReceiveCount += nResult;
			if (strstr(acBuffer, "于修订版") || strstr(acBuffer, "At revision") || strstr(acBuffer, "Updated to revision"))
			{
				bSuccess = true;
				break;;
			}
			nCount = gUpdateWaitTime;
		}
		else
		{
			if (nResult == LIBSSH2_ERROR_EAGAIN)
			{
				Sleep(1000);
				-- nCount;
			}
			else
			{

				bSuccess = false;
				break;
			}
		}
		if (nCount <= 0)
		{
			break;
		}
	}

	if (bSuccess)
	{
		if(!mBatch)
		{
			AfxMessageBox(_T("服务器更新成功!"));
		}
		else
		{
			strncpy(acMessage, "服务器更新成功!", sizeof(acMessage) - 1);
		}
	}
	else
	{
		if(!mBatch)
		{
			AfxMessageBox((_T("服务器更新失败!")));
		}
		else
		{
			strncpy(acMessage, "ERROR:服务器更新失败!", sizeof(acMessage) - 1);
		}
	}

	if (mBatch)
	{
		CSSHServerManagerDlg *dlg = (CSSHServerManagerDlg*)AfxGetApp()->m_pMainWnd;
		if (NULL != dlg)
		{
			dlg->OnBatchComplete(bSuccess);
		}
	}
	else
	{
		CSSHServerManagerDlg *dlg = (CSSHServerManagerDlg*)AfxGetApp()->m_pMainWnd;
		if (NULL != dlg)
		{
			dlg->BatchEnableCtrl(true);
		}
	}
	SetServerManagerType(emServerManagerType_None, false);
	acBuffer[nReceiveCount] = '\0';
	snprintf(acBuffer,sizeof(acBuffer) - 1, "%s\n\n\n%s%s\n\n\n", acBuffer, mID, acMessage);
	LOG_INFO(acBuffer);
	if (!bSuccess)
	{
		printf("%s", acBuffer);
	}
}

void CServerManager::UpdateServerDB()
{
	CSSH2 ssh2;
	if (!ConnectSSHServer(&ssh2, mInternetIP, mUserName, mPassWord, true))
	{
		SetServerManagerType(emServerManagerType_None, false);
		return;
	}

	//const char * pCommand="cd server/server/bin\n ./updatedbtable.sh\n";
	char acCommand[MAX_PATH] = {0};
	snprintf(acCommand, sizeof(acCommand) - 1, "%s %s\n", gUpdatedbCommand, mLocalIP);
	ssh2.Write(acCommand);
	int nReceiveCount = 0;


	char acBuffer[4096] = {0};
	char acMessage[256] = {0};
	bool bSuccess = false;
	int nCount = gUpdateDBWaitTime;
	while(true)
	{
		int nResult = ssh2.Read(acBuffer + nReceiveCount, sizeof(acBuffer) - nReceiveCount - 1);
		if (nResult > 0)
		{
			nReceiveCount += nResult;
			if (strstr(acBuffer, "Access denied for user") || strstr(acBuffer, "Can't connect to local MySQL server"))
			{
				bSuccess = false;
				break;
			}
			else if (strstr(acBuffer, "Success"))
			{
				bSuccess = true;
				break;;
			}
			nCount = gUpdateDBWaitTime;
		}
		else
		{
			if (nResult == LIBSSH2_ERROR_EAGAIN)
			{
				Sleep(1000);
				-- nCount;
			}
			else
			{
				bSuccess = false;
				break;
			}
		}
		if (nCount <= 0)
		{
			break;
		}
	}

	if (bSuccess)
	{
		if(!mBatch)
		{
			AfxMessageBox(_T("服务器更新数据库成功!"));
		}
		else
		{
			strncpy(acMessage, "服务器更新数据库成功!", sizeof(acMessage) - 1);
		}
	}
	else
	{
		if(!mBatch)
		{
			AfxMessageBox((_T("服务器更新数据库失败!")));
		}
		else
		{
			strncpy(acMessage, "ERROR:服务器更新数据库失败!", sizeof(acMessage) - 1);
		}
	}

	if (mBatch)
	{
		CSSHServerManagerDlg *dlg = (CSSHServerManagerDlg*)AfxGetApp()->m_pMainWnd;
		if (NULL != dlg)
		{
			dlg->OnBatchComplete(bSuccess);
		}
	}
	else
	{
		CSSHServerManagerDlg *dlg = (CSSHServerManagerDlg*)AfxGetApp()->m_pMainWnd;
		if (NULL != dlg)
		{
			dlg->BatchEnableCtrl(true);
		}
	}
	SetServerManagerType(emServerManagerType_None, false);
	acBuffer[nReceiveCount] = '\0';
	snprintf(acBuffer,sizeof(acBuffer) - 1, "%s\n\n\n%s%s\n\n\n", acBuffer, mID, acMessage);
	LOG_INFO(acBuffer);
	if (!bSuccess)
	{
		printf("%s", acBuffer);
	}
}

void CServerManager::ChmodScript()
{
	CSSH2 ssh2;
	if (!ConnectSSHServer(&ssh2, mInternetIP, mUserName, mPassWord, true))
	{
		SetServerManagerType(emServerManagerType_None, false);
		return;
	}

	//const char * pCommand="sudo su\n cd /data/wwwroot/qxzcode/server\n svn up\n";
	//const char * pCommand="sudo su\n cd /data/wwwroot/qxzcode/server/bin\n chmod +x monitor.sh\n chmod +x restart.sh\n chmod +x updatedbtable.sh\n";
	ssh2.Write(gChmodScriptCommand);
	int nReceiveCount = 0;


	char acBuffer[1024] = {0};
	char acMessage[256] = {0};
	bool bSuccess = true;
	int nCount = 40;
	while(true)
	{
		int nResult = ssh2.Read(acBuffer + nReceiveCount, sizeof(acBuffer) - nReceiveCount - 1);
		if (nResult > 0)
		{
			nReceiveCount += nResult;
			nCount = 40;
		}
		else
		{
			if (nResult == LIBSSH2_ERROR_EAGAIN)
			{
				Sleep(300);
				-- nCount;
			}
			else
			{

				break;
			}
		}
		if (nCount <= 0)
		{
			break;
		}
	}

	if (bSuccess)
	{
		if(!mBatch)
		{
			AfxMessageBox(_T("修改权限成功!"));
		}
		else
		{
			strncpy(acMessage, "修改权限成功!", sizeof(acMessage) - 1);
		}
	}

	if (mBatch)
	{
		CSSHServerManagerDlg *dlg = (CSSHServerManagerDlg*)AfxGetApp()->m_pMainWnd;
		if (NULL != dlg)
		{
			dlg->OnBatchComplete(bSuccess);
		}
	}
	else
	{
		CSSHServerManagerDlg *dlg = (CSSHServerManagerDlg*)AfxGetApp()->m_pMainWnd;
		if (NULL != dlg)
		{
			dlg->BatchEnableCtrl(true);
		}
	}
	SetServerManagerType(emServerManagerType_None, false);
	acBuffer[nReceiveCount] = '\0';
	snprintf(acBuffer,sizeof(acBuffer) - 1, "%s\n\n\n%s%s\n\n\n", acBuffer, mID, acMessage);
	LOG_INFO(acBuffer);
	if (!bSuccess)
	{
		printf("%s", acBuffer);
	}
}