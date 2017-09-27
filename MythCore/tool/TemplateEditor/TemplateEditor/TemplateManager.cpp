#include "StdAfx.h"
#include "TemplateManager.h"
#include "CommonDefine.h"
#include "tinyxml2.h"
#include "CatalogTree.h"
#include "TemplateWrite.h"

CTemplateFieldData::CTemplateFieldData(CTemplateFieldData& rData)
{
	if (this == &rData)
	{
		return;
	}
	m_pParentInfo = rData.m_pParentInfo;
	m_vecValue = rData.m_vecValue;
};

/**************************************************************/
// CCTemplateInfo
/**************************************************************/
CTemplateInfo::CTemplateInfo()
{
	m_bDirty = FALSE;
}

CTemplateInfo::~CTemplateInfo()
{
	Free();
}

void CTemplateInfo::Free()
{
	for (size_t i = 0; i < m_vecTemplateFieldData.size(); i++)
	{
		if (NULL != m_vecTemplateFieldData[i])
		{
			delete m_vecTemplateFieldData[i];
		}
	}
	m_vecTemplateFieldData.clear();
}


void COriginalTemplate::Free()
{
	for(size_t i = 0; i < m_vecTemplate.size(); i++)
	{
		if (NULL != m_vecTemplate[i])
		{
			delete m_vecTemplate[i];
		}
	}

	m_vecTemplate.clear();
}

/**************************************************************/
// CTemplateManager
/**************************************************************/
CTemplateManager* CTemplateManager::m_pInst = NULL;
CTemplateManager::CTemplateManager(void)
{
	if (NULL != m_pInst)
	{
		return;
	}
	m_pInst = this;
}

CTemplateManager::~CTemplateManager(void)
{
	Free();
}


void CTemplateManager::Free()
{
	for (size_t i = 0; i < m_vecOriginalTemp.size(); i++)
	{
		if (NULL != m_vecOriginalTemp[i])
		{
			delete m_vecOriginalTemp[i];
		}
	}
	m_vecOriginalTemp.clear();
}

// 读模板表
CTemplateInfo* CTemplateManager::ReadTemplateTable(CString strPath, CString strFile, COriginalTemplate* pOriginTemp)
{
	CTemplateInfo* pTemplateinfo = new CTemplateInfo;
	if (NULL == pTemplateinfo)
	{
		return NULL;
	}

	pTemplateinfo->m_strPathFile = strPath + strFile;
	pTemplateinfo->m_pParent = pOriginTemp;
	pTemplateinfo->m_emTempLoadState = emLoadState_InitHead;
	//int nOriginIndex = GetOriginTempIndex(strOriginFile);
	//if (nOriginIndex < 0)
	//{
	//	return NULL;
	//}

	//pTemplateinfo->m_nOriginTempIndex = pOriginTemp->;

	//ReadOriginTemplate(pTemplateinfo, strPath + _T("#") + strFile);
	//ReadTemplate(pTemplateinfo, strPath + strFile);


	if(!ReadTemplate_C(pTemplateinfo, CString(strPath + strFile).GetBuffer()))
	{
		delete pTemplateinfo;
		return NULL;
	}
	//ReadTemplate(pTemplateinfo, strPath + strFile);


	// 如果没有模板数据就直接初始化完成了
	if (pTemplateinfo->m_vecTemplateFieldData.size() <= 0)
	{
		pTemplateinfo->m_emTempLoadState = emLoadState_Inited;
	}

	pOriginTemp->m_vecTemplate.push_back(pTemplateinfo);
	return pTemplateinfo;
}

// 读原始模板
COriginalTemplate* CTemplateManager::ReadOriginTemplate(CString strPath, CString strOriginName)
{
	ifstream InFile;
	InFile.open(strPath.GetBuffer());
	if (!InFile.is_open())
	{
		// 报错吧
		return FALSE;
	}

	COriginalTemplate* pOriginalTemplate = new COriginalTemplate;
	if (NULL == pOriginalTemplate)
	{
		return NULL;
	}

	pOriginalTemplate->m_strOriginName = strOriginName;

	string strLine = "";
	COriginalField Field;

	// 一行一行分析
	while(getline(InFile, strLine))
	{
		string::size_type nIndex = strLine.find(":");
		if (nIndex == string::npos)
		{
			continue;
		}

		string strKey = strLine.substr(0, nIndex);
		string strValue = strLine.substr(nIndex + 1);
	
		strValue.erase(strValue.find_last_not_of(" ") + 1);
		strValue.erase(0, strValue.find_first_not_of(" "));
		strValue.erase(strValue.find_last_not_of(char(0x0)) + 1);
		
		if (strKey == "FieldName")
		{
			Field.m_strFieldName = strValue;
		}
		else if (strKey == "FieldType")
		{
			Field.m_strFieldType = strValue;
		}
		// 一个字段结束了
		else if (strKey == "Type")
		{
			Field.m_strType = strValue;
			pOriginalTemplate->m_vecOriginalField.push_back(Field);
		}
	}
	InFile.close();

	pOriginalTemplate->m_nTemplateSize = CalculateOriginSize(pOriginalTemplate);
	m_vecOriginalTemp.push_back(pOriginalTemplate);
	

	return pOriginalTemplate;
}

