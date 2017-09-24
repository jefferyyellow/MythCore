// TemplateEditorView.cpp : CTemplateEditorView ���ʵ��
//

#include "stdafx.h"
#include "TemplateEditor.h"

#include "TemplateEditorDoc.h"
#include "TemplateEditorView.h"
#include "TemplateManager.h"
#include "CommonDefine.h"
#include "ComboBoxDialog.h"
#include "MultiCheckDialog.h"
#include "NumberDialog.h"
#include "StringDialog.h"
#include "TempPathDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTemplateEditorView

IMPLEMENT_DYNCREATE(CTemplateEditorView, CListView)

BEGIN_MESSAGE_MAP(CTemplateEditorView, CListView)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

// CTemplateEditorView ����/����

CTemplateEditorView::CTemplateEditorView()
{
	// TODO: �ڴ˴���ӹ������
	m_CurrTempData = NULL;
}

CTemplateEditorView::~CTemplateEditorView()
{
}

BOOL CTemplateEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CListView::PreCreateWindow(cs);
}

void CTemplateEditorView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	this->ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
	CListCtrl* tpList = (CListCtrl*) &GetListCtrl( );
	DWORD dwStyle1;
	dwStyle1 = tpList->GetStyle();
	//����listctrl��ʾ����
	dwStyle1 |= LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES | LVS_EX_FULLROWSELECT;
	tpList->SetExtendedStyle(dwStyle1);
	//����ͼƬ�����и�
	m_ImageList.Create(1,   20,   ILC_COLOR16|ILC_MASK,   1,   1); 
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_LISTVIEWICON));       
	tpList->SetImageList(&m_ImageList,   LVSIL_SMALL);

	//��ʼ��List��
	tpList->InsertColumn(0,L"����");
	tpList->SetColumnWidth(0,200);
	tpList->InsertColumn(1,L"ֵ");
	//����listvalue��Ŀ��
	CRect tListSheet;
	this->GetWindowRect( &tListSheet );
	ScreenToClient( tListSheet );
	int tWind			= tListSheet.Width( ) - 205;
	tpList->SetColumnWidth(1,tWind);

	// TODO: ���� GetListCtrl() ֱ�ӷ��� ListView ���б�ؼ���
	//  �Ӷ������������ ListView��

	//// ����һ������,���ڸı�����
	//m_Font.CreateFont( 
	//	20,                                                 //   nHeight 
	//	0,                                                   //   nWidth 
	//	0,                                                   //   nEscapement 
	//	0,                                                   //   nOrientation 
	//	FW_NORMAL,                                   //   nWeight 
	//	FALSE,                                           //   bItalic 
	//	FALSE,                                           //   bUnderline 
	//	0,                                                   //   cStrikeOut 
	//	ANSI_CHARSET,                             //   nCharSet 
	//	OUT_DEFAULT_PRECIS,                 //   nOutPrecision 
	//	CLIP_DEFAULT_PRECIS,               //   nClipPrecision 
	//	DEFAULT_QUALITY,                       //   nQuality 
	//	DEFAULT_PITCH   |   FF_SWISS,     //   nPitchAndFamily 
	//	_T("����"));     
	
}


// CTemplateEditorView ���

#ifdef _DEBUG
void CTemplateEditorView::AssertValid() const
{
	CListView::AssertValid();
}

