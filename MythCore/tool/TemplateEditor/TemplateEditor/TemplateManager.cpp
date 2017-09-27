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

// ��ģ���
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


	// ���û��ģ�����ݾ�ֱ�ӳ�ʼ�������
	if (pTemplateinfo->m_vecTemplateFieldData.size() <= 0)
	{
		pTemplateinfo->m_emTempLoadState = emLoadState_Inited;
	}

	pOriginTemp->m_vecTemplate.push_back(pTemplateinfo);
	return pTemplateinfo;
}

// ��ԭʼģ��
COriginalTemplate* CTemplateManager::ReadOriginTemplate(CString strPath, CString strOriginName)
{
	ifstream InFile;
	InFile.open(strPath.GetBuffer());
	if (!InFile.is_open())
	{
		// �����
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

	// һ��һ�з���
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
		// һ���ֶν�����
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

// ��ģ��
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

	//�����ڴ�
	char* pMemory = new char[nFileSize+1];
	fread(pMemory, 1, nFileSize, pFile);
	pMemory[nFileSize] = 0;

	// ���ڼ�¼��ͷ��ԭʼģ���ֶεĶ�Ӧ
	hash_map<size_t, size_t>		hashFieldIndex;

	// ����ģ��ͷ
	char* pNext = ParseTemplateHead_C(pTemplateInfo, hashFieldIndex, pMemory, pMemory + nFileSize + 1);
	// ����ģ����
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

	//�����ڴ�
	char* pMemory = new char[nFileSize+1];
	fread(pMemory, 1, nFileSize, pFile);
	pMemory[nFileSize] = 0;

	// ���ڼ�¼��ͷ��ԭʼģ���ֶεĶ�Ӧ
	hash_map<size_t, size_t>		hashFieldIndex;
	char* pNext = ParseTemplateHead_C(pTemplateInfo, hashFieldIndex, pMemory, pMemory + nFileSize + 1);

	ParseTemplateBody_CNext(pTemplateInfo, hashFieldIndex, pNext, pMemory + nFileSize + 1);
	delete []pMemory;
	fclose(pFile);
	return TRUE;
}

// ����ģ���ͷ
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
	// ��¼�Ѿ�ƥ�䵽ͷ���Ǹ��ֶ���
	int nFieldRecorder = 0;
	//char* pWorldEnd = 0;
	for (; pStart < pFileEnd; pStart++)
	{
		if ((*pStart=='\r' || *pStart=='\n'))
		{
			*pStart = '\0';
			break;
		}
		
		// TAB��
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

			// ����Ӽ�¼����������û��������,ֻ���ٴ�ͷ������
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
			// ע��,������ܸ�ֵ��,û�����ж��Ƿ�Ϸ���
			pWorldStart = pStart + 1;
			continue;
		}
	}

	// ��ͷ��ԭʼģ���Ӧ
	for (int i = nFieldRecorder; i < (int)OriginalField.size(); i++)
	{
		if (strcmp(pWorldStart, OriginalField[i].m_strFieldName.c_str()) == 0)
		{
			hashFieldIndex[nFieldCount] = i;
			nFieldRecorder = i;
			break;
		}
	}
	// ���˻س��ͻ��з�
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

// �����������ֶ�����
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
		// �µ�һ��
		if ((*pStart=='\r' || *pStart=='\n'))
		{
			*pStart = '\0';

			hash_map<size_t, size_t>::iterator it = hashFieldIndex.find(nFieldCount);
			if (it != hashFieldIndex.end())
			{
				pTemplateFieldData->m_vecValue[it->second] = pWorldStart;
			}
			
			// ���˵��س��ͻ��з�
			for (pStart++; pStart < pFileEnd; pStart++)
			{
				if ((*pStart=='\r' || *pStart=='\n'))
				{
					continue;
				}
				break;
			}

			
			// ��������һ���ǿ��м�����ǿ��е����
			nFieldCount = 0;
			pTemplateFieldData->m_emTempLoadState = emLoadState_Inited;
			if (pStart < pFileEnd -1)
			{
				nTemplateCount++;
				pTemplateFieldData = pTemplateInfo->m_vecTemplateFieldData[nTemplateCount];
				pWorldStart = pStart;
			}


		}

		// TAB��
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
			// ע��,������ܸ�ֵ��,û�����ж��Ƿ�Ϸ���
			pWorldStart = pStart + 1;
			continue;
		}
	}

	// �Ƿ�����һ��û����ģ���������һ���ǿ��м�����ǿ��е����
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

