#pragma once
/********************************************************************
	created:	21/12/2011   16:35
	filename: 	MultiCheckDialog.h
	author:		jeffery

	purpose:	多选多数据编辑对话框
*********************************************************************/

// CMultiCheckDialog 对话框

class CMultiCheckDialog : public CDialog
{
	DECLARE_DYNAMIC(CMultiCheckDialog)

public:
	CMultiCheckDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMultiCheckDialog();

// 对话框数据
	enum { IDD = IDD_MULTICHECKDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