void CTemplateEditorView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CTemplateEditorDoc* CTemplateEditorView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTemplateEditorDoc)));
	return (CTemplateEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CTemplateEditorView ��Ϣ�������
// ��ģ�����ݸ��µ���ͼ��
void CTemplateEditorView::Update(CTemplateFieldData* pTemplateFieldData)
{
	if (NULL == pTemplateFieldData)
	{
		return;
	}

	CTemplateInfo* pTemplateInfo = pTemplateFieldData->m_pParentInfo;
	if (NULL == pTemplateInfo)
	{
		return;
	}

	COriginalTemplate* pOriginTemplate = CTemplateManager::GetSingletonPtr()->GetOriginalTemplate(pTemplateInfo);
	if (NULL == pOriginTemplate)
	{
		return;
	}


	m_CurrTempData = pTemplateFieldData;
	vector<COriginalField>& OriginalField =  pOriginTemplate->m_vecOriginalField;

	CListCtrl* pListCtrl = &GetListCtrl( );
	//pListCtrl->SetFont(&m_Font);

	int nScrollPos	 = pListCtrl->GetScrollPos( SB_VERT );
	int nSelectIndex = pListCtrl->GetSelectionMark( );
	pListCtrl->DeleteAllItems( );
	pListCtrl->InsertItem( 0, _T("ģ��ID") );
	TCHAR acTempIDBuffer[ 32 ] = { 0 };
	swprintf_s( acTempIDBuffer, L"%d", pTemplateFieldData->m_nTemplateID );
	pListCtrl->SetItemText( 0, 1, acTempIDBuffer );
	TCHAR acDest[MAX_PATH] = {0};

	string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";
	setlocale(LC_ALL, "chs"); 

	for ( size_t i = 0; i < OriginalField.size(); i ++ )
	{
		mbstowcs(acDest,OriginalField[i].m_strFieldName.c_str(),OriginalField[i].m_strFieldName.size() + 1);

		// ʹ�������ַ������ڲ߻��Ķ�
		string strDest = GetTypeString(OriginalField[i].m_strType, pTemplateFieldData->m_vecValue[i]);
		if (strDest.size() > 0)
		{
			strDest = pTemplateFieldData->m_vecValue[i] + " ," +strDest;
		}
		else
		{
			strDest = pTemplateFieldData->m_vecValue[i];
		}
		pListCtrl->InsertItem( int( i + 1 ), acDest);
		
		mbstowcs(acDest, strDest.c_str(), strDest.size() + 1);

		pListCtrl->SetItemText( int( i + 1 ), 1, acDest);
	}

	pListCtrl->Scroll( CSize( 0, 21 * nScrollPos ) );

	setlocale(LC_ALL, curLocale.c_str());

}
// ˫�������ݱ༭��
void CTemplateEditorView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if(CTemplateWrite::GetSingletonPtr()->GetWritingPB())
	{
		AfxMessageBox(_T("��������ģ���У��������޸����ݣ�"));
		return;
	}

	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	COriginalTemplate* pOriginTemp = CTemplateManager::GetSingletonPtr()->GetOriginalTemplate(m_CurrTempData->GetParent());
	if (NULL == pOriginTemp)
	{
		return;
	}
	CListCtrl* pList		= (CListCtrl*)this;
	int nIndex	= pList->GetSelectionMark( );
	if( nIndex == -1 )
		return;

	if( nIndex == 0 )
	{
		::AfxMessageBox( L"ģ��ID�����޸�!");
		return;
	}
	
	nIndex -= 1;
	string strType = pOriginTemp->m_vecOriginalField[nIndex].m_strType;
	string strEditType;
	size_t nstrIndex = strType.find("(");
	if (nstrIndex != string::npos)
	{
		strEditType = strType.substr(0, nstrIndex);
	}
	else
	{
		strEditType = strType;
	}

	// ���ݲ�ͬ�����ݱ༭���ʹ򿪲�ͬ�ı༭�Ի���
	if (strEditType.find("COMBOSINGLE") != string::npos)
	{
		CComboBoxDialog dialog;
		dialog.DoModal();
	}
	else if (strEditType.find("CHECKMUTI") != string::npos)
	{
		CMultiCheckDialog dialog;
		dialog.DoModal();
	}
	else if (strEditType.find("EDITNUMBER") != string::npos)
	{
		CNumberDialog dialog;
		dialog.DoModal();
	}
	else if (strEditType.find("EDITSTRING") != string::npos)
	{
		CStringDialog dialog;
		dialog.DoModal();
	}
	else if (strEditType.find("TEMPPATH") != string::npos)
	{
		CTempPathDialog dialog;
		dialog.DoModal();
	}

	
	CListView::OnLButtonDblClk(nFlags, point);
}

