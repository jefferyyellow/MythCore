
// GameClientDlgDlg.h : 头文件
//

#pragma once
#include "resource.h"
class CGameClient;
// CGameClientDlgDlg 对话框
class CGameClientDlgDlg : public CDialogEx
{
// 构造
public:
	CGameClientDlgDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_GAMECLIENTDLG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


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
