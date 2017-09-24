#pragma once
/********************************************************************
	created:	21/12/2011   16:35
	filename: 	NumberDialog.h
	author:		jeffery

	purpose:	数字数据编辑对话框
*********************************************************************/

// CNumberDialog 对话框

class CNumberDialog : public CDialog
{
	DECLARE_DYNAMIC(CNumberDialog)

public:
	CNumberDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNumberDialog();

// 对话框数据
	enum { IDD = IDD_NUMBERDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_strMinMaxDescribe;
	CString m_strIntValue;
	afx_msg void OnBnClickedOk();
};