// �õ������ַ���
string CTemplateEditorView::GetTypeString(string strType, string strOldValue)
{
	if (strType.find("COMBOSINGLE") == string::npos && strType.find("CHECKMUTI") == string::npos)
	{
		return "";
	}

	int nOldValue = atoi(strOldValue.c_str());
	// �жϵ�����û��ֵ���ǿո��ǿյ�
	if (nOldValue == 0)
	{
		size_t nIndex = 0;

		for(size_t i = 0; i < strOldValue.size(); i++)
		{
			if (strOldValue[i] == 32 || strOldValue[i] == 9)
			{
				continue;
			}
			nIndex = i;
			break;
		}
		if (nIndex >= strOldValue.size())
		{
			return "";
		}
	}
	if (strType.find("COMBOSINGLE") != string::npos)
	{
		return GetComboBoxString(strType, nOldValue);
	}
	else if (strType.find("CHECKMUTI") != string::npos)
	{
		return GetMutiString(strType, nOldValue);
	}

	return "";
}

// �õ���ѡһ���ַ���
string CTemplateEditorView::GetComboBoxString(string strType, int nOldValue)
{
	
	size_t nIndex = strType.find("(");
	if (nIndex != string::npos)
	{
		strType = strType.substr(nIndex + 1);
	}

	size_t nValueIndex = 0;
	string strName;
	string strResultValue;
	int nValue = 0;
	vector<DWORD>	vecData;
	while((nIndex = strType.find(",")) != string::npos)
	{
		strName = strType.substr(0, nIndex);
		nValueIndex = strName.find("=");
		if (nValueIndex != string::npos)
		{
			nValue = atoi(strName.substr(nValueIndex + 1).c_str());

			strName = strName.substr(0, nValueIndex);
		}
		else
		{
			if (((int)vecData.size()) - 1 >= 0)
			{
				nValue = vecData[vecData.size() - 1];
			}
			else
			{
				nValue = 1;
			}
		}

		vecData.push_back(nValue);
		if (nValue == nOldValue)
		{
			strResultValue = strName;
			return strResultValue;
		}
		strType = strType.substr(nIndex + 1);
	}


	nIndex = strType.find(")");
	if (nIndex != string::npos)
	{
		strName = strType.substr(0, nIndex);
		nValueIndex = strName.find("=");
		if (nValueIndex != string::npos)
		{
			nValue = atoi(strName.substr(nValueIndex + 1).c_str());
			vecData.push_back(nValue);
			strName = strName.substr(0, nValueIndex);
		}
		else
		{
			if (((int)vecData.size()) - 1 >= 0)
			{
				nValue = vecData[vecData.size() - 1];
			}
			else
			{
				nValue = 1;
			}
		}
		
		if (nValue == nOldValue)
		{
			strResultValue = strName;
			return strResultValue;
		}
	}

	return "";
}

// �õ���ѡ����ַ���
string CTemplateEditorView::GetMutiString(string strType, int nOldValue)
{
	string strResultValue;

	size_t nIndex = strType.find("(");
	if (nIndex != string::npos)
	{
		strType = strType.substr(nIndex + 1);
	}

	size_t nValueIndex = 0;
	string strName;
	int nValue = 0;
	int nCheckCount = 0;
	int nOffset = 1;
	while((nIndex = strType.find(",")) != string::npos && nCheckCount < 32)
	{
		strName = strType.substr(0, nIndex);
		strType = strType.substr(nIndex + 1);

		BOOL bCheck = nOldValue & 0x01;
		if (bCheck)
		{
			char acBuffer[MAX_PATH] = {0};
			_itoa(nCheckCount + 1, acBuffer, 10);
			strResultValue += string(acBuffer) + string(":") + strName + string(",");
		}
		nOldValue = nOldValue >> 1;
		nCheckCount ++;
	}


	nIndex = strType.find(")");
	if (nIndex != string::npos && nCheckCount <= 32)
	{
		strName = strType.substr(0, nIndex);
		BOOL bCheck = nOldValue & 0x01;
		if (bCheck)
		{
			char acBuffer[MAX_PATH] = {0};
			_itoa(nCheckCount + 1, acBuffer, 10);
			strResultValue += string(acBuffer) + string(":") + strName + string(",");
		}
	}
	strResultValue += ")";

	return strResultValue;
}