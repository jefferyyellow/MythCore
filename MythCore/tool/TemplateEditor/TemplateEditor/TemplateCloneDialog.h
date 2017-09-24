#pragma once
/********************************************************************
	created:	16/12/2011   12:43
	filename: 	TemplateCloneDialog.h
	author:		jeffery

	purpose:	克隆模板数据的对话框
*********************************************************************/

// CTemplateCloneDialog 对话框

class CTemplateCloneDialog : public CDialog
{
	DECLARE_DYNAMIC(CTemplateCloneDialog)

public:
	CTemplateCloneDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTemplateCloneDialog();

// 对话框数据
	enum { IDD = IDD_TEMPLATECLONEDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 克隆后模板的名称
	CString m_strCloneName;
	// 克隆后的模板ID
	//CString m_strCloneID;
	afx_msg void OnBnClickedOk();
	// 克隆后的模板
	int m_nCloneID;
};
