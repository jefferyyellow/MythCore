// ChildFrm.cpp : CChildFrame 类的实现
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


// CChildFrame 构造/析构

CChildFrame::CChildFrame()
{
	// TODO: 在此添加成员初始化代码
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或样式
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}


// CChildFrame 诊断

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


// CChildFrame 消息处理程序

void CChildFrame::OnClose()
{

	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(CTemplateWrite::GetSingletonPtr()->GetWritingPB())
	{
		AfxMessageBox(_T("正在生成模板中，不允许修改数据！"));
		return;
	}

	if(!Save())
	{
		return;
	}
	CMDIChildWnd::OnClose();
}

// 保存数据
void CChildFrame::OnSave()
{
	Save();
}

BOOL CChildFrame::Save()
{
	CTemplateEditorView* pView = (CTemplateEditorView*)GetActiveView();

	if(pView->GetCurrTempData()->m_pParentInfo->m_emTempLoadState != emLoadState_Inited)
	{
		AfxMessageBox(_T("模板表数据尚未初始化完全,请稍后再试"));
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
	// TODO: 在此处添加消息处理程序代码
}
