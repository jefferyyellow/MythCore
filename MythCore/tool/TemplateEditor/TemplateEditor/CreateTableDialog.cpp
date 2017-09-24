// CreateTableDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "TemplateEditor.h"
#include "CreateTableDialog.h"
#include "MainFrm.h"
#include "CatalogTree.h"
#include "CommonDefine.h"
#include "tinyxml2.h"

// CCreateTableDialog 对话框

IMPLEMENT_DYNAMIC(CCreateTableDialog, CDialog)

CCreateTableDialog::CCreateTableDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateTableDialog::IDD, pParent)
	, m_strTableName(_T(""))
{

}

CCreateTableDialog::~CCreateTableDialog()
{
}

void CCreateTableDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TABLENAME, m_strTableName);
}


BEGIN_MESSAGE_MAP(CCreateTableDialog, CDialog)
	ON_BN_CLICKED(ID_CREATETABLE, &CCreateTableDialog::OnBnClickedCreatetable)
END_MESSAGE_MAP()


// CCreateTableDialog 消息处理程序

void CCreateTableDialog::OnBnClickedCreatetable()
{

	UpdateData(TRUE);

	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainFrame = (CMainFrame*) ( (CMainFrame*) AfxGetMainWnd( ) );
	CTreeCtrl& CatalogTree = pMainFrame->GetCatalogTreeDialog().GetCatalogTree();

	HTREEITEM pSelectItem = CatalogTree.GetSelectedItem();
	if (NULL == pSelectItem)
	{
		return;
	}
	
	CTreeItemData* pData = (CTreeItemData*)CatalogTree.GetItemData(pSelectItem);
	if(NULL == pData || TEMP_ITEM_ORIGIN != pData->GetItemType())
	{
		return;
	}
		
	// 得到模板表的路径
	CString strText = CatalogTree.GetItemText(pSelectItem);
	CString strPath;
	pMainFrame->GetCatalogTreeDialog().GetTemplatePath(pSelectItem, strPath);

	strPath += strText + _T("\\") + m_strTableName + _T(".txt");
	COriginalTemplate* pOrigin = CTemplateManager::GetSingletonPtr()->GetOriginalTemplate(strText);
	if (NULL == pOrigin)
	{
		return;
	}

	if (CTemplateManager::GetSingletonPtr()->CheckSameTemplateTable(pOrigin, strPath))
	{
		AfxMessageBox(_T("不能有同名的模板表!"));
		return;
	}

	// 写模板表头
	FILE* pFile = _wfopen(strPath.GetBuffer(), _T("wt"));
	if (pFile == NULL)
	{
		return;
	}

	fwrite("模板ID", strlen("模板ID"), 1, pFile);
	fwrite("	", 1, 1, pFile);
	fwrite("模板名称", strlen("模板名称"), 1, pFile);

	size_t nSize = pOrigin->m_vecOriginalField.size();
	for (size_t i = 0; i < nSize; i++)
	{
		fwrite("	", 1, 1, pFile);
		string& strFieldName = pOrigin->m_vecOriginalField[i].m_strFieldName;
		fwrite(strFieldName.c_str(), strFieldName.size(), 1, pFile);
	}
	fclose(pFile);


	// 给模板表分配内存
	CTemplateInfo* pNewTemplateInfo = new CTemplateInfo;
	if (NULL == pNewTemplateInfo)
	{
		// 报错吧
		return;
	}
	pNewTemplateInfo->m_strPathFile = strPath;
	pNewTemplateInfo->m_pParent = pOrigin;
	pNewTemplateInfo->m_emTempLoadState = emLoadState_Inited;
	pOrigin->m_vecTemplate.push_back(pNewTemplateInfo);

	// 分配树节点
	HTREEITEM pItem = CatalogTree.InsertItem( (LPCTSTR)(m_strTableName+_T(".txt")).GetBuffer(), 1, 1, pSelectItem );
	CatalogTree.SetItemImage( pItem, 1, 1 );
	CTreeItemData* pNewData = new CTreeItemData;
	if (pNewData == NULL)
	{
		// 报错吧
		return;
	}
	pNewData->SetData(pNewTemplateInfo);
	pNewData->SetItemType(TEMP_ITEM_TABLE);
	CatalogTree.SetItemData(pItem, (DWORD_PTR)pNewData);

	OnOK();

}

BOOL CCreateTableDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