// 读模板
BOOL CTemplateManager::ReadTemplate_C(CTemplateInfo* pTemplateInfo, TCHAR* pPath)
{
	FILE* pFile = _wfopen(pPath, _T("rb"));
	if (NULL == pFile)
	{
		return FALSE;
	}

	fseek(pFile, 0, SEEK_END);
	int nFileSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	//读入内存
	char* pMemory = new char[nFileSize+1];
	fread(pMemory, 1, nFileSize, pFile);
	pMemory[nFileSize] = 0;

	// 用于记录表头与原始模板字段的对应
	hash_map<size_t, size_t>		hashFieldIndex;

	// 分析模板头
	char* pNext = ParseTemplateHead_C(pTemplateInfo, hashFieldIndex, pMemory, pMemory + nFileSize + 1);
	// 分析模板体
	BOOL bResult = ParseTemplateBody_C(pTemplateInfo, hashFieldIndex, pNext, pMemory + nFileSize + 1);
	delete []pMemory;

	fclose(pFile);
	return bResult;
}


BOOL CTemplateManager::ReadTemplate_CNext(CTemplateInfo* pTemplateInfo, TCHAR* pPath)
{
	if(NULL == pTemplateInfo)
	{
		return FALSE;
	}

	FILE* pFile = _wfopen(pPath, _T("rb"));
	if (NULL == pFile)
	{
		return FALSE;
	}

	fseek(pFile, 0, SEEK_END);
	int nFileSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	//读入内存
	char* pMemory = new char[nFileSize+1];
	fread(pMemory, 1, nFileSize, pFile);
	pMemory[nFileSize] = 0;

	// 用于记录表头与原始模板字段的对应
	hash_map<size_t, size_t>		hashFieldIndex;
	char* pNext = ParseTemplateHead_C(pTemplateInfo, hashFieldIndex, pMemory, pMemory + nFileSize + 1);

	ParseTemplateBody_CNext(pTemplateInfo, hashFieldIndex, pNext, pMemory + nFileSize + 1);
	delete []pMemory;
	fclose(pFile);
	return TRUE;
}

// 分析模板表头
char* CTemplateManager::ParseTemplateHead_C(CTemplateInfo* pTemplateInfo, hash_map<size_t, size_t>& hashFieldIndex, char* pFileStart, char* pFileEnd)
{

	if (NULL == pTemplateInfo)
	{
		return FALSE;
	}

	vector<COriginalField>& OriginalField = pTemplateInfo->m_pParent->m_vecOriginalField;

	char* pStart = pFileStart;
	char* pWorldStart = pFileStart;
	int nFieldCount = 0;
	// 记录已经匹配到头的那个字段了
	int nFieldRecorder = 0;
	//char* pWorldEnd = 0;
	for (; pStart < pFileEnd; pStart++)
	{
		if ((*pStart=='\r' || *pStart=='\n'))
		{
			*pStart = '\0';
			break;
		}
		
		// TAB键
		if (*pStart == 0x09)
		{
			*pStart = '\0';
			int nIndex;
			for (nIndex = nFieldRecorder; nIndex < (int)OriginalField.size(); nIndex++)
			{
				if (strcmp(pWorldStart, OriginalField[nIndex].m_strFieldName.c_str()) == 0)
				{
					hashFieldIndex[nFieldCount] = nIndex;
					nFieldRecorder = nIndex + 1;
					break;
				}
			}

			// 如果从记录处往后搜索没有搜索到,只能再从头搜索了
			if(nIndex >= (int)OriginalField.size())
			{
				for (int i = 0; i < nFieldRecorder; i++)
				{
					if (strcmp(pWorldStart, OriginalField[i].m_strFieldName.c_str()) == 0)
					{
						hashFieldIndex[nFieldCount] = i;
						nFieldRecorder = i + 1;
						break;
					}
				}
			}
			nFieldCount++;
			// 注意,这个不能赋值了,没有做判断是否合法的
			pWorldStart = pStart + 1;
			continue;
		}
	}

	// 表头与原始模板对应
	for (int i = nFieldRecorder; i < (int)OriginalField.size(); i++)
	{
		if (strcmp(pWorldStart, OriginalField[i].m_strFieldName.c_str()) == 0)
		{
			hashFieldIndex[nFieldCount] = i;
			nFieldRecorder = i;
			break;
		}
	}
	// 过滤回车和换行符
	for (pStart++; pStart < pFileEnd; pStart++)
	{
		if ((*pStart=='\r' || *pStart=='\n'))
		{
			continue;
		}
		break;
	}

	return pStart;
}

