
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
	SetTitle(_T("*.xml"));
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
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (NULL == pMainFrame)
	{
		return FALSE;
	}
	POSITION pos = GetFirstViewPosition();
	CTaskEditorView* pView = (CTaskEditorView*)GetNextView(pos);
	if (NULL == pView)
	{
		return FALSE;
	}


	CString strNewFilePaht = lpszPathName;
	if (lpszPathName[0] == _T('\0'))
	{
		AfxMessageBox(_T("保存失败！注意命名规则，任务文件必须以数字为文件名,比如1.xml"), MB_ICONEXCLAMATION);
		return FALSE;
	}

	CString strFileName = lpszPathName;
	strFileName = strFileName.Right(strFileName.GetLength() - strFileName.ReverseFind('\\') - 1);
	strFileName = strFileName.Left(strFileName.Find('.'));

	// 新的文件名的检查
	if (_T("") == m_strPathName && lpszPathName[0] != _T('\0'))
	{

		if (strFileName != pView->GetMainGridText(_T("TaskId")))
		{
			AfxMessageBox(_T("保存失败！任务ID和文件名不匹配!"), MB_ICONEXCLAMATION);
			return FALSE;
		}

		CString str = pView->GetMainGridText(_T("TaskType"));
		if (pView->GetMainGridText(_T("TaskType")) == _T(""))
		{
			AfxMessageBox(_T("保存失败！任务类型为空!"), MB_ICONEXCLAMATION);
			return FALSE;
		}

		if (pView->GetMainGridText(_T("TaskName")) == _T(""))
		{
			AfxMessageBox(_T("保存失败！任务名字为空!"), MB_ICONEXCLAMATION);
			return FALSE;
		}
		for (int i = 0; i < strFileName.GetLength(); ++ i)
		{
			if (strFileName[i] < _T('0') || strFileName[i] > _T('9'))
			{
				AfxMessageBox(_T("保存失败！注意命名规则，任务文件必须以数字为文件名,比如1.xml"), MB_ICONEXCLAMATION);
				return FALSE;
			}
		}
	}

	char acBuffer[4096] = { 0 };
	tinyxml2::XMLDocument tDocument;
	// 保持到XML文件
	SaveToXml(tDocument);
	UnicodeToAnsi(lpszPathName, acBuffer, sizeof(acBuffer));
	tDocument.SaveFile(acBuffer);
	// 将修改标志设置为FALSE
	SetModifiedFlag(FALSE);

	if (_T("") == m_strPathName)
	{
		pMainFrame->AddFileItem(tDocument, strFileName);

		CGridCtrl* pManGrid = pView->mMainGrid;
		pManGrid->SetItemState(0, 1, pManGrid->GetItemState(0, 1) | GVIS_READONLY);
	}

	CString strTaskID = pView->GetMainGridText(_T("TaskId"));
	CString strTaskType = pView->GetMainGridText(_T("TaskType"));
	strTaskType = strTaskType.Right(strTaskType.GetLength() - strTaskType.Find(',') - 1);
	CString strTaskName = pView->GetMainGridText(_T("TaskName"));
	/// 更新文件树视图的节点
	pMainFrame->UpdateFileViewItem(strTaskID, strTaskType, strTaskName);
	return TRUE;
}

/// 将任务保存到xml文件中去
void CTaskEditorDoc::SaveToXml(tinyxml2::XMLDocument& tDocument)
{
	// TODO:  在此添加专用代码和/或调用基类
	char acBuffer[4096] = { 0 };

	XMLDeclaration* pDeclaration = tDocument.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"");
	tDocument.LinkEndChild(pDeclaration);

	XMLElement* pRootElem = tDocument.NewElement("TaskData");
	tDocument.LinkEndChild(pRootElem);

	POSITION pos = GetFirstViewPosition();
	CTaskEditorView* pView = (CTaskEditorView*)GetNextView(pos);

	// 主节点
	CGridCtrl* pManGrid = pView->mMainGrid;
	for (int i = 0; i < pManGrid->GetRowCount(); ++i)
	{
		for (int j = 0; j < pManGrid->GetColumnCount(); j += 2)
		{
			SaveMainNode(pManGrid, i, j, tDocument, pRootElem, true);
		}
	}

	// 条件节点
	CGridCtrl* pCondGrid = pView->mCondGrid;
	XMLElement* pCondElem = NULL;
	for (int i = 0; i < pCondGrid->GetRowCount(); ++i)
	{
		XMLElement* pTmpElem = SaveMainNode(pCondGrid, i, 0, tDocument, pRootElem, false);
		if (NULL != pTmpElem)
		{
			pCondElem = pTmpElem;
		}
		else
		{
			SaveDataNode(pCondGrid, i, 0, tDocument, pCondElem, MAX_COND_PARAM_NUM);
		}
	}

	// 对话节点
	CGridCtrl* pDiagGrid = pView->mDiagGrid;;
	pCondElem = NULL;
	for (int i = 0; i < pDiagGrid->GetRowCount(); ++i)
	{
		XMLElement* pTmpElem = SaveMainNode(pDiagGrid, i, 0, tDocument, pRootElem, false);
		if (NULL != pTmpElem)
		{
			pCondElem = pTmpElem;
		}
		else
		{
			SaveDataNode(pDiagGrid, i, 0, tDocument, pCondElem, MAX_DIAG_PARAM_NUM - 1);
		}
	}

}

