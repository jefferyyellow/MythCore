// TemplateBuildlerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TemplateBuildler.h"
#include "TemplateBuildlerDlg.h"
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
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

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTemplateBuildlerDlg �Ի���




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


// CTemplateBuildlerDlg ��Ϣ�������

BOOL CTemplateBuildlerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	TCHAR acCurrentDir[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, acCurrentDir);

	// �������ļ���ȡ·��
	m_strConfigPath = acCurrentDir;
	m_strConfigPath.Append(L"\\config.txt");

	// �����ļ���ʽ header_path:��path\path"
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
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTemplateBuildlerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
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
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CTemplateBuildlerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTemplateBuildlerDlg::OnBnClickedChangeheadpath()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog filedialog(TRUE, NULL, m_strHeadFilePath);

	if(IDOK == filedialog.DoModal())
	{
		// ת����ծ�ַ�������Ķ���
		m_strHeadFilePath = filedialog.GetPathName();
		wstring strTemp = m_strHeadFilePath;
		strTemp.insert(0, L"header_path:\"");
		string res = m_WriteFile.ws2s(strTemp);
		//string res(strTemp.begin(), strTemp.end());
		res.append("\"");

		// д���ļ�
		fstream fs;
		fs.open(m_strConfigPath, ios::trunc | ios::out | ios::in);
		fs << res;
		fs.close();
		
		UpdateData(FALSE);
	}

}

void CTemplateBuildlerDlg::OnBnClickedChangetemppath()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TCHAR szPath[MAX_PATH];     //���ѡ���Ŀ¼·�� 
	CString str;

	ZeroMemory(szPath, sizeof(szPath));   


	BROWSEINFO bi;   
	bi.hwndOwner = m_hWnd;   
	bi.pidlRoot = NULL;   
	bi.pszDisplayName = NULL;
	bi.lpszTitle = _T("��ѡ����Ҫ�����Ŀ¼��");   
	bi.lpfn = NULL;   
	bi.lParam = 0;   
	bi.iImage = 0;   
	bi.ulFlags = BIF_RETURNONLYFSDIRS|BIF_USENEWUI;

	//����ѡ��Ŀ¼�Ի���
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);   

	if(lp && SHGetPathFromIDList(lp, szPath))   
	{
		m_strTemplatePath = szPath;
		UpdateData(FALSE);
	}


}

void CTemplateBuildlerDlg::OnBnClickedGeneratetemplatefile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bParseResult = m_strParseFile.BeginParseFile(m_strHeadFilePath.GetBuffer());
	if (bParseResult)
	{
		m_WriteFile.BeginWriteFile(m_strParseFile, m_strTemplatePath);
	}
}