// ����ģ����
BOOL CTemplateManager::ParseTemplateBody_C(CTemplateInfo* pTemplateInfo, hash_map<size_t, size_t>& hashFieldIndex, char* pFileStart, char* pFileEnd)
{
	if (hashFieldIndex.size() <= 0)
	{
		return FALSE;
	}

	// ����һ��ģ������
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
		// �µ�һ�У���ʾһ������һ���µ�ģ��������
		if ((*pStart=='\r' || *pStart=='\n'))
		{
			*pStart = '\0';

			hash_map<size_t, size_t>::iterator it = hashFieldIndex.find(nFieldCount);
			if (it != hashFieldIndex.end())
			{
				pTemplateFieldData->m_vecValue[it->second] = pWorldStart;
			}
			
			// ���˵��س��ͻ���
			for (pStart++; pStart < pFileEnd; pStart++)
			{
				if ((*pStart=='\r' || *pStart=='\n'))
				{
					continue;
				}
				break;
			}

			// ����������
			pTemplateInfo->m_vecTemplateFieldData.push_back(pTemplateFieldData);
			AddNewTemplateField(pTemplateFieldData);

			
			nFieldCount = 0;
			// ��������һ���ǿ��м�����ǿ��е����
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

		// TAB��
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
			// ֻ��ģ��ID��ģ�����֣���������Ϣ�����ٶ�
			if (nFieldCount == 0)
			{
				pTemplateFieldData->m_nTemplateID = atoi(pWorldStart);
				if (pTemplateFieldData->m_nTemplateID <= 0)
				{
					AfxMessageBox(_T("С�ڻ��ߵ������ID����,����!"));
					return FALSE;
				}

				if (CTemplateManager::GetSingletonPtr()->IsExistTemplate(pTemplateFieldData->m_nTemplateID))
				{
					TCHAR acTempID[MAX_PATH] = {0};
					_snwprintf(acTempID, MAX_PATH, _T("�Ѿ�����ͬ��ģ��ID����,���ݳ������˳��༭�����������Ա���, ģ��ID:%d"), pTemplateFieldData->m_nTemplateID);
					AfxMessageBox(acTempID);
					return FALSE;
				}
			}
			else if (nFieldCount == 1)
			{
				pTemplateFieldData->m_strTemplateName = pWorldStart;
			}
			

			nFieldCount++;
			// ע��,������ܸ�ֵ��,û�����ж��Ƿ�Ϸ���
			pWorldStart = pStart + 1;
			continue;
		}
	}
	
	// �Ƿ�����һ��û����ģ���������һ���ǿ��м�����ǿ��е����
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
//		// �����
//		return FALSE;
//	}
//
//	// ���ڼ�¼��ͷ��ԭʼģ���ֶεĶ�Ӧ
//	hash_map<size_t, size_t>		hashFieldIndex;
//	if(getline(InFile,strLine))
//	{
//		// ����һ��,������ͷ
//		ParseTemplateHead(pTemplateInfo, hashFieldIndex, strLine);
//	}
//	while(getline(InFile,strLine))
//	{	
//		// ��������
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

// ͨ��ģ���õ���ԭʼģ��
COriginalTemplate* CTemplateManager::GetOriginalTemplate(CTemplateInfo* pTemplateInfo)
{
	if (NULL == pTemplateInfo)
	{
		return NULL;
	}

	return pTemplateInfo->m_pParent;
}

// ͨ�����ֵõ�ԭʼģ��
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

// ����ԭʼģ�������ֶεĴ�С
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
		// ����INT2 UINT2
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

// �õ�ģ�����ֶ���Ŀ
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

// ��ģ�����Ϣд���ļ�
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
	// д��ͷ
	WriteTemplateInfoHead(GetOriginalTemplate(pTemplateInfo), pFile);
	// д����
	WriteTemplateInfoBody(pTemplateInfo, pFile);
	pTemplateInfo->m_bDirty = FALSE;
	fclose(pFile);
	return TRUE;
}

// дģ���ͷ
BOOL CTemplateManager::WriteTemplateInfoHead(COriginalTemplate* pOrigin, FILE* pFile)
{
	fwrite("ģ��ID", strlen("ģ��ID"), 1, pFile);
	fwrite("	", 1, 1, pFile);
	fwrite("ģ������", strlen("ģ������"), 1, pFile);

	size_t nSize = pOrigin->m_vecOriginalField.size();
	for (size_t i = 0; i < nSize; i++)
	{
		fwrite("	", 1, 1, pFile);
		string& strFieldName = pOrigin->m_vecOriginalField[i].m_strFieldName;
		fwrite(strFieldName.c_str(), strFieldName.size(), 1, pFile);
	}
	return TRUE;
}

