#pragma once
/********************************************************************
	created:	13/12/2011   13:33
	filename: 	TemplateManager.h
	author:		jeffery

	purpose:	读取模板,原始模板的管理类
*********************************************************************/
#include "CommonDefine.h"
class CCatalogTree;
class TiXmlElement;
class CTemplateInfo;
// 模板字段数据
class CTemplateFieldData
{
public:
	CTemplateFieldData(){m_emTempLoadState = emLoadState_Null;}
	~CTemplateFieldData(){}
	CTemplateFieldData(CTemplateFieldData& rData);

public:
	CTemplateInfo*		GetParent(){return m_pParentInfo;}

public:
	TmTempLoadState			m_emTempLoadState;
	CTemplateInfo*			m_pParentInfo;		// 所属的模板表
	int						m_nTemplateID;		// 模板ID
	string					m_strTemplateName;	// 模板名称
	vector<string>			m_vecValue;			// 模板字段


};
class COriginalTemplate;
// 模板
class CTemplateInfo
{
public:
	CTemplateInfo();
	~CTemplateInfo();

public:
	void Free();
	void RemoveFieldData(CTemplateFieldData* pFieldData)
	{
		for (vector<CTemplateFieldData*>::iterator it = m_vecTemplateFieldData.begin();
			it != m_vecTemplateFieldData.end(); it++)
		{
			if (*(it) == pFieldData)
			{
				m_vecTemplateFieldData.erase(it);
				return;
			}
		}
	}

public:
	volatile TmTempLoadState	m_emTempLoadState;		// 模板的加载状态
	CString						m_strPathFile;			// 记录其加载路径
	// 模板数据与原始模板数据一一匹配
	vector<CTemplateFieldData*>	m_vecTemplateFieldData;
	COriginalTemplate*			m_pParent;
	BOOL						m_bDirty;				// 数据是否已经修改过
};

// 原始模板字段
class COriginalField
{
public:
	string		m_strFieldName;		// 字段名
	string		m_strFieldType;		// 字段类值
	string		m_strType;			// 字段编辑类型
};

// 原始模板类
class COriginalTemplate
{
public:
	COriginalTemplate(){}
	~COriginalTemplate(){Free();}
	
	void Free();

public:
	int						m_nTemplateSize;		// 模板所占字段的大少,单位:比特
	CString					m_strOriginName;
	vector<COriginalField>	m_vecOriginalField;
	vector<CTemplateInfo*>	m_vecTemplate;
};
class CTemplateManager
{
public:
	enum
	{
		// 每从文件中取一行用的BUFF大小
		emLine_Max_Char		= 1024,
	};

public:
	CTemplateManager(void);
	~CTemplateManager(void);

public:
	CTemplateInfo*	ReadTemplateTable(CString strPath, CString strFile, COriginalTemplate* pOriginTemp);
	COriginalTemplate*	ReadOriginTemplate(CString strPath, CString strOriginName);
	//BOOL	ReadTemplate(CTemplateInfo* pTemplateInfo, CString strPath);
	//BOOL	ParseTemplateHead(CTemplateInfo* pTemplateInfo, hash_map<size_t, size_t>& hashFieldIndex, string strLine);
	//BOOL	ParseTemplateBody(CTemplateInfo* pTemplateInfo, hash_map<size_t, size_t>& hashFieldIndex, string strLine);
	//int		GetOriginTempIndex(CString strOriginFileName);
	COriginalTemplate* GetOriginalTemplate(CTemplateInfo* pTemplateInfo);
	COriginalTemplate* GetOriginalTemplate(CString strOriginName);
	BOOL	CheckSameTemplateTable(COriginalTemplate* pOriginalTemplate, CString strTemplateTableName);
	//CTemplateInfo* GetTemplateInfo(CString strTemplateName);
	int		CalculateOriginSize(COriginalTemplate* pOriginTemplate);
	int		GetTemplateInfoFieldNum(CTemplateInfo* pTemplateInfo);
	BOOL	WriteTemplateInfoToFile(CTemplateInfo* pTemplateInfo);
	BOOL	WriteTemplateInfoHead(COriginalTemplate* pOrigin, FILE* pFile);
	BOOL	WriteTemplateInfoBody(CTemplateInfo* pTemplateInfo, FILE* pFile);
	void	AddNewTemplateField(CTemplateFieldData* pFieldData);
	BOOL	IsExistTemplate(int nTemplateID);
	CTemplateFieldData* GetTemplateInfoByID(int nTemplateID);

public:
	BOOL	ReadTemplate_C(CTemplateInfo* pTemplateInfo, TCHAR* pPath);
	char*	ParseTemplateHead_C(CTemplateInfo* pTemplateInfo, hash_map<size_t, size_t>& hashFieldIndex, char* pFileStart, char* pFileEnd);
	BOOL	ParseTemplateBody_C(CTemplateInfo* pTemplateInfo, hash_map<size_t, size_t>& hashFieldIndex, char* pFileStart, char* pFileEnd);
	BOOL	ReadTemplate_CNext(CTemplateInfo* pTemplateInfo, TCHAR* pPath);
	BOOL	ParseTemplateBody_CNext(CTemplateInfo* pTemplateInfo, hash_map<size_t, size_t>& hashFieldIndex, char* pFileStart, char* pFileEnd);

	// 从树创建模板数据
	void	BuildTempFromTree(COriginalTemplate* pOriginal, TCHAR* pServerPath, CTreeCtrl* pTree, HTREEITEM pParent);
	// 从树节点创建模板数据
	void	BuildTempFromTreeItem(COriginalTemplate* pOriginal, TCHAR* pServerPath, CTreeCtrl* pTree, HTREEITEM pTreeItem);
	// 加载所有没有完全加载的模板数据，在生成二进制前将所有的数据全加载进来
	BOOL	LoadAllData();
	void	WriteAllDirtyData();

public:
	vector<COriginalTemplate*>& GetOriginalTemp(){return m_vecOriginalTemp;}
	
public:
	static CTemplateManager*	GetSingletonPtr(){return m_pInst;}

public:
	void	Free();

private:
	// 原始模板数据
	vector<COriginalTemplate*>	m_vecOriginalTemp;
	// ID 到数据的索引
	hash_map<int, CTemplateFieldData*> m_hashTempFieldData;

private:
	static CTemplateManager* m_pInst;
};
