#pragma once


// CSearchTemplateDialog 对话框

class CSearchTemplateDialog : public CDialog
{
	DECLARE_DYNAMIC(CSearchTemplateDialog)

public:
	CSearchTemplateDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSearchTemplateDialog();

// 对话框数据
	enum { IDD = IDD_SEARCHTEMPLATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	int m_nSearchID;
};