// 处理真正的字段数据
BOOL CTemplateManager::ParseTemplateBody_CNext(CTemplateInfo* pTemplateInfo, hash_map<size_t, size_t>& hashFieldIndex, char* pFileStart, char* pFileEnd)
{
	if (hashFieldIndex.size() <= 0)
	{
		return FALSE;
	}
	if (pTemplateInfo->m_vecTemplateFieldData.size() <= 0)
	{
		return FALSE;
	}

	CTemplateFieldData* pTemplateFieldData = pTemplateInfo->m_vecTemplateFieldData[0];
	if (NULL == pTemplateFieldData)
	{
		return FALSE;
	}
	int nFieldCount = 0;
	int nTemplateCount = 0;
	char* pStart = pFileStart;
	char* pWorldStart = pFileStart;
	for (; pStart < pFileEnd; pStart++)
	{
		// 新的一行
		if ((*pStart=='\r' || *pStart=='\n'))
		{
			*pStart = '\0';

			hash_map<size_t, size_t>::iterator it = hashFieldIndex.find(nFieldCount);
			if (it != hashFieldIndex.end())
			{
				pTemplateFieldData->m_vecValue[it->second] = pWorldStart;
			}
			
			// 过滤掉回车和换行符
			for (pStart++; pStart < pFileEnd; pStart++)
			{
				if ((*pStart=='\r' || *pStart=='\n'))
				{
					continue;
				}
				break;
			}

			
			// 处理最行一行是空行及最后不是空行的情况
			nFieldCount = 0;
			pTemplateFieldData->m_emTempLoadState = emLoadState_Inited;
			if (pStart < pFileEnd -1)
			{
				nTemplateCount++;
				pTemplateFieldData = pTemplateInfo->m_vecTemplateFieldData[nTemplateCount];
				pWorldStart = pStart;
			}


		}

		// TAB键
		else if (*pStart == 0x09)
		{
			*pStart = '\0';
			if (nFieldCount >= 2)
			{
				hash_map<size_t, size_t>::iterator it = hashFieldIndex.find(nFieldCount);
				if (it != hashFieldIndex.end())
				{
					pTemplateFieldData->m_vecValue[it->second] = pWorldStart;
				}
			}

			nFieldCount++;
			// 注意,这个不能赋值了,没有做判断是否合法的
			pWorldStart = pStart + 1;
			continue;
		}
	}

	// 是否是上一行没读完的，处理最行一行是空行及最后不是空行的情况
	if (nFieldCount > 0)
	{
		hash_map<size_t, size_t>::iterator it = hashFieldIndex.find(nFieldCount);
		if (it != hashFieldIndex.end())
		{
			pTemplateFieldData->m_vecValue[it->second] = pWorldStart;
		}
		pTemplateFieldData->m_emTempLoadState = emLoadState_Inited;
	}

	return TRUE;
}

