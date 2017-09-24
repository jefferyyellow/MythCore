#pragma once
#include "afxwin.h"


// CChangeTableInfo 对话框

class CChangeTableInfo : public CDialog
{
	DECLARE_DYNAMIC(CChangeTableInfo)

public:
	CChangeTableInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChangeTableInfo();

// 对话框数据
	enum { IDD = IDD_CHANGETABLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 模板表名字
	CString m_strTableName;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
