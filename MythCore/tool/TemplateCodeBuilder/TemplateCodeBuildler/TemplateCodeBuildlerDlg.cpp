
// TemplateCodeBuildlerDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CTemplateCodeBuildlerDlg �Ի���



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


// CTemplateCodeBuildlerDlg ��Ϣ�������

BOOL CTemplateCodeBuildlerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	LoadHistoryHeadPath();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTemplateCodeBuildlerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTemplateCodeBuildlerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//��template.h�ļ���
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
			MessageBox(_T("���ɳɹ�!"));
		}
		else
		{
			MessageBox(_T("δ�ҵ�����!"));
		}
	}
}

//Unicode ת UTF-8
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