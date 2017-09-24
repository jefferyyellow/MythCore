// ChildFrm.cpp : CChildFrame ���ʵ��
//
#include "stdafx.h"
#include "TemplateEditor.h"
#include "TemplateEditorView.h"
#include "ChildFrm.h"
#include "CommonDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	ON_WM_CLOSE()
	ON_COMMAND(ID_SAVETEMPLATE, &CChildFrame::OnSave)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CChildFrame ����/����

CChildFrame::CChildFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ��������ʽ
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}


// CChildFrame ���

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG


// CChildFrame ��Ϣ�������

void CChildFrame::OnClose()
{

	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(CTemplateWrite::GetSingletonPtr()->GetWritingPB())
	{
		AfxMessageBox(_T("��������ģ���У��������޸����ݣ�"));
		return;
	}

	if(!Save())
	{
		return;
	}
	CMDIChildWnd::OnClose();
}

// ��������
void CChildFrame::OnSave()
{
	Save();
}

BOOL CChildFrame::Save()
{
	CTemplateEditorView* pView = (CTemplateEditorView*)GetActiveView();

	if(pView->GetCurrTempData()->m_pParentInfo->m_emTempLoadState != emLoadState_Inited)
	{
		AfxMessageBox(_T("ģ���������δ��ʼ����ȫ,���Ժ�����"));
		return FALSE;
	}

	if (NULL != pView)
	{
		if (NULL != pView->GetCurrTempData() && pView->GetCurrTempData()->m_pParentInfo->m_bDirty)
		{
			PERFORMACE(OnSave, CTemplateManager::GetSingletonPtr()->WriteTemplateInfoToFile(pView->GetCurrTempData()->m_pParentInfo));
		}
	}

	return TRUE;
}
void CChildFrame::OnDestroy()
{
	CMDIChildWnd::OnDestroy();
	Save();
	// TODO: �ڴ˴������Ϣ����������
}