// 保持主节点
XMLElement* CTaskEditorDoc::SaveMainNode(CGridCtrl* pGridCtrl, int nRowNum, int nColumnNum, 
	tinyxml2::XMLDocument& tDocument, XMLElement* pParentElem, bool bAttribute)
{
	char acBuffer[4096] = { 0 };
	CGridCellBase* pCellBase = pGridCtrl->GetCell(nRowNum, nColumnNum);
	if (NULL == pCellBase)
	{
		return NULL;
	}

	CGridData* pGridData = (CGridData*)pCellBase->GetData();
	if (NULL == pGridData)
	{
		return NULL;
	}

	CTaskMainNode* pMainNode = (CTaskMainNode*)pGridData->mData;
	if (NULL == pMainNode)
	{
		return NULL;
	}

	if (pGridData->mDataType != emDataType_MainNode)
	{
		return NULL;
	}


	UnicodeToUtf8(pMainNode->mDesName.c_str(), acBuffer, sizeof(acBuffer));
	XMLElement* pMainElem = tDocument.NewElement(acBuffer);


	if (bAttribute)
	{
		CString strValue = pGridCtrl->GetItemText(nRowNum, nColumnNum + 1);
		if (pMainNode->mOptionList.size() > 0)
		{
			strValue = strValue.Left(strValue.Find(_T(",")));
		}

		UnicodeToUtf8(strValue.GetBuffer(), acBuffer, sizeof(acBuffer));
		pMainElem->SetAttribute("Value", acBuffer);
	}

	pParentElem->LinkEndChild(pMainElem);
	return pMainElem;
}

/// 保存数据节点
void CTaskEditorDoc::SaveDataNode(CGridCtrl* pGridCtrl, int nRowNum, int nColumnNum,
	tinyxml2::XMLDocument& tDocument, XMLElement* pParentElem, int nParamNum)
{

	char acBuffer[4096] = { 0 };
	CGridCellBase* pCellBase = pGridCtrl->GetCell(nRowNum, nColumnNum);
	if (NULL == pCellBase)
	{
		return;
	}

	CGridData* pGridData = (CGridData*)pCellBase->GetData();
	if (NULL == pGridData)
	{
		return;
	}

	CTaskMainNode* pMainNode = (CTaskMainNode*)pGridData->mData;
	if (NULL == pMainNode)
	{
		return;
	}

	if (pGridData->mDataType != emDataType_DataNode)
	{
		return;
	}

	CString strValue = pCellBase->GetText();
	strValue = strValue.Left(strValue.Find(_T(",")));

	XMLElement* pMainElem = tDocument.NewElement("Cond");
	UnicodeToUtf8(strValue.GetBuffer(), acBuffer, sizeof(acBuffer));
	pMainElem->SetAttribute("Type", acBuffer);

	for (int j = 0; j < nParamNum; ++j)
	{
		CGridCellBase* pParamCell = pGridCtrl->GetCell(nRowNum, j + 1);
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

		char acAttributeName[MAX_PATH] = { 0 };
		_snprintf_s(acAttributeName, sizeof(acAttributeName), "Para%d", j);
		UnicodeToUtf8(strParam.GetBuffer(), acBuffer, sizeof(acBuffer));
		pMainElem->SetAttribute(acAttributeName, acBuffer);
	}
	pParentElem->LinkEndChild(pMainElem);
}

BOOL CTaskEditorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;


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

