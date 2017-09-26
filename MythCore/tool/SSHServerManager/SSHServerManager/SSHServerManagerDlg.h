// SSHServerManagerDlg.h : 头文件
//

#pragma once

#include "ServerManager.h"
#include "afxcmn.h"
// CSSHServerManagerDlg 对话框
#include "ServerInfo.h"
#include "resource.h"
#include "Logger.h"
class CSSHServerManagerDlg : public CDialog
{

public:
	typedef vector<CServerInfo> VEC_SERVER_INFO;
	typedef vector<CServerManager*> VEC_SERVER_MONITOR;
	typedef vector<CServerManager*> VEC_SERVER_EXECUTE;
// 构造
public:
	CSSHServerManagerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SSHSERVERMANAGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	void InitAuthorizeInfo();
	void InitManulServerInfo();
	void GetCommandInfo(TiXmlElement *pCommandElement, char* pCommand, char* pTargetCommand, int nLength, CString strErrMessage);
	void InitServerListInfo();
	void SaveServerListInfoToFile();
	int GetMonitorIndex();
	void GetExecute(int nIndex);
	void GetServerInfo(char* pIP, int nIPLength, char* pLocalIP, int nLocalIPLength, char* pUserName, int nUserNameLength, char* pPassWord, int nPassWordLength);
public:
	void OnServerStateChange(int nMonitorIndex, TmServerState eState);
	void OnServerManagerTypeChange(int nMonitorIndex, TmServerManagerType eType);
	void OnBatchComplete(bool bSuccess);
	void BatchEnableCtrl(bool bEnable);

// 实现
protected:
	HICON m_hIcon;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedManualCloseServer();
	afx_msg void OnBnClickedManualRestartServer();

private:
	CServerManager mServerManager;
	VEC_SERVER_MONITOR mServerMonitor;
	VEC_SERVER_EXECUTE mServerExecute;
	CCSLock mLock;
	
public:
	VEC_SERVER_INFO& GetServerListInfo(){return mServerListInfo;}
	CListCtrl&		 GetServerListCtrl(){return mServerListCtrl;}

private:
	CListCtrl mServerListCtrl;
	VEC_SERVER_INFO mServerListInfo;

private:
	TmBatchOperType	mBatchOperType;
	int				mBatchNum;
	int				mBatchSuccess;
	CLogger			mLogger;

public:
	afx_msg void OnBnClickedAddServerListItem();
	afx_msg void OnBnClickedEditServerListItem();
	afx_msg void OnBnClickedDeleteServerListItem();
	void AddServerInfo(CString strID, CString strIP, CString strLocalIP, CString strUserName, CString strPassWord);
	void SetServerInfo(CString strID, CString strIP, CString strLocalIP, CString strUserName, CString strPassWord);
	afx_msg void OnLvnItemchangedServerList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedManualMonitor();
	LRESULT OnUpdateMyData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedManualStopMonitor();
	afx_msg void OnBnClickedAllStopMonitor();
	afx_msg void OnBnClickedAllMonitor();
	void AllMonitor();
	afx_msg void OnBnClickedAllStopServer();
	void AllStopServer();
	afx_msg void OnBnClickedAllUpdate();
	void AllUpdate();
	afx_msg void OnBnClickedAllStartServer();
	void AllStartServer();
	virtual BOOL DestroyWindow();
	afx_msg void OnClose();
	afx_msg void OnBnClickedChangeServerTime();
	afx_msg void OnBnClickedUpdateServer();
	afx_msg void OnBnClickedAllUpdateDatebase();
	void AllUpdateDatebase();
	afx_msg void OnBnClickedUpdateDatabase();
	afx_msg void OnBnClickedAllScriptExecute();
	void AllScriptExecute();
	afx_msg void OnBnClickedManualScriptExecute();

	TmServerManagerType mManagerType;

private:
	char				mUserName[32];			// 用户名
	char				mPassWord[32];			// 密码
};
