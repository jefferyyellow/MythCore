
// GameClientDlgDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GameClientDlg.h"
#include "GameClientDlgDlg.h"
#include "afxdialogex.h"
#include "i18n.h"
#include "./gameclient.h"

using namespace Myth;
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


// CGameClientDlgDlg �Ի���



CGameClientDlgDlg::CGameClientDlgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGameClientDlgDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGameClientDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGameClientDlgDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_PLAYER_LOGIN, &CGameClientDlgDlg::OnBnClickedPlayerLogin)
	ON_BN_CLICKED(IDC_SENDGMCOMMAND, &CGameClientDlgDlg::OnBnClickedSendgmcommand)
END_MESSAGE_MAP()


// CGameClientDlgDlg ��Ϣ�������

BOOL CGameClientDlgDlg::OnInitDialog()
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
	mpClient = new CGameClient();
	mpClient->init();
	mpClient->setDlg(this);
	SetTimer(GAME_MAIN_TIMER, 30, NULL);
	((CEdit*)GetDlgItem(IDC_LOG))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_LOG))->SetWindowText(_T("�Ҳ���"));
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CGameClientDlgDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CGameClientDlgDlg::OnPaint()
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
HCURSOR CGameClientDlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGameClientDlgDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == GAME_MAIN_TIMER)
	{
		mpClient->run();
	}
	CDialogEx::OnTimer(nIDEvent);
}


HBRUSH CGameClientDlgDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd->GetDlgCtrlID() == IDC_LOG)
	{
		pDC->SetTextColor(RGB(255, 0, 0));//���ñ༭���������ɫ
		//pDC->SetBkColor(RGB(255, 255, 0));//�������屳����ɫ
		//CFont font;
		////font.CreatePointFont(100, "���Ŀ���");
		//pDC->SelectObject(&font);//��������        
		return m_brush;
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CGameClientDlgDlg::OnBnClickedPlayerLogin()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strAccName;
	((CEdit*)GetDlgItem(IDC_PLAYER_ACCOUNT))->GetWindowText(strAccName);
	CString stRoleName;
	((CEdit*)GetDlgItem(IDC_ROLE_NAME))->GetWindowText(stRoleName);

	mpClient->LoginServer(strAccName.GetBuffer(), stRoleName.GetBuffer());
}


void CGameClientDlgDlg::OnBnClickedSendgmcommand()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strCommand;
	((CEdit*)GetDlgItem(IDC_GM_COMMAND))->GetWindowText(strCommand);

	mpClient->sendGMCommandRequest(strCommand.GetBuffer());
}

void CGameClientDlgDlg::DisplayLog(char* pLog)
{
	if (NULL == pLog)
	{
		return;
	}
	if (pLog[0] == '\0')
	{
		return;
	}

	mStrLog += pLog;
	mStrLog += "\r\n";
	((CEdit*)GetDlgItem(IDC_LOG))->SetWindowText(mStrLog);
	UpdateData(FALSE);
}