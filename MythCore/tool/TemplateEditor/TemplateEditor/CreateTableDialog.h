#pragma once
#include "afxwin.h"
/********************************************************************
	created:	21/12/2011   18:28
	filename: 	CreateTableDialog.h
	author:		jeffery

	purpose:	创建模板表对话框
*********************************************************************/

// CCreateTableDialog 对话框

class CCreateTableDialog : public CDialog
{
	DECLARE_DYNAMIC(CCreateTableDialog)

public:
	CCreateTableDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCreateTableDialog();

// 对话框数据
	enum { IDD = IDD_CREATETABLEDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 模板表的名称
	CString m_strTableName;
	afx_msg void OnBnClickedCreatetable();
	virtual BOOL OnInitDialog();
};
