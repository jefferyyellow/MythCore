// TemplateEditorDoc.cpp : CTemplateEditorDoc ���ʵ��
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


// CTemplateEditorDoc ����/����

CTemplateEditorDoc::CTemplateEditorDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CTemplateEditorDoc::~CTemplateEditorDoc()
{
}

BOOL CTemplateEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CTemplateEditorDoc ���л�

void CTemplateEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CTemplateEditorDoc ���

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


// CTemplateEditorDoc ����
