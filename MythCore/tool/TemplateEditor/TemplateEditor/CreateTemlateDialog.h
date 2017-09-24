#pragma once
/********************************************************************
	created:	21/12/2011   16:31
	filename: 	CreateTemlateDialog.h
	author:		jeffery

	purpose:	创建模板的弹出对话框类
*********************************************************************/

// CCreateTemlateDialog 对话框

class CCreateTemlateDialog : public CDialog
{
	DECLARE_DYNAMIC(CCreateTemlateDialog)

public:
	CCreateTemlateDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCreateTemlateDialog();

// 对话框数据
	enum { IDD = IDD_CREATETEMPLATEDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 模板名称
	CString m_strTemplateName;
	// 模板ID
	int m_nTemplateID;
	afx_msg void OnBnClickedCreatetemplate();
};
