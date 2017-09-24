#pragma once
/********************************************************************
	created:	21/12/2011   18:24
	filename: 	CreateFilter.h
	author:		jeffery

	purpose:	创建过滤器对话框类
*********************************************************************/

// CCreateFilter 对话框

class CCreateFilter : public CDialog
{
	DECLARE_DYNAMIC(CCreateFilter)

public:
	CCreateFilter(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCreateFilter();

// 对话框数据
	enum { IDD = IDD_CREATEFILTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_strFilterName;

};
