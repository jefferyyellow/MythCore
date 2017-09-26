
// TemplateCodeBuildlerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TemplateCodeBuildler.h"
#include "TemplateCodeBuildlerDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include <sstream> 
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTemplateCodeBuildlerDlg 对话框



CTemplateCodeBuildlerDlg::CTemplateCodeBuildlerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTemplateCodeBuildlerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTemplateCodeBuildlerDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Text(pDX, IDC_EDIT1, mTemplateHeadPath);
	DDX_Text(pDX, IDC_EDIT2, mClassName);
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTemplateCodeBuildlerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CTemplateCodeBuildlerDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CTemplateCodeBuildlerDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CTemplateCodeBuildlerDlg 消息处理程序

BOOL CTemplateCodeBuildlerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	LoadHistoryHeadPath();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTemplateCodeBuildlerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CTemplateCodeBuildlerDlg::LoadHistoryHeadPath()
{
	ifstream InFile;
	InFile.open(CONFIG_PATH);
	if (!InFile.is_open())
	{
		return;
	}

	char acBuffer[emLine_Max_Char] = { 0 };
	InFile.getline(acBuffer, emLine_Max_Char);
	mTemplateHeadPath = acBuffer;
	UpdateData(false);
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTemplateCodeBuildlerDlg::OnPaint()
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
HCURSOR CTemplateCodeBuildlerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//打开template.h文件夹
void CTemplateCodeBuildlerDlg::OnBnClickedButton1()
{
	CString strFilePath = _T("");

	CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Describe All Files (*.*)|*.*||"), NULL);

	if (dlgFile.DoModal())
	{
		strFilePath = dlgFile.GetPathName();
	}

	if (strFilePath.IsEmpty())
	{
		return;
	}
	
	mTemplateHeadPath = strFilePath;

	UpdateData(false);

	SaveHistoryHeadPath();
}

void CTemplateCodeBuildlerDlg::SaveHistoryHeadPath()
{
	ofstream out;
	out.open(CONFIG_PATH, ios::trunc | ios::out);
	if (!out)
	{
		return;
	}
	string strHeadPath;
	ConvertCStringToString(mTemplateHeadPath, strHeadPath);
	out << strHeadPath;
}

void CTemplateCodeBuildlerDlg::OnBnClickedOk()
{
	UpdateData(true);
	
	std::string strHeadPath;
	ConvertCStringToString(mTemplateHeadPath, strHeadPath);
	std::string strClassName;
	ConvertCStringToString(mClassName, strClassName);
	if (mParseCodeFile.BeginParseFile(strHeadPath, mTemplateData))
	{
		if (mBuildCodeFile.BeginBuildCode("data/", strClassName, mTemplateData))
		{
			MessageBox(_T("生成成功!"));
		}
		else
		{
			MessageBox(_T("未找到该类!"));
		}
	}
}

//Unicode 转 UTF-8
int CTemplateCodeBuildlerDlg::UnicodeToUTF8(CString & strSrc, char * pBuff, int nBuffLen)
{
	if (pBuff == NULL || nBuffLen <= 0)
	{
		return -1;
	}
	int nLen = WideCharToMultiByte(CP_ACP, 0, strSrc, strSrc.GetLength(), NULL, 0, NULL, NULL);

	if (nLen < 0 || nLen + 1 > nBuffLen)
	{
		return -1;
	}

	WideCharToMultiByte(CP_ACP, 0, strSrc, strSrc.GetLength(), pBuff, nLen, NULL, NULL);

	pBuff[nLen + 1] = '\0';

	return 0;
}

void CTemplateCodeBuildlerDlg::ConvertCStringToString(CString & strCString, string & strStr)
{
	char pContext[MAX_BUFF_LEN] = { 0 };
	UnicodeToUTF8(strCString, pContext, MAX_BUFF_LEN);
	strStr = pContext;
}