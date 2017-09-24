// TemplateBuildlerDlg.h : ͷ�ļ�
//

#pragma once
#include "ParseHeadFile.h"
#include "WriteFile.h"

// CTemplateBuildlerDlg �Ի���
class CTemplateBuildlerDlg : public CDialog
{
// ����
public:
	CTemplateBuildlerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TEMPLATEBUILDLER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


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
	// �޸�ͷ�ļ�·��
	afx_msg void OnBnClickedChangeheadpath();
	// �޸�ģ���ļ�·��
	afx_msg void OnBnClickedChangetemppath();
	// ����ģ���ļ�
	afx_msg void OnBnClickedGeneratetemplatefile();

public:
	CString&		GetTemplatePath(){return m_strTemplatePath;}

private:
	// ͷ�ļ�·��
	CString			m_strHeadFilePath;
	// ���ɵ�ģ���ļ�·��
	CString			m_strTemplatePath;
	// �����ļ�·��
	CString			m_strConfigPath;
	// ��Ҫ�ķ���ͷ�ļ�����
	CParseHeadFile	m_strParseFile;
	// дԭʼģ���ļ�����
	CWriteFile		m_WriteFile;
};