// 分析模板体
BOOL CTemplateManager::ParseTemplateBody_C(CTemplateInfo* pTemplateInfo, hash_map<size_t, size_t>& hashFieldIndex, char* pFileStart, char* pFileEnd)
{
	if (hashFieldIndex.size() <= 0)
	{
		return FALSE;
	}

	// 分配一个模板数据
	CTemplateFieldData* pTemplateFieldData = new CTemplateFieldData;
	if (NULL == pTemplateFieldData)
	{
		return FALSE;
	}
	pTemplateFieldData->m_pParentInfo = pTemplateInfo;

	

	int nFieldCount = 0;
	pTemplateFieldData->m_vecValue.resize(pTemplateInfo->m_pParent->m_vecOriginalField.size());

	char* pStart = pFileStart;
	char* pWorldStart = pFileStart;
	for (; pStart < pFileEnd; pStart++)
	{
		// 新的一行，表示一个进入一个新的模板数据了
		if ((*pStart=='\r' || *pStart=='\n'))
		{
			*pStart = '\0';

			hash_map<size_t, size_t>::iterator it = hashFieldIndex.find(nFieldCount);
			if (it != hashFieldIndex.end())
			{
				pTemplateFieldData->m_vecValue[it->second] = pWorldStart;
			}
			
			// 过滤掉回车和换行
			for (pStart++; pStart < pFileEnd; pStart++)
			{
				if ((*pStart=='\r' || *pStart=='\n'))
				{
					continue;
				}
				break;
			}

			// 加入数据中
			pTemplateInfo->m_vecTemplateFieldData.push_back(pTemplateFieldData);
			AddNewTemplateField(pTemplateFieldData);

			
			nFieldCount = 0;
			// 处理最行一行是空行及最后不是空行的情况
			if (pStart < pFileEnd -1)
			{
				pTemplateFieldData = new CTemplateFieldData;
				if (NULL == pTemplateFieldData)
				{
					return FALSE;
				}
				pTemplateFieldData->m_pParentInfo = pTemplateInfo;

				pTemplateFieldData->m_vecValue.resize(pTemplateInfo->m_pParent->m_vecOriginalField.size());
				pWorldStart = pStart;
				
			}


		}

		// TAB键
		else if (*pStart == 0x09)
		{
			*pStart = '\0';
			//if (nFieldCount >= 2)
			//{
			//	hash_map<size_t, size_t>::iterator it = hashFieldIndex.find(nFieldCount);
			//	if (it != hashFieldIndex.end())
			//	{
			//		pTemplateFieldData->m_vecValue[it->second] = pWorldStart;
			//	}
			//}
			// 只读模板ID和模板名字，其它的信息后面再读
			if (nFieldCount == 0)
			{
				pTemplateFieldData->m_nTemplateID = atoi(pWorldStart);
				if (pTemplateFieldData->m_nTemplateID <= 0)
				{
					AfxMessageBox(_T("小于或者等于零的ID存在,请检查!"));
					return FALSE;
				}

				if (CTemplateManager::GetSingletonPtr()->IsExistTemplate(pTemplateFieldData->m_nTemplateID))
				{
					TCHAR acTempID[MAX_PATH] = {0};
					_snwprintf(acTempID, MAX_PATH, _T("已经有相同的模板ID存在,数据出错，请退出编辑器并找相关人员解决, 模板ID:%d"), pTemplateFieldData->m_nTemplateID);
					AfxMessageBox(acTempID);
					return FALSE;
				}
			}
			else if (nFieldCount == 1)
			{
				pTemplateFieldData->m_strTemplateName = pWorldStart;
			}
			

			nFieldCount++;
			// 注意,这个不能赋值了,没有做判断是否合法的
			pWorldStart = pStart + 1;
			continue;
		}
	}
	
	// 是否是上一行没读完的，处理最行一行是空行及最后不是空行的情况
	if (nFieldCount > 0)
	{
		hash_map<size_t, size_t>::iterator it = hashFieldIndex.find(nFieldCount);
		if (it != hashFieldIndex.end())
		{
			pTemplateFieldData->m_vecValue[it->second] = pWorldStart;
		}
		pTemplateInfo->m_vecTemplateFieldData.push_back(pTemplateFieldData);
		AddNewTemplateField(pTemplateFieldData);
	}


	return TRUE;
}