/// 从XML中加载任务配置
void CTaskEditorDoc::OpenDocument()
{
	if (m_strPathName == _T(""))
	{
		return;
	}

	tinyxml2::XMLDocument tDocument;
	char acBuffer[4096] = { 0 };
	UnicodeToAnsi(m_strPathName, acBuffer, sizeof(acBuffer));
	if (tinyxml2::XML_SUCCESS != tDocument.LoadFile(acBuffer))
	{
		return;
	}

	LoadFromXml(tDocument, false);
}

/// 从XML中加载任务配置
void CTaskEditorDoc::LoadFromXml(tinyxml2::XMLDocument& tDocument, bool bCopy)
{
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
	// 主节点
	CGridCtrl* pManGrid = pView->mMainGrid;
	for (int i = 0; i < pManGrid->GetRowCount(); ++i)
	{
		for (int j = 0; j < pManGrid->GetColumnCount(); j += 2)
		{
			LoadMainNode(pManGrid, i, j, tDocument, pRootElem, true);
		}
	}
	if (!bCopy)
	{
		// 特殊处理，将ID列置成不能编辑的
		pManGrid->SetItemState(0, 1, pManGrid->GetItemState(0, 1) | GVIS_READONLY);
	}
	
	// 条件节点
	CGridCtrl* pCondGrid = pView->mCondGrid;
	for (int i = 0; i < pCondGrid->GetRowCount(); ++i)
	{

		XMLElement* pMainElem = LoadMainNode(pCondGrid, i, 0, tDocument, pRootElem, false);
		if (NULL != pMainElem)
		{
			int nLineCount = LoadDataNode(pCondGrid, i, 0, tDocument, pMainElem, MAX_COND_PARAM_NUM);
			i += nLineCount;
		}
	}

	// 对话节点
	CGridCtrl* pDiagGrid = pView->mDiagGrid;
	for (int i = 0; i < pDiagGrid->GetRowCount(); ++i)
	{

		XMLElement* pMainElem = LoadMainNode(pDiagGrid, i, 0, tDocument, pRootElem, false);
		if (NULL != pMainElem)
		{
			int nLineCount = LoadDataNode(pDiagGrid, i, 0, tDocument, pMainElem, MAX_COND_PARAM_NUM);
			i += nLineCount;
		}
	}
	pManGrid->Invalidate();
	pCondGrid->Invalidate();
	pDiagGrid->Invalidate();
	SetModifiedFlag(FALSE);
}

/// 加载主节点
XMLElement*	CTaskEditorDoc::LoadMainNode(CGridCtrl* pGridCtrl, int nRowNum, int nColumnNum,
tinyxml2::XMLDocument& tDocument, XMLElement* pParentElem, bool bAttribute)
{
	char acBuffer[4096] = { 0 };
	wchar_t wBuffer[4096] = { 0 };
	CGridCellBase* pCellBase = pGridCtrl->GetCell(nRowNum, nColumnNum);
	if (NULL == pCellBase)
	{
		return NULL;
	}

	CGridData* pGridData = (CGridData*)pCellBase->GetData();
	if (NULL == pGridData)
	{
		return NULL;
	}

	CTaskMainNode* pMainNode = (CTaskMainNode*)pGridData->mData;
	if (NULL == pMainNode)
	{
		return NULL;
	}

	UnicodeToAnsi(pMainNode->mDesName.c_str(), acBuffer, sizeof(acBuffer));
	XMLElement* pMainElem = pParentElem->FirstChildElement(acBuffer);
	if (NULL == pMainElem)
	{
		return NULL;
	}

	if (bAttribute)
	{
		Utf8ToUnicode(pMainElem->Attribute("Value"), wBuffer, sizeof(wBuffer) / 2 - 1);
		if (pMainNode->mOptionList.size() > 0)
		{
			// 是否有外置的配置文件
			if (_T("") != pMainNode->mConfigName)
			{
				CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if (NULL != pMainFrame)
				{
					int nNum = _wtoi(wBuffer);
					pGridCtrl->SetItemText(nRowNum, nColumnNum + 1, pMainFrame->mTaskTemplate.FindOptionName(pMainNode->mConfigName, nNum).c_str());
				}
			}
			else
			{
				// 从本身的选项列表里取
				for (unsigned int nOptionNum = 0; nOptionNum < pMainNode->mOptionList.size(); ++nOptionNum)
				{
					int tOffset = pMainNode->mOptionList[nOptionNum]->mDes.find(',');
					wstring strValue = pMainNode->mOptionList[nOptionNum]->mDes.substr(0, tOffset);
					if (strValue == wBuffer)
					{
						pGridCtrl->SetItemText(nRowNum, nColumnNum + 1, pMainNode->mOptionList[nOptionNum]->mDes.c_str());
						break;
					}
				}
			}
		}
		else
		{
			pGridCtrl->SetItemText(nRowNum, nColumnNum + 1, wBuffer);
		}
	}

	return pMainElem;
}

/// 加载数据节点
int CTaskEditorDoc::LoadDataNode(CGridCtrl* pGridCtrl, int nRowNum, int nColumnNum,
	tinyxml2::XMLDocument& tDocument, XMLElement* pParentElem, int nParamNum)
{
	char acBuffer[4096] = { 0 };
	wchar_t wBuffer[4096] = { 0 };

	POSITION pos = GetFirstViewPosition();
	CTaskEditorView* pView = (CTaskEditorView*)GetNextView(pos);
	if (NULL == pView)
	{
		return 0;
	}
	CGridCellBase* pCellBase = pGridCtrl->GetCell(nRowNum, nColumnNum);
	if (NULL == pCellBase)
	{
		return 0;
	}

	CGridData* pGridData = (CGridData*)pCellBase->GetData();
	if (NULL == pGridData)
	{
		return 0;
	}

	CTaskMainNode* pMainNode = (CTaskMainNode*)pGridData->mData;
	if (NULL == pMainNode)
	{
		return 0;
	}

	int nNewLineNum = nRowNum;
	int nLineCount = 0;
	XMLElement* pCondElem = pParentElem->FirstChildElement("Cond");
	for (; NULL != pCondElem; pCondElem = pCondElem->NextSiblingElement("Cond"))
	{
		Utf8ToUnicode(pCondElem->Attribute("Type"), wBuffer, sizeof(wBuffer) / 2 - 1);
		wstring strCondType(wBuffer);
		if (pMainNode->mOptionList.size() > 0)
		{
			if (nNewLineNum == pGridCtrl->GetRowCount() - 1)
			{
				pGridCtrl->SetRowCount(pGridCtrl->GetRowCount() + 1);
			}
			else
			{
				pGridCtrl->InsertRow(_T(""), nNewLineNum + 1);
			}
			++nNewLineNum;

			CStringArray tOptionValue;
			for (int nParamCount = 0; nParamCount < 4; ++nParamCount)
			{
				char acAttributeName[MAX_PATH] = { 0 };
				_snprintf_s(acAttributeName, sizeof(acAttributeName), "Para%d", nParamCount);
				const char* pAttributeValue = pCondElem->Attribute(acAttributeName);
				if (NULL == pAttributeValue)
				{
					break;
				}
				Utf8ToUnicode(pAttributeValue, wBuffer, sizeof(wBuffer) / 2 - 1);
				tOptionValue.Add(wBuffer);
			}
			pView->AddCondRow(pGridCtrl, nNewLineNum, pMainNode, strCondType, tOptionValue, MAX_DIAG_PARAM_NUM);
			++nLineCount;
		}
	}

	return nLineCount;
}

/// 拷贝老的粘贴到一个新的界面上
void CTaskEditorDoc::PasteNew(LPCTSTR pStrName)
{
	POSITION pos = theApp.GetFirstDocTemplatePosition();
	CTaskEditorDoc* pFindDoc = NULL;
	while (pos != NULL)
	{
		CDocTemplate *p = theApp.GetNextDocTemplate(pos);
		POSITION posdoc = p->GetFirstDocPosition();
		while (posdoc != NULL)
		{
			CDocument* pDoc = p->GetNextDoc(posdoc);
			if (pDoc != NULL && pDoc->GetTitle() == pStrName)
			{
				pFindDoc = (CTaskEditorDoc*)pDoc;
				break;
			}
		}
		if (NULL != pFindDoc)
		{
			break;
		}
	}
	// 文档已经打开，可能有没保存的，不能直接从文件里加载，而是将界面上的先序列化到一个xml里面，
	// 在从xml反序列化到界面上
	if (pFindDoc != NULL)
	{
		tinyxml2::XMLDocument tDocument;
		pFindDoc->SaveToXml(tDocument);
		LoadFromXml(tDocument, true);
	}
	else
	{
		tinyxml2::XMLDocument tDocument;
		CString strFilePath = CString("Tasks\\") + pStrName + CString(_T(".xml"));
		char acBuffer[4096] = { 0 };

		UnicodeToAnsi(strFilePath, acBuffer, sizeof(acBuffer));
		if (tinyxml2::XML_SUCCESS == tDocument.LoadFile(acBuffer))
		{
			LoadFromXml(tDocument, true);	
		}
	}
}