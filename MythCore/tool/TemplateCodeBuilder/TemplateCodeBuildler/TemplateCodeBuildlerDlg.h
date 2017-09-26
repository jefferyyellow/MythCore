
// TemplateCodeBuildlerDlg.h : 头文件
//

#pragma once

#include "TemplateCode.h"

#define CONFIG_PATH "config.txt"
#define OUT_PATH_ROOT "Data/"
#define MAX_BUFF_LEN 1024

// CTemplateCodeBuildlerDlg 对话框
class CTemplateCodeBuildlerDlg : public CDialogEx
{
// 构造
public:
	CTemplateCodeBuildlerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TEMPLATECODEBUILDLER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	int CTemplateCodeBuildlerDlg::UnicodeToUTF8(CString & strSrc, char * pBuff, int nBuffLen);
	void ConvertCStringToString(CString & strCString, string & strStr);

	void LoadHistoryHeadPath();
	void SaveHistoryHeadPath();
private:
	CString mTemplateHeadPath;
	CString mClassName;

	CTemplateData  mTemplateData;
	CParseCodeFile mParseCodeFile;
	CBuildCodeFile mBuildCodeFile;
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();
};
