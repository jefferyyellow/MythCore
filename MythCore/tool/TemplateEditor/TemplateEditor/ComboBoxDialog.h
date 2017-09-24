#pragma once
#include "afxwin.h"
/********************************************************************
	created:	21/12/2011   16:34
	filename: 	ComboBoxDialog.h
	author:		jeffery

	purpose:	多选一数据编辑对话框
*********************************************************************/

// CComboBoxDialog 对话框

class CComboBoxDialog : public CDialog
{
	DECLARE_DYNAMIC(CComboBoxDialog)

public:
	CComboBoxDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CComboBoxDialog();

// 对话框数据
	enum { IDD = IDD_COMBOBOXDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_SelectComboBox;
	CString m_SelectDescribe;

	vector<DWORD>	m_vecComboBoxData;
	afx_msg void OnBnClickedOk();
};
