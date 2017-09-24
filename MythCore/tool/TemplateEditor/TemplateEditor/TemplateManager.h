#pragma once
/********************************************************************
	created:	13/12/2011   13:33
	filename: 	TemplateManager.h
	author:		jeffery

	purpose:	��ȡģ��,ԭʼģ��Ĺ�����
*********************************************************************/
#include "CommonDefine.h"
class CCatalogTree;
class TiXmlElement;
class CTemplateInfo;
// ģ���ֶ�����
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
	CTemplateInfo*			m_pParentInfo;		// ������ģ���
	int						m_nTemplateID;		// ģ��ID
	string					m_strTemplateName;	// ģ������
	vector<string>			m_vecValue;			// ģ���ֶ�


};
class COriginalTemplate;
// ģ��
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
	volatile TmTempLoadState	m_emTempLoadState;		// ģ��ļ���״̬
	CString						m_strPathFile;			// ��¼�����·��
	// ģ��������ԭʼģ������һһƥ��
	vector<CTemplateFieldData*>	m_vecTemplateFieldData;
	COriginalTemplate*			m_pParent;
	BOOL						m_bDirty;				// �����Ƿ��Ѿ��޸Ĺ�
};

// ԭʼģ���ֶ�
class COriginalField
{
public:
	string		m_strFieldName;		// �ֶ���
	string		m_strFieldType;		// �ֶ���ֵ
	string		m_strType;			// �ֶα༭����
};

// ԭʼģ����
class COriginalTemplate
{
public:
	COriginalTemplate(){}
	~COriginalTemplate(){Free();}
	
	void Free();

public:
	int						m_nTemplateSize;		// ģ����ռ�ֶεĴ���,��λ:����
	CString					m_strOriginName;
	vector<COriginalField>	m_vecOriginalField;
	vector<CTemplateInfo*>	m_vecTemplate;
};
class CTemplateManager
{
public:
	enum
	{
		// ÿ���ļ���ȡһ���õ�BUFF��С
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

	// ��������ģ������
	void	BuildTempFromTree(COriginalTemplate* pOriginal, TCHAR* pServerPath, CTreeCtrl* pTree, HTREEITEM pParent);
	// �����ڵ㴴��ģ������
	void	BuildTempFromTreeItem(COriginalTemplate* pOriginal, TCHAR* pServerPath, CTreeCtrl* pTree, HTREEITEM pTreeItem);
	// ��������û����ȫ���ص�ģ�����ݣ������ɶ�����ǰ�����е�����ȫ���ؽ���
	BOOL	LoadAllData();
	void	WriteAllDirtyData();

public:
	vector<COriginalTemplate*>& GetOriginalTemp(){return m_vecOriginalTemp;}
	
public:
	static CTemplateManager*	GetSingletonPtr(){return m_pInst;}

public:
	void	Free();

private:
	// ԭʼģ������
	vector<COriginalTemplate*>	m_vecOriginalTemp;
	// ID �����ݵ�����
	hash_map<int, CTemplateFieldData*> m_hashTempFieldData;

private:
	static CTemplateManager* m_pInst;
};