//BOOL CTemplateManager::ReadTemplate(CTemplateInfo* pTemplateInfo, CString strPath)
//{
//
//	string strLine;
//	ifstream InFile;
//	InFile.open(strPath);
//	if (!InFile.is_open())
//	{
//		// 报错吧
//		return FALSE;
//	}
//
//	// 用于记录表头与原始模板字段的对应
//	hash_map<size_t, size_t>		hashFieldIndex;
//	if(getline(InFile,strLine))
//	{
//		// 读第一行,解析表头
//		ParseTemplateHead(pTemplateInfo, hashFieldIndex, strLine);
//	}
//	while(getline(InFile,strLine))
//	{	
//		// 读数据行
//		ParseTemplateBody(pTemplateInfo, hashFieldIndex, strLine);
//	}
//	InFile.close();
//	return TRUE;
//}
//
//BOOL CTemplateManager::ParseTemplateHead(CTemplateInfo* pTemplateInfo, hash_map<size_t, size_t>& hashFieldIndex, string strLine)
//{
//	if (NULL == pTemplateInfo)
//	{
//		return FALSE;
//	}
//	if (pTemplateInfo->m_nOriginTempIndex >= m_vecOriginalTemp.size())
//	{
//		return FALSE;
//	}
//
//	string::size_type nStartIndex = 0;
//	string::size_type nEndIndex = 0;
//	
//
//	vector<COriginalField>& OriginalField = m_vecOriginalTemp[pTemplateInfo->m_nOriginTempIndex].m_vecOriginalField;
//
//	string strFieldName;
//	int nFieldCount = 0;
//	while((nEndIndex = strLine.find(char(0x09), nStartIndex)) != string::npos)
//	{
//		strFieldName = strLine.substr(nStartIndex, nEndIndex - nStartIndex);
//		strFieldName.erase(strFieldName.find_last_not_of(" ") + 1);
//		strFieldName.erase(0, strFieldName.find_first_not_of(" "));
//
//		for (size_t i = 0; i < OriginalField.size(); i++)
//		{
//			if (strFieldName == OriginalField[i].m_strFieldName)
//			{
//				hashFieldIndex[nFieldCount] = i;
//				break;
//			}
//		}
//
//		nFieldCount++;
//		nStartIndex = nEndIndex + 1;
//	}
//	
//	strFieldName = strLine.substr(nStartIndex);
//	strFieldName.erase(strFieldName.find_last_not_of(" ") + 1);
//	strFieldName.erase(0, strFieldName.find_first_not_of(" "));
//
//	for (size_t i = 0; i < OriginalField.size(); i++)
//	{
//		if (strFieldName == OriginalField[i].m_strFieldName)
//		{
//			hashFieldIndex[nFieldCount] = i;
//		}
//	}
//
//	return TRUE;
//}
//
//BOOL CTemplateManager::ParseTemplateBody(CTemplateInfo* pTemplateInfo, hash_map<size_t, size_t>& hashFieldIndex, string strLine)
//{
//	if (hashFieldIndex.size() <= 0)
//	{
//		return FALSE;
//	}
//
//	CTemplateFieldData* pTemplateFieldData = new CTemplateFieldData;
//	if (NULL == pTemplateFieldData)
//	{
//		return FALSE;
//	}
//	pTemplateFieldData->m_pParentInfo = pTemplateInfo;
//
//	string::size_type nStartIndex = 0;
//	string::size_type nEndIndex = 0;
//	string strFieldValue;
//	int nFieldCount = 0;
//	pTemplateFieldData->m_vecValue.resize(hashFieldIndex.size());
//	while((nEndIndex = strLine.find(char(0x09), nStartIndex)) != string::npos)
//	{
//		strFieldValue = strLine.substr(nStartIndex, nEndIndex - nStartIndex);
//		//strFieldValue.erase(strFieldValue.find_last_not_of(" ") + 1);
//		//strFieldValue.erase(0, strFieldValue.find_first_not_of(" "));
//		if (nFieldCount >= 2)
//		{
//			hash_map<size_t, size_t>::iterator it = hashFieldIndex.find(nFieldCount);
//			if (it != hashFieldIndex.end())
//			{
//				pTemplateFieldData->m_vecValue[it->second] = strFieldValue;
//			}
//		}
//		else if (nFieldCount == 0)
//		{
//			pTemplateFieldData->m_nTemplateID = atoi(strFieldValue.c_str());
//		}
//		else if (nFieldCount == 1)
//		{
//			pTemplateFieldData->m_strTemplateName = strFieldValue;
//		}
//
//		nStartIndex = nEndIndex + 1;
//		nFieldCount++;
//	}
//
//	strFieldValue = strLine.substr(nStartIndex);
//	//strFieldValue.erase(strFieldValue.find_last_not_of(" ") + 1);
//	//strFieldValue.erase(0, strFieldValue.find_first_not_of(" "));
//	pTemplateFieldData->m_vecValue[hashFieldIndex[nFieldCount]] = strFieldValue;
//	
//	pTemplateInfo->m_vecTemplateFieldData.push_back(pTemplateFieldData);
//	AddNewTemplateField(pTemplateFieldData);
//
//	return TRUE;
//}
//
//
//int CTemplateManager::GetOriginTempIndex(CString strOriginFileName)
//{
//	for (size_t i = 0; i < m_vecOriginalTemp.size(); i++)
//	{
//		if (m_vecOriginalTemp[i]->m_strOriginName == strOriginFileName)
//		{
//			return (int)i;
//		}
//	}
//
//	return -1;
//}

