
// DirtyWordDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DirtyWord.h"
#include "DirtyWordDlg.h"
#include "afxdialogex.h"
#include "SWexchange.h"
#include "CDirtyWord.h"
#include <string.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDirtyWordDlg 对话框



CDirtyWordDlg::CDirtyWordDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDirtyWordDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDirtyWordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDirtyWordDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON1, &CDirtyWordDlg::OnBnClickedButton1)
    ON_BN_CLICKED(IDOK, &CDirtyWordDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CDirtyWordDlg 消息处理程序

BOOL CDirtyWordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDirtyWordDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDirtyWordDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDirtyWordDlg::OnBnClickedButton1()
{
    // TODO:  在此添加控件通知处理程序代码
    // TODO: Add your control notification handler code here   
    // 设置过滤器   
    TCHAR szFilter[] = _T("文本文件(*.txt)|*.txt|所有文件(*.*)|*.*||");
    // 构造打开文件对话框   
    CFileDialog fileDlg(TRUE, _T("txt"), NULL, 0, szFilter, this);
    CString strFilePath;

    // 显示打开文件对话框   
    if (IDOK == fileDlg.DoModal())
    {
        // 如果点击了文件对话框上的“打开”按钮，则将选择的文件路径显示到编辑框里  
        strFilePath = fileDlg.GetPathName();
        SetDlgItemText(IDC_EDIT1, strFilePath);
    }
}


void CDirtyWordDlg::OnBnClickedOk()
{
    // TODO:  在此添加控件通知处理程序代码
    CDialogEx::OnOK();
    CString  str;
    GetDlgItemText(IDC_EDIT1, str);
    std::CDirtyWord P;
    std::string path;
    path = std::ws2s(str.GetBuffer());
    char p[256];
    strncpy(p, path.c_str(),path.size()+1);
    P.DoIt(p);
}
