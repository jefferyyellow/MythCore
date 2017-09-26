
// TemplateCodeBuildlerDlg.h : ͷ�ļ�
//

#pragma once

#include "TemplateCode.h"

#define CONFIG_PATH "config.txt"
#define OUT_PATH_ROOT "Data/"
#define MAX_BUFF_LEN 1024

// CTemplateCodeBuildlerDlg �Ի���
class CTemplateCodeBuildlerDlg : public CDialogEx
{
// ����
public:
	CTemplateCodeBuildlerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TEMPLATECODEBUILDLER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

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
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();
};