// 通过模板表得到其原始模板
COriginalTemplate* CTemplateManager::GetOriginalTemplate(CTemplateInfo* pTemplateInfo)
{
	if (NULL == pTemplateInfo)
	{
		return NULL;
	}

	return pTemplateInfo->m_pParent;
}

// 通过名字得到原始模板
COriginalTemplate* CTemplateManager::GetOriginalTemplate(CString strOriginName)
{
	for (size_t i = 0; i < m_vecOriginalTemp.size(); i++)
	{
		if (m_vecOriginalTemp[i]->m_strOriginName == strOriginName)
		{
			return m_vecOriginalTemp[i];
		}
	}

	return NULL;
}

BOOL CTemplateManager::CheckSameTemplateTable(COriginalTemplate* pOriginalTemplate, CString strTableFileName)
{	
	if (NULL == pOriginalTemplate)
	{
		return TRUE;
	}

	for (size_t i = 0; i < pOriginalTemplate->m_vecTemplate.size(); i++)
	{
		if (strTableFileName == pOriginalTemplate->m_vecTemplate[i]->m_strPathFile)
		{
			return TRUE;
		}
	}

	return FALSE;
}

// 计数原始模板所有字段的大小
int CTemplateManager::CalculateOriginSize(COriginalTemplate* pOriginTemplate)
{
	int nLength = 0;
	size_t nStringIndex = 0;
	for (size_t i = 0; i < pOriginTemplate->m_vecOriginalField.size(); i++)
	{
		string& strType = pOriginTemplate->m_vecOriginalField[i].m_strFieldType;
		if (strType.find("INT4") != string::npos)
		{
			nLength += 4;
		}
		else if (strType.find("INT8") != string::npos)
		{
			nLength += 8;
		}
		// 包括INT2 UINT2
		else if (strType.find("INT2") != string::npos)
		{
			nLength += 2;
		}
		else if ( (nStringIndex = strType.find("STRING", 0)) != string::npos)
		{
			string strText = strType.substr(nStringIndex + 6, strType.length() - nStringIndex - 6);
			nLength += atoi(strText.c_str());
		}
		else if (strType.find("FLOAT", 0) != string::npos)
		{
			nLength += sizeof(float);
		}
		else if (strType.find("DOUBLE", 0) != string::npos)
		{
			nLength += sizeof(double);
		}
	}
	return nLength;
}

// 得到模板表的字段数目
int CTemplateManager::GetTemplateInfoFieldNum(CTemplateInfo* pTemplateInfo)
{
	if (NULL == pTemplateInfo)
	{
		return 0;
	}
	COriginalTemplate* pOriginal = GetOriginalTemplate(pTemplateInfo);
	if (NULL == pOriginal)
	{
		return 0;
	}

	return (int)(pOriginal->m_vecOriginalField.size());
}

// 将模板表信息写入文件
BOOL CTemplateManager::WriteTemplateInfoToFile(CTemplateInfo* pTemplateInfo)
{
	if (NULL == pTemplateInfo)
	{
		return FALSE;
	}
	
	TCHAR* pFilePath = NULL;
	FILE* pFile = _wfopen(pTemplateInfo->m_strPathFile, _T("wt"));
	if (pFile == NULL)
	{
		return FALSE;
	}
	// 写表头
	WriteTemplateInfoHead(GetOriginalTemplate(pTemplateInfo), pFile);
	// 写表体
	WriteTemplateInfoBody(pTemplateInfo, pFile);
	pTemplateInfo->m_bDirty = FALSE;
	fclose(pFile);
	return TRUE;
}

// 写模板表头
BOOL CTemplateManager::WriteTemplateInfoHead(COriginalTemplate* pOrigin, FILE* pFile)
{
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
	return TRUE;
}

