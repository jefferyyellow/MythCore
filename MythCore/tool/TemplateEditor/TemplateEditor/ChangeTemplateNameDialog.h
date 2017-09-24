#pragma once


// CChangeTemplateNameDialog 对话框

class CChangeTemplateNameDialog : public CDialog
{
	DECLARE_DYNAMIC(CChangeTemplateNameDialog)

public:
	CChangeTemplateNameDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChangeTemplateNameDialog();

// 对话框数据
	enum { IDD = IDD_CHANGETEMPLATENAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_strTemplateName;
};
