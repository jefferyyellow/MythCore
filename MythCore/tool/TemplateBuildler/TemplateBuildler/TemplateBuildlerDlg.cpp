// TemplateBuildlerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TemplateBuildler.h"
#include "TemplateBuildlerDlg.h"
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
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

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTemplateBuildlerDlg 对话框




CTemplateBuildlerDlg::CTemplateBuildlerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTemplateBuildlerDlg::IDD, pParent)
	, m_strHeadFilePath(_T(""))
	, m_strTemplatePath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTemplateBuildlerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_HEADPATH, m_strHeadFilePath);
	DDX_Text(pDX, IDC_TEMPLATEPATH, m_strTemplatePath);
}

BEGIN_MESSAGE_MAP(CTemplateBuildlerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHANGEHEADPATH, &CTemplateBuildlerDlg::OnBnClickedChangeheadpath)
	ON_BN_CLICKED(IDC_CHANGETEMPPATH, &CTemplateBuildlerDlg::OnBnClickedChangetemppath)
	ON_BN_CLICKED(IDC_GENERATETEMPLATEFILE, &CTemplateBuildlerDlg::OnBnClickedGeneratetemplatefile)
END_MESSAGE_MAP()


// CTemplateBuildlerDlg 消息处理程序

BOOL CTemplateBuildlerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	TCHAR acCurrentDir[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, acCurrentDir);

	// 从配置文件读取路径
	m_strConfigPath = acCurrentDir;
	m_strConfigPath.Append(L"\\config.txt");

	// 配置文件格式 header_path:“path\path"
	fstream ifst;
	ifst.open(m_strConfigPath, ios::in | ios::out);
	if (ifst.is_open())
	{
		char strbuf[MAX_PATH] = { 0 };
		string strline;
		int bpos = 0;
		int epos = 0;
		while (ifst.getline(strbuf, MAX_PATH))
		{
			strline = strbuf;

			if (strline.find("header_path:") != string::npos)
			{
				bpos = strline.find(":\"");
				epos = strline.find_last_of("\"");
				if (epos > bpos + 1)
				{
					m_strHeadFilePath = strline.substr(bpos += 2, epos - bpos).c_str();
				}
				else
				{
					m_strHeadFilePath = strline.substr(bpos += 2).c_str();
				}
			}
		}
	}

	ifst.close();
	m_strTemplatePath = acCurrentDir;

	UpdateData(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTemplateBuildlerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTemplateBuildlerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CTemplateBuildlerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTemplateBuildlerDlg::OnBnClickedChangeheadpath()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog filedialog(TRUE, NULL, m_strHeadFilePath);

	if(IDOK == filedialog.DoModal())
	{
		// 转换成债字符，蛋碎的东西
		m_strHeadFilePath = filedialog.GetPathName();
		wstring strTemp = m_strHeadFilePath;
		strTemp.insert(0, L"header_path:\"");
		string res = m_WriteFile.ws2s(strTemp);
		//string res(strTemp.begin(), strTemp.end());
		res.append("\"");

		// 写入文件
		fstream fs;
		fs.open(m_strConfigPath, ios::trunc | ios::out | ios::in);
		fs << res;
		fs.close();
		
		UpdateData(FALSE);
	}

}

void CTemplateBuildlerDlg::OnBnClickedChangetemppath()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR szPath[MAX_PATH];     //存放选择的目录路径 
	CString str;

	ZeroMemory(szPath, sizeof(szPath));   


	BROWSEINFO bi;   
	bi.hwndOwner = m_hWnd;   
	bi.pidlRoot = NULL;   
	bi.pszDisplayName = NULL;
	bi.lpszTitle = _T("请选择需要打包的目录：");   
	bi.lpfn = NULL;   
	bi.lParam = 0;   
	bi.iImage = 0;   
	bi.ulFlags = BIF_RETURNONLYFSDIRS|BIF_USENEWUI;

	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);   

	if(lp && SHGetPathFromIDList(lp, szPath))   
	{
		m_strTemplatePath = szPath;
		UpdateData(FALSE);
	}


}

void CTemplateBuildlerDlg::OnBnClickedGeneratetemplatefile()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bParseResult = m_strParseFile.BeginParseFile(m_strHeadFilePath.GetBuffer());
	if (bParseResult)
	{
		m_WriteFile.BeginWriteFile(m_strParseFile, m_strTemplatePath);
	}
}
