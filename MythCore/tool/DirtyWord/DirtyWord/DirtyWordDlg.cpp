
// DirtyWordDlg.cpp : ʵ���ļ�
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


// CDirtyWordDlg �Ի���



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


// CDirtyWordDlg ��Ϣ�������

BOOL CDirtyWordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDirtyWordDlg::OnPaint()
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
HCURSOR CDirtyWordDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDirtyWordDlg::OnBnClickedButton1()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    // TODO: Add your control notification handler code here   
    // ���ù�����   
    TCHAR szFilter[] = _T("�ı��ļ�(*.txt)|*.txt|�����ļ�(*.*)|*.*||");
    // ������ļ��Ի���   
    CFileDialog fileDlg(TRUE, _T("txt"), NULL, 0, szFilter, this);
    CString strFilePath;

    // ��ʾ���ļ��Ի���   
    if (IDOK == fileDlg.DoModal())
    {
        // ���������ļ��Ի����ϵġ��򿪡���ť����ѡ����ļ�·����ʾ���༭����  
        strFilePath = fileDlg.GetPathName();
        SetDlgItemText(IDC_EDIT1, strFilePath);
    }
}


void CDirtyWordDlg::OnBnClickedOk()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
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