// дģ�����
BOOL CTemplateManager::WriteTemplateInfoBody(CTemplateInfo* pTemplateInfo, FILE* pFile)
{
	char acBuffer[MAX_PATH] = {0};
	for (size_t i = 0; i < pTemplateInfo->m_vecTemplateFieldData.size(); i++)
	{
		fwrite("\n", 1, 1, pFile);
		CTemplateFieldData* pFieldData = pTemplateInfo->m_vecTemplateFieldData[i];
		if (NULL == pFieldData)
		{
			// �����
			return FALSE;
		}

		memset(acBuffer, 0, sizeof(acBuffer));
		// дģ��ID
		_itoa(pFieldData->m_nTemplateID, acBuffer, 10);
		fwrite(acBuffer, strlen(acBuffer), 1, pFile);
		fwrite("	", 1, 1, pFile);
		// дģ������
		fwrite(pFieldData->m_strTemplateName.c_str(), pFieldData->m_strTemplateName.size(), 1, pFile);

		// д��ģ���ֶ�����
		for (size_t j = 0; j < pFieldData->m_vecValue.size(); j++)
		{
			fwrite("	", 1, 1, pFile);
			string& strValue = pFieldData->m_vecValue[j];
			fwrite(strValue.c_str(), strValue.size(), 1, pFile);
		}
	}

	return TRUE;
}

// ����һ���µ�ģ��
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
// �����ݹ鴴��ģ������
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

// �����ڵ㴴��ģ������
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
		// �������ڵ�����
		CTreeItemData* pData = new CTreeItemData;
		if (pData == NULL)
		{
			// �����
			return;
		}
		pData->SetData(NULL);
		pData->SetItemType(TEMP_ITEM_DIR);
		pTree->SetItemData(pTreeItem, (DWORD_PTR)pData);
		// �ݹ鴴��
		BuildTempFromTree(pOriginal, strPath.GetBuffer(), pTree, pTreeItem);
	}
	// ԭʼģ��
	else if (dwData == TEMP_ITEM_ORIGIN)
	{
		CString strOriginFile = strPath + pTree->GetItemText(pTreeItem) + CString("\\#") + pTree->GetItemText(pTreeItem) + CString(".txt");
		strPath += pTree->GetItemText(pTreeItem) + + CString("\\");

		COriginalTemplate* pOriginTemp = ReadOriginTemplate(strOriginFile, pTree->GetItemText(pTreeItem));
		if (pOriginTemp == NULL)
		{
			// �����
			return;
		}

		// �������ڵ�����
		CTreeItemData* pData = new CTreeItemData;
		if (pData == NULL)
		{
			// �����
			return;
		}
		pData->SetData(pOriginTemp);
		pData->SetItemType(TEMP_ITEM_ORIGIN);
		pTree->SetItemData(pTreeItem, (DWORD_PTR)pData);
		// �ݹ�
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
			_snwprintf(acBuffer, MAX_PATH, _T("��ģ������:%s"), strTableFile);
			// �����
			AfxMessageBox(acBuffer);
			exit(1);
			return ;
		}

		// �������ڵ�����
		CTreeItemData* pData = new CTreeItemData;
		if (pData == NULL)
		{
			// �����
			return;
		}
		pData->SetData(pNewTemplateInfo);
		pData->SetItemType(TEMP_ITEM_TABLE);
		pTree->SetItemData(pTreeItem, (DWORD_PTR)pData);

		
		//LARGE_INTEGER  frequency,start,end;
		//QueryPerformanceFrequency(&frequency);
		//QueryPerformanceCounter(&start);
		
		// ����ģ����Ӧ�������
		CCatalogTree::GetSingletonPtr()->BuildTemplateItem(pNewTemplateInfo, pTreeItem);

		//QueryPerformanceCounter(&end);
		//double d   =   (double)(end.QuadPart   -   start.QuadPart)   /   (double)frequency.QuadPart   *   1000.0;
		//TRACE("\n********name: һ��������%fms********\n", d);

	}
}

// ��������û����ȫ���ص�ģ�����ݣ������ɶ�����ǰ�����е�����ȫ���ؽ���
BOOL CTemplateManager::LoadAllData()
{
	CTemplateWrite::GetSingletonPtr()->SetProcessText(_T("��ʼ��ȡ����:"));
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
			// ���ֻ�г�ʼ��Ϣ���ؽ������ͽ���ȫ��Ϣ���ؽ���
			// �Ѿ���ȫ���ص���Ҫ���¼���,�Ա���߻����ⲿ�޸�ģ�������ļ�
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
			// ����������״̬���ǳ�����
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