// 写模板表体
BOOL CTemplateManager::WriteTemplateInfoBody(CTemplateInfo* pTemplateInfo, FILE* pFile)
{
	char acBuffer[MAX_PATH] = {0};
	for (size_t i = 0; i < pTemplateInfo->m_vecTemplateFieldData.size(); i++)
	{
		fwrite("\n", 1, 1, pFile);
		CTemplateFieldData* pFieldData = pTemplateInfo->m_vecTemplateFieldData[i];
		if (NULL == pFieldData)
		{
			// 报错吧
			return FALSE;
		}

		memset(acBuffer, 0, sizeof(acBuffer));
		// 写模板ID
		_itoa(pFieldData->m_nTemplateID, acBuffer, 10);
		fwrite(acBuffer, strlen(acBuffer), 1, pFile);
		fwrite("	", 1, 1, pFile);
		// 写模板名字
		fwrite(pFieldData->m_strTemplateName.c_str(), pFieldData->m_strTemplateName.size(), 1, pFile);

		// 写入模板字段数据
		for (size_t j = 0; j < pFieldData->m_vecValue.size(); j++)
		{
			fwrite("	", 1, 1, pFile);
			string& strValue = pFieldData->m_vecValue[j];
			fwrite(strValue.c_str(), strValue.size(), 1, pFile);
		}
	}

	return TRUE;
}

// 加入一个新的模板
void CTemplateManager::AddNewTemplateField(CTemplateFieldData* pFieldData)
{
	if (NULL == pFieldData)
	{
		return;
	}
	m_hashTempFieldData[pFieldData->m_nTemplateID] = pFieldData;
}

BOOL CTemplateManager::IsExistTemplate(int nTemplateID)
{
	hash_map<int, CTemplateFieldData*>::iterator itFind = m_hashTempFieldData.find(nTemplateID);
	if (itFind != m_hashTempFieldData.end())
	{
		return TRUE;
	}
	return FALSE;
}


CTemplateFieldData* CTemplateManager::GetTemplateInfoByID(int nTemplateID)
{
	hash_map<int, CTemplateFieldData*>::iterator itFind = m_hashTempFieldData.find(nTemplateID);
	if (itFind != m_hashTempFieldData.end())
	{
		return itFind->second;
	}
	return NULL;
}
// 从树递归创建模板数据
void CTemplateManager::BuildTempFromTree(COriginalTemplate* pOriginal, TCHAR* pServerPath, CTreeCtrl* pTree, HTREEITEM pParent)
{
	if (pTree->ItemHasChildren(pParent))
	{
		for (HTREEITEM hChildItem = pTree->GetChildItem(pParent); NULL != hChildItem; hChildItem = pTree->GetNextSiblingItem(hChildItem))
		{
			BuildTempFromTreeItem(pOriginal, pServerPath, pTree, hChildItem);
		}
	}
}

// 从树节点创建模板数据
void CTemplateManager::BuildTempFromTreeItem(COriginalTemplate* pOriginal, TCHAR* pServerPath, CTreeCtrl* pTree, HTREEITEM pTreeItem)
{
	if (NULL == pTree || NULL == pTreeItem)
	{
		return;
	}


	CString strPath(pServerPath);
	DWORD_PTR dwData = pTree->GetItemData(pTreeItem);

	if (dwData == TEMP_ITEM_DIR)
	{
		strPath += pTree->GetItemText(pTreeItem) + CString("\\");
		// 更改树节点数据
		CTreeItemData* pData = new CTreeItemData;
		if (pData == NULL)
		{
			// 报错吧
			return;
		}
		pData->SetData(NULL);
		pData->SetItemType(TEMP_ITEM_DIR);
		pTree->SetItemData(pTreeItem, (DWORD_PTR)pData);
		// 递归创建
		BuildTempFromTree(pOriginal, strPath.GetBuffer(), pTree, pTreeItem);
	}
	// 原始模板
	else if (dwData == TEMP_ITEM_ORIGIN)
	{
		CString strOriginFile = strPath + pTree->GetItemText(pTreeItem) + CString("\\#") + pTree->GetItemText(pTreeItem) + CString(".txt");
		strPath += pTree->GetItemText(pTreeItem) + + CString("\\");

		COriginalTemplate* pOriginTemp = ReadOriginTemplate(strOriginFile, pTree->GetItemText(pTreeItem));
		if (pOriginTemp == NULL)
		{
			// 报错吧
			return;
		}

		// 更改树节点数据
		CTreeItemData* pData = new CTreeItemData;
		if (pData == NULL)
		{
			// 报错吧
			return;
		}
		pData->SetData(pOriginTemp);
		pData->SetItemType(TEMP_ITEM_ORIGIN);
		pTree->SetItemData(pTreeItem, (DWORD_PTR)pData);
		// 递归
		BuildTempFromTree(pOriginTemp, strPath.GetBuffer(), pTree, pTreeItem);
	}
	else if(dwData == TEMP_ITEM_TABLE)
	{
		CString strTableFile = strPath;
		CString strFileName = pTree->GetItemText(pTreeItem).Left(pTree->GetItemText(pTreeItem).GetLength() - 4);
		CString strFilterFile = strPath + strFileName + CString("_Filter.xml");
		CTemplateInfo* pNewTemplateInfo = ReadTemplateTable(strTableFile, pTree->GetItemText(pTreeItem), pOriginal);
		if (NULL == pNewTemplateInfo)
		{
			TCHAR acBuffer[MAX_PATH] = {0};
			_snwprintf(acBuffer, MAX_PATH, _T("读模板表出错:%s"), strTableFile);
			// 报错吧
			AfxMessageBox(acBuffer);
			exit(1);
			return ;
		}

		// 更改树节点数据
		CTreeItemData* pData = new CTreeItemData;
		if (pData == NULL)
		{
			// 报错吧
			return;
		}
		pData->SetData(pNewTemplateInfo);
		pData->SetItemType(TEMP_ITEM_TABLE);
		pTree->SetItemData(pTreeItem, (DWORD_PTR)pData);

		
		//LARGE_INTEGER  frequency,start,end;
		//QueryPerformanceFrequency(&frequency);
		//QueryPerformanceCounter(&start);
		
		// 创建模板表对应的树结点
		CCatalogTree::GetSingletonPtr()->BuildTemplateItem(pNewTemplateInfo, pTreeItem);

		//QueryPerformanceCounter(&end);
		//double d   =   (double)(end.QuadPart   -   start.QuadPart)   /   (double)frequency.QuadPart   *   1000.0;
		//TRACE("\n********name: 一共运行了%fms********\n", d);

	}
}

// 加载所有没有完全加载的模板数据，在生成二进制前将所有的数据全加载进来
BOOL CTemplateManager::LoadAllData()
{
	CTemplateWrite::GetSingletonPtr()->SetProcessText(_T("开始读取数据:"));
	CTemplateWrite::GetSingletonPtr()->SetProcess(0);
	float fPercent = 100.0f / (float)m_vecOriginalTemp.size();

	for (size_t i = 0; i < m_vecOriginalTemp.size(); i++)
	{
		CTemplateWrite::GetSingletonPtr()->SetProcess((int)(i * fPercent));

		COriginalTemplate* pOrigin = m_vecOriginalTemp[i];
		if (NULL == pOrigin)
		{
			return FALSE;
		}
		for (size_t j = 0; j < pOrigin->m_vecTemplate.size(); j++)
		{
			CTemplateInfo* pTemplateInfo = pOrigin->m_vecTemplate[j];
			if (NULL == pTemplateInfo)
			{
				return FALSE;
			}
			// 如果只有初始信息加载进来，就将完全信息加载进来
			// 已经完全加载的需要重新加载,以避免策划在外部修改模板数据文件
			if (pTemplateInfo->m_emTempLoadState == emLoadState_InitHead || pTemplateInfo->m_emTempLoadState == emLoadState_Inited)
			{
				TmTempLoadState emNewState = emLoadState_Initing;
				InterlockedExchange(((LONG*)&pTemplateInfo->m_emTempLoadState), emNewState);

				if(!ReadTemplate_CNext(pTemplateInfo, pTemplateInfo->m_strPathFile.GetBuffer()))
				{
					return FALSE;
				}
				emNewState = emLoadState_Inited;
				InterlockedExchange(((LONG*)&pTemplateInfo->m_emTempLoadState), emNewState);
			}
			// 出现这两个状态就是出错了
			else if (pTemplateInfo->m_emTempLoadState == emLoadState_Initing && pTemplateInfo->m_emTempLoadState == emLoadState_Null)
			{
				return FALSE;
			}

		}
	}
	return TRUE;
}

void CTemplateManager::WriteAllDirtyData()
{

	for (size_t i = 0; i < m_vecOriginalTemp.size(); i++)
	{
		COriginalTemplate* pOriginal = (COriginalTemplate*)m_vecOriginalTemp[i];
		for (size_t i = 0; i < pOriginal->m_vecTemplate.size(); i++)
		{
			CTemplateInfo* pInfo = pOriginal->m_vecTemplate[i];
			if (pInfo->m_bDirty)
			{
				WriteTemplateInfoToFile(pInfo);
			}
		}
	}
}