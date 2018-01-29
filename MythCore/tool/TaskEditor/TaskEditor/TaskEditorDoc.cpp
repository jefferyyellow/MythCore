
// TaskEditorDoc.cpp : CTaskEditorDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "TaskEditor.h"
#endif

#include "TaskEditorDoc.h"
#include "MainFrm.h"
#include "TaskEditorView.h"
#include "commondefine.h"
#include <propkey.h>
#include "GridCellCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTaskEditorDoc

IMPLEMENT_DYNCREATE(CTaskEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CTaskEditorDoc, CDocument)
END_MESSAGE_MAP()


// CTaskEditorDoc 构造/析构

CTaskEditorDoc::CTaskEditorDoc()
{
	// TODO:  在此添加一次性构造代码
}

CTaskEditorDoc::~CTaskEditorDoc()
{
}

BOOL CTaskEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO:  在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CTaskEditorDoc 序列化
//
//void CTaskEditorDoc::Serialize(CArchive& ar)
//{
//	if (ar.IsStoring())
//	{
//
//	}
//	else
//	{
//		// TODO:  在此添加加载代码
//	}
//}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CTaskEditorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CTaskEditorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CTaskEditorDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CTaskEditorDoc 诊断

#ifdef _DEBUG
void CTaskEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTaskEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTaskEditorDoc 命令


BOOL CTaskEditorDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO:  在此添加专用代码和/或调用基类
	char acBuffer[4096] = { 0 };
	tinyxml2::XMLDocument tDocument;

	XMLDeclaration* pDeclaration = tDocument.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
	tDocument.LinkEndChild(pDeclaration);

	XMLElement* pRootElem = tDocument.NewElement("TaskData");
	tDocument.LinkEndChild(pRootElem);

	POSITION pos = GetFirstViewPosition();
	CTaskEditorView* pView = (CTaskEditorView*)GetNextView(pos);
	//CTaskEditorView* pView = GetView (CTaskEditorView*)((CMainFrame*)AfxGetMainWnd())->MDIGetActive()->GetActiveView();
	// TODO:  在此添加存储代码
	CGridCtrl* pManGrid = pView->mMainGrid;
	for (int i = 0; i < pManGrid->GetRowCount(); ++i)
	{
		for (int j = 0; j < pManGrid->GetColumnCount(); j += 2)
		{
			CGridCellBase* pCellBase = pManGrid->GetCell(i, j);
			if (NULL == pCellBase)
			{
				continue;
			}

			CGridData* pGridData = (CGridData*)pCellBase->GetData();
			if (NULL == pGridData)
			{
				continue;
			}

			CTaskMainNode* pMainNode = (CTaskMainNode*)pGridData->mData;
			if (NULL == pMainNode)
			{
				continue;
			}

			CString strValue = pManGrid->GetItemText(i, j + 1);

			UnicodeToAnsi(pMainNode->mDesName.c_str(), acBuffer, sizeof(acBuffer));
			XMLElement* pMainElem = tDocument.NewElement(acBuffer);

			if (pMainNode->mOptionList.size() > 0)
			{
				strValue = strValue.Left(strValue.Find(_T(",")));
			}

			UnicodeToAnsi(strValue.GetBuffer(), acBuffer, sizeof(acBuffer));
			pMainElem->SetAttribute("Value", acBuffer);
			pRootElem->LinkEndChild(pMainElem);
		}
	}


	CGridCtrl* pCondGrid = pView->mCondGrid;
	XMLElement* pCondElem = NULL;
	for (int i = 0; i < pCondGrid->GetRowCount(); ++ i)
	{
		CGridCellBase* pCellBase = pCondGrid->GetCell(i, 0);
		if (NULL == pCellBase)
		{
			continue;
		}

		CGridData* pGridData = (CGridData*)pCellBase->GetData();
		if (NULL == pGridData)
		{
			continue;
		}

		CTaskMainNode* pMainNode = (CTaskMainNode*)pGridData->mData;
		if (NULL == pMainNode)
		{
			continue;
		}

		if (pGridData->mDataType == emDataType_CondMainNode)
		{
			UnicodeToAnsi(pMainNode->mDesName.c_str(), acBuffer, sizeof(acBuffer));
			XMLElement* pMainElem = tDocument.NewElement(acBuffer);
			pRootElem->LinkEndChild(pMainElem);
			pCondElem = pMainElem;
		}
		else if (pGridData->mDataType == emDataType_CondDataNode)
		{
			CString strValue = pCellBase->GetText();
			strValue = strValue.Left(strValue.Find(_T(",")));

			XMLElement* pMainElem = tDocument.NewElement("Cond");
			UnicodeToAnsi(strValue.GetBuffer(), acBuffer, sizeof(acBuffer));
			pMainElem->SetAttribute("Type", acBuffer);

			for (int j = 0; j < 4; ++ j)
			{
				CGridCellBase* pParamCell = pCondGrid->GetCell(i, j + 1);
				if (NULL == pParamCell)
				{
					continue;
				}

				CString strParam = pParamCell->GetText();
				if (pParamCell->IsKindOf(RUNTIME_CLASS(CGridCellCombo)))
				{
					strParam = strParam.Left(strParam.Find(_T(",")));
				}
				if (strParam == _T(""))
				{
					continue;
				}

				char acAttributeName[MAX_PATH] = {0};
				_snprintf_s(acAttributeName, sizeof(acAttributeName), "Para%d", j);
				UnicodeToAnsi(strParam.GetBuffer(), acBuffer, sizeof(acBuffer));
				pMainElem->SetAttribute(acAttributeName, acBuffer);
			}
			pCondElem->LinkEndChild(pMainElem);
		}
	}


	UnicodeToAnsi(lpszPathName, acBuffer, sizeof(acBuffer));
	tDocument.SaveFile(acBuffer);
	SetModifiedFlag(FALSE);
	return TRUE;
}


