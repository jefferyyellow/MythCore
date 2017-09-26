// RestartServerDlg.h : ͷ�ļ�
//

#pragma once
#include <WinSock2.h>
#include "message.h"
#include "afxdtctl.h"
#include "afxwin.h"


#define MAX_BUFF_SIZE 8 * 1024

// CRestartServerDlg �Ի���
class CRestartServerDlg : public CDialog
{
// ����
public:
	CRestartServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_RESTARTSERVER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
	bool flag;

// ʵ��
protected:
	HICON m_hIcon;
	// ����ͼ��
	NOTIFYICONDATA mNotifyIconData;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	SOCKET GetSocket(){return mSockfd;}

private:
	SOCKET mSockfd;
	char	mBuffer[MAX_BUFF_SIZE];
	int		mBuffLength;
public:
	afx_msg LRESULT OnAddTrayIcon(WPARAM wParam,LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedGetServerDateTime();
	afx_msg void OnBnClickedSetServerTime();
	int mEditHour;
	int mEditMinute;
	int mEditSecond;
	CDateTimeCtrl mDateTimeCtrl;
	time_t mCurrentServerTime;
	CString mStaticServerTime;
	CStatic mServerTimeControl;
	bool	mSelectServer[MAX_SERVER_NUM];

	// �Ƿ��Ѿ���ȡ��������
	bool mbGetServerName;
	// �Ƿ��Ѿ��Ͽ�����
	bool mbDisConnect;

	// ��������
	int mCurrRestartNum;
public:
	void OnMessage(CMessage* pMessage);
	void SendMsgBoxFailed();

public:
	afx_msg void OnBnClickedRestartServer0();
	afx_msg void OnBnClickedStopServer0();

private:
	void OnClickedRestartServer();
	void OnClickedStopServer();
	void SendGetServerNameRequest();
	void OnMsgGetServerNameResponse(CMessage* pMessage);
	void OnMsgServerRestartResponse(CMessage * pMessage);
public:
	afx_msg void OnBnClickedRestartServer2();
	afx_msg void OnBnClickedRestartServer3();
	afx_msg void OnBnClickedStopServer2();
	afx_msg void OnBnClickedStopServer3();
	afx_msg void OnBnClickedSelectServer0();
	afx_msg void OnBnClickedSelectServer1();
	afx_msg void OnBnClickedSelectServer2();
	afx_msg void OnBnClickedSelectServer3();

	void UpdateSelectServerItemText();
	void UpdateSelectServerItemText( const CString & Str);
	afx_msg void OnBnClickedSelectServer4();
	afx_msg void OnBnClickedSelectServer5();

	afx_msg void OnBnClickedSelectServer6();
	afx_msg void OnBnClickedSelectServer7();
	afx_msg void OnBnClickedSelectServer8();
	afx_msg void OnBnClickedSelectServer9();
	afx_msg void OnBnClickedSelectServer10();
	afx_msg void OnBnClickedSelectServer11();
};
