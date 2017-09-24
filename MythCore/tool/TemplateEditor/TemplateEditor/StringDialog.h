#pragma once
/********************************************************************
	created:	21/12/2011   16:36
	filename: 	StringDialog.h
	author:		jeffery

	purpose:	字符数据编辑对话框
*********************************************************************/

// CStringDialog 对话框

class CStringDialog : public CDialog
{
	DECLARE_DYNAMIC(CStringDialog)

public:
	CStringDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStringDialog();

// 对话框数据
	enum { IDD = IDD_STRINGDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CString m_strString;
};