BOOL CTaskEditorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;


	mOpenFilePath = lpszPathName;
	// TODO:  在此添加您专用的创建代码
	char acBuffer[4096] = { 0 };

	tinyxml2::XMLDocument tDocument;
	UnicodeToAnsi(lpszPathName, acBuffer, sizeof(acBuffer));
	if (tinyxml2::XML_SUCCESS != tDocument.LoadFile(acBuffer))
	{
		return FALSE;
	}
	return TRUE;
}

void CTaskEditorDoc::OpenDocument()
{
	if (mOpenFilePath == _T(""))
	{
		return;
	}

	tinyxml2::XMLDocument tDocument;
	char acBuffer[4096] = { 0 };
	wchar_t wBuffer[4096] = {0};
	UnicodeToAnsi(mOpenFilePath, acBuffer, sizeof(acBuffer));
	if (tinyxml2::XML_SUCCESS != tDocument.LoadFile(acBuffer))
	{
		return;
	}

	XMLElement* pRootElem = tDocument.RootElement();
	if (NULL == pRootElem)
	{
		return;
	}

	POSITION pos = GetFirstViewPosition();
	CTaskEditorView* pView = (CTaskEditorView*)GetNextView(pos);
	if (NULL == pView)
	{
		return;
	}
	CGridCtrl* pManGrid = pView->mMainGrid;
	for (int i = 0; i < pManGrid->GetRowCount(); ++i)
	{
		for (int j = 0; j < pManGrid->GetColumnCount(); j += 2)
		{
			CGridCellBase* pCellBase = pManGrid->GetCell(i, j);
			if (NULL == pCellBase)
			{
				continue;
			}

			CGridData* pGridData = (CGridData*)pCellBase->GetData();
			if (NULL == pGridData)
			{
				continue;
			}

			CTaskMainNode* pMainNode = (CTaskMainNode*)pGridData->mData;
			if (NULL == pMainNode)
			{
				continue;
			}

			UnicodeToAnsi(pMainNode->mDesName.c_str(), acBuffer, sizeof(acBuffer));
			XMLElement* pMainElem = pRootElem->FirstChildElement(acBuffer);
			if (NULL == pMainElem)
			{
				continue;
			}
			
			char acValue[MAX_PATH] = {0};
			pMainElem->Attribute("Value", acValue);
			AnsiToUnicode(acValue, wBuffer, sizeof(wBuffer) / 2 - 1);
			pManGrid->SetItemText(i, j + 1, wBuffer);
		}
	}


	pManGrid->Invalidate();
}