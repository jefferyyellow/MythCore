
// TaskEditorDoc.cpp : CTaskEditorDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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


// CTaskEditorDoc ����/����

CTaskEditorDoc::CTaskEditorDoc()
{
	// TODO:  �ڴ����һ���Թ������
}

CTaskEditorDoc::~CTaskEditorDoc()
{
}

BOOL CTaskEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO:  �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)
	SetTitle(_T("*.xml"));
	return TRUE;
}




// CTaskEditorDoc ���л�
//
//void CTaskEditorDoc::Serialize(CArchive& ar)
//{
//	if (ar.IsStoring())
//	{
//
//	}
//	else
//	{
//		// TODO:  �ڴ���Ӽ��ش���
//	}
//}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CTaskEditorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
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

// ������������֧��
void CTaskEditorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:     strSearchContent = _T("point;rectangle;circle;ole object;")��
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

// CTaskEditorDoc ���

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


// CTaskEditorDoc ����


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
		AfxMessageBox(_T("����ʧ�ܣ�ע���������������ļ�����������Ϊ�ļ���,����1.xml"), MB_ICONEXCLAMATION);
		return FALSE;
	}

	CString strFileName = lpszPathName;
	strFileName = strFileName.Right(strFileName.GetLength() - strFileName.ReverseFind('\\') - 1);
	strFileName = strFileName.Left(strFileName.Find('.'));

	// �µ��ļ����ļ��
	if (_T("") == m_strPathName && lpszPathName[0] != _T('\0'))
	{

		if (strFileName != pView->GetMainGridText(_T("TaskId")))
		{
			AfxMessageBox(_T("����ʧ�ܣ�����ID���ļ�����ƥ��!"), MB_ICONEXCLAMATION);
			return FALSE;
		}

		CString str = pView->GetMainGridText(_T("TaskType"));
		if (pView->GetMainGridText(_T("TaskType")) == _T(""))
		{
			AfxMessageBox(_T("����ʧ�ܣ���������Ϊ��!"), MB_ICONEXCLAMATION);
			return FALSE;
		}

		if (pView->GetMainGridText(_T("TaskName")) == _T(""))
		{
			AfxMessageBox(_T("����ʧ�ܣ���������Ϊ��!"), MB_ICONEXCLAMATION);
			return FALSE;
		}
		for (int i = 0; i < strFileName.GetLength(); ++ i)
		{
			if (strFileName[i] < _T('0') || strFileName[i] > _T('9'))
			{
				AfxMessageBox(_T("����ʧ�ܣ�ע���������������ļ�����������Ϊ�ļ���,����1.xml"), MB_ICONEXCLAMATION);
				return FALSE;
			}
		}
	}

	char acBuffer[4096] = { 0 };
	tinyxml2::XMLDocument tDocument;
	// ���ֵ�XML�ļ�
	SaveToXml(tDocument);
	UnicodeToAnsi(lpszPathName, acBuffer, sizeof(acBuffer));
	tDocument.SaveFile(acBuffer);
	// ���޸ı�־����ΪFALSE
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
	/// �����ļ�����ͼ�Ľڵ�
	pMainFrame->UpdateFileViewItem(strTaskID, strTaskType, strTaskName);
	return TRUE;
}

/// �����񱣴浽xml�ļ���ȥ
void CTaskEditorDoc::SaveToXml(tinyxml2::XMLDocument& tDocument)
{
	// TODO:  �ڴ����ר�ô����/����û���
	char acBuffer[4096] = { 0 };

	XMLDeclaration* pDeclaration = tDocument.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"");
	tDocument.LinkEndChild(pDeclaration);

	XMLElement* pRootElem = tDocument.NewElement("TaskData");
	tDocument.LinkEndChild(pRootElem);

	POSITION pos = GetFirstViewPosition();
	CTaskEditorView* pView = (CTaskEditorView*)GetNextView(pos);

	// ���ڵ�
	CGridCtrl* pManGrid = pView->mMainGrid;
	for (int i = 0; i < pManGrid->GetRowCount(); ++i)
	{
		for (int j = 0; j < pManGrid->GetColumnCount(); j += 2)
		{
			SaveMainNode(pManGrid, i, j, tDocument, pRootElem, true);
		}
	}

	// �����ڵ�
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

	// �Ի��ڵ�
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

// �������ڵ�
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

/// �������ݽڵ�
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


	// TODO:  �ڴ������ר�õĴ�������
	char acBuffer[4096] = { 0 };

	tinyxml2::XMLDocument tDocument;
	UnicodeToAnsi(lpszPathName, acBuffer, sizeof(acBuffer));
	if (tinyxml2::XML_SUCCESS != tDocument.LoadFile(acBuffer))
	{
		return FALSE;
	}
	return TRUE;
}

/// ��XML�м�����������
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

/// ��XML�м�����������
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
	// ���ڵ�
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
		// ���⴦����ID���óɲ��ܱ༭��
		pManGrid->SetItemState(0, 1, pManGrid->GetItemState(0, 1) | GVIS_READONLY);
	}
	
	// �����ڵ�
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

	// �Ի��ڵ�
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

/// �������ڵ�
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
			// �Ƿ������õ������ļ�
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
				// �ӱ����ѡ���б���ȡ
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

/// �������ݽڵ�
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

/// �����ϵ�ճ����һ���µĽ�����
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
	// �ĵ��Ѿ��򿪣�������û����ģ�����ֱ�Ӵ��ļ�����أ����ǽ������ϵ������л���һ��xml���棬
	// �ڴ�xml�����л���������
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