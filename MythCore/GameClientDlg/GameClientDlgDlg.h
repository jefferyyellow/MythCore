
// GameClientDlgDlg.h : ͷ�ļ�
//

#pragma once
#include "resource.h"
class CGameClient;
// CGameClientDlgDlg �Ի���
class CGameClientDlgDlg : public CDialogEx
{
// ����
public:
	CGameClientDlgDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GAMECLIENTDLG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	

public:
	CGameClient*	mpClient;
	CString			mStrLog;
	void DisplayLog(char* pLog);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CBrush m_brush;
	afx_msg void OnBnClickedPlayerLogin();
	afx_msg void OnBnClickedSendgmcommand();
	afx_msg void OnBnClickedEquip();
	afx_msg void OnBnClickedUnequip();
	afx_msg void OnBnClickedGetPlayerProperty();
	afx_msg void OnBnClickedAcceptTask();
	afx_msg void OnBnClickedCompleteTask();
	afx_msg void OnBnClickedClearLog();
};
