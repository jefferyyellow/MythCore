// TemplateBuildlerDlg.h : 头文件
//

#pragma once
#include "ParseHeadFile.h"
#include "WriteFile.h"

// CTemplateBuildlerDlg 对话框
class CTemplateBuildlerDlg : public CDialog
{
// 构造
public:
	CTemplateBuildlerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TEMPLATEBUILDLER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


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
	// 修改头文件路径
	afx_msg void OnBnClickedChangeheadpath();
	// 修改模板文件路径
	afx_msg void OnBnClickedChangetemppath();
	// 生成模板文件
	afx_msg void OnBnClickedGeneratetemplatefile();

public:
	CString&		GetTemplatePath(){return m_strTemplatePath;}

private:
	// 头文件路径
	CString			m_strHeadFilePath;
	// 生成的模板文件路径
	CString			m_strTemplatePath;
	// 配置文件路径
	CString			m_strConfigPath;
	// 主要的分配头文件的类
	CParseHeadFile	m_strParseFile;
	// 写原始模板文件的类
	CWriteFile		m_WriteFile;
};
