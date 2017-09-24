// TemplateEditorDoc.cpp : CTemplateEditorDoc 类的实现
//

#include "stdafx.h"
#include "TemplateEditor.h"

#include "TemplateEditorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTemplateEditorDoc

IMPLEMENT_DYNCREATE(CTemplateEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CTemplateEditorDoc, CDocument)
END_MESSAGE_MAP()


// CTemplateEditorDoc 构造/析构

CTemplateEditorDoc::CTemplateEditorDoc()
{
	// TODO: 在此添加一次性构造代码

}

CTemplateEditorDoc::~CTemplateEditorDoc()
{
}

BOOL CTemplateEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CTemplateEditorDoc 序列化

void CTemplateEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CTemplateEditorDoc 诊断

#ifdef _DEBUG
void CTemplateEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTemplateEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTemplateEditorDoc 命令
