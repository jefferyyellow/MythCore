#include "StdAfx.h"
#include "ParseHeadFile.h"


// ��һ���ַ����н�ȡ��һ������,����ǰ��Ŀռ��TAB��
string GetFirstWord(string& strSource)
{
	int nLength = (int)strSource.length();
	if (nLength <= 0)
	{
		return "";
	}

	int nBeginIndex = 0;
	for (int i = 0; i < nLength; i++)
	{
		// ������ǿո��Ҳ���TAB��
		if(strSource[i] != 32 && strSource[i] != 9)
		{
			nBeginIndex = i;
			break;
		}
	}

	int nEndIndex = nLength;
	for (int i = nBeginIndex; i < nLength; i++)
	{
		// ����ǿո����TAB��
		if(strSource[i] == 32 || strSource[i] == 9)
		{
			nEndIndex = i;
			break;
		}
	}

	// ���ַ���ȡ����
	string strResult = strSource.substr(nBeginIndex, nEndIndex - nBeginIndex);
	// ��ȡԭ�ַ���
	if (nEndIndex >= nLength)
	{
		strSource = "";
	}
	else
	{
		strSource = strSource.substr(nEndIndex + 1);
	}
	return strResult;
}

// --------------------------------------------------------------------------------------------------------------------
CParseHeadFile::CParseHeadFile(void)
:m_nDefineLen((int)strlen("#define")),m_nEnumLen((int)strlen("enum"))
{
}

CParseHeadFile::~CParseHeadFile(void)
{
}

// ��ʼ����ģ��ͷ�ļ�
BOOL CParseHeadFile::BeginParseFile(String strFilePath)
{
	// ��ģ��ͷ�ļ�
	ifstream InFile;
	InFile.open(strFilePath.c_str());
	if (!InFile.is_open())
	{
		AfxMessageBox(_T("�Ҳ���ģ��ͷ�ļ�,ָ��·������ȷ!"));
		return FALSE;
	}


	string::size_type nIndex = 0;
	char acBuffer[emLine_Max_Char] = {0};
	
	string strLine = "";
	while(InFile.getline(acBuffer, emLine_Max_Char))
	{
		strLine = acBuffer;
		// ������
		if(( nIndex = strLine.find("#define")) != string::npos)
		{
			ParseMacro(strLine.substr(nIndex + m_nDefineLen));
		}
		// ����ȫ�ֵ�ö��
		else if((nIndex = strLine.find("enum")) != string::npos)
		{
			ParseEnum(NULL, InFile, strLine.substr(nIndex + m_nEnumLen));
		}
		// ��ʼ����һ��������ģ��
		else if ((nIndex = strLine.find("BEGINMAKE")) != string::npos)
		{
			ParseTemplate(InFile);
		}
	}
	
	InFile.close();

	//AfxMessageBox(_T("��ϲ��,���ݷ������!!!"));
	return TRUE;
}

// ������
BOOL CParseHeadFile::ParseMacro(string strLine)
{

	string strMacroName = GetFirstWord(strLine);
	string strMacroValue = GetFirstWord(strLine);
	// ����겻�����ݺ�,ֱ�ӷ���FALSE
	if (strMacroValue[0] < 0x30 || strMacroValue[0] >= 0x40)
	{
		return FALSE;
	}
	// ���������
	m_hashMacro[strMacroName] = atoi(strMacroValue.c_str());
	return TRUE;
}

// ����ö��,���pinfoΪNULL��ʾȫ��ö��,�����ΪNULL,��ʾĳһ��ģ���Ƕ��ö��
BOOL CParseHeadFile::ParseEnum(CTemplateInfo* pinfo, ifstream& InFile, string strEnumLine)
{
	string::size_type nIndex = 0;
	char acBuffer[emLine_Max_Char] = {0};
	string strLine = "";

	string strEnumName = GetFirstWord(strEnumLine);
	if (strEnumName.empty())
	{
		return FALSE;
	}

	string strValue;
	string strComment;


	CEnumInfo enumInfo;
	while(InFile.getline(acBuffer, emLine_Max_Char))
	{
		strLine = acBuffer;
		// �Ƿ��Ѿ�����ö�ٵĽ�β��
		if (strLine.find("}") != string::npos)
		{
			break;
		}
		int nValueIndex = 0;
		// ��ʾ�м�ֵ��
		if ((nValueIndex = (int)strLine.find('=')) != string::npos)
		{
			CEnumItemInfo ItemInfo;
			// +1��ʾ����"="
			ItemInfo.m_strValue = GetFirstWord(strLine.substr(nValueIndex + 1));
			
			int nCommentIndex = (int)strLine.find('/', nValueIndex + 1);
			// +2��ʾ����"//"
			ItemInfo.m_strKey = GetFirstWord(strLine.substr(nCommentIndex + 2));
			// ���ö�����һ������",",�͸�����������һ��","
			if (ItemInfo.m_strValue[ItemInfo.m_strValue.length() - 1] != ',')
			{
				ItemInfo.m_strValue.append(1, ',');
			}
			// ��ö������뵽ö����Ϣ��
			enumInfo.m_vecEnumInfo.push_back(ItemInfo);
		}
	}

	// ���뵽ö�ٱ���
	if (NULL == pinfo)
	{
		m_hashEnum[strEnumName] = enumInfo;
	}
	else
	{
		pinfo->m_hashEnum[strEnumName] = enumInfo;
	}
	
	return TRUE;
}

// ����ģ��ͷ
BOOL CParseHeadFile::ParseTemplateHead(CTemplateInfo& info, ifstream& InFile)
{
	string::size_type nIndex = 0;

	char acBuffer[emLine_Max_Char] = {0};
	string strLine;

	while(InFile.getline(acBuffer, emLine_Max_Char))
	{
		strLine = acBuffer;
		// �Ƿ���Ҫ�����
		if ((nIndex = strLine.find("virtual")) != string::npos)
		{
			nIndex = strLine.find("=", nIndex);
			string strValue = GetFirstWord(strLine.substr(nIndex + 1));
			if (strValue == "false")
			{
				info.m_bVirtual = FALSE;
			}
			else if (strValue == "true")
			{
				info.m_bVirtual = TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		// ģ����
		else if ((nIndex = strLine.find("name")) != string::npos)
		{
			nIndex = strLine.find("=", nIndex);
			info.m_strName = GetFirstWord(strLine.substr(nIndex + 1));
			if (info.m_strName == "##")
			{
				info.m_strName = "";
			}

			//// �����ǵ������
			if (info.m_strName == "(ְҵ)���Ա�")
			{
				int i = 0;
			}
		}
		// ģ��汾��
		else if ((nIndex = strLine.find("version")) != string::npos)
		{
			nIndex = strLine.find("=", nIndex);
			info.m_strVersion = GetFirstWord(strLine.substr(nIndex + 1));
			if (info.m_strVersion == "##")
			{
				info.m_strVersion = "";
			}
		}
		// ģ��·��
		else if ((nIndex = strLine.find("path")) != string::npos)
		{
			nIndex = strLine.find("=", nIndex);
			info.m_strPath = GetFirstWord(strLine.substr(nIndex + 1));
			if (info.m_strPath == "##")
			{
				info.m_strPath = "";
			}
		}
		// ��ģ������
		else if ((nIndex = strLine.find("parent")) != string::npos)
		{
			nIndex = strLine.find("=", nIndex);
			info.m_strParent = GetFirstWord(strLine.substr(nIndex + 1));
			if (info.m_strParent == "##")
			{
				info.m_strParent = "";
			}
			// ������������,��ʾͷ�Ѿ�������
			break;
		}
	}


	return TRUE;
}

// �����ظ��ֶ�,"%d"���
BOOL CParseHeadFile::ParseRepeateField(CTemplateInfo& info, string strLine)
{
	// �õ�ע�͵�%d����Ŀ
	string strCommentLine = m_RepeatvecFieldInfo[0].m_strFieldName;
	int nRepeatCount = 0;
	string::size_type nIndex = strCommentLine.find("%d");
	while (nIndex != string::npos)
	{
		nRepeatCount++;
		nIndex = strCommentLine.find("%d", nIndex + 2);
	}

	// �õ����������Ĵ������ά����ÿά�Ĵ�С
	int nRealRepeatCount = 0;
	nIndex = strLine.find("[");
	int nRepateNum[2] = {0};
	while (nIndex != string::npos)
	{
		string::size_type nEndIndex =  strLine.find("]", nIndex);
		nRepateNum[nRealRepeatCount] = GetFirstDigit(strLine.substr(nIndex + 1, nEndIndex - nIndex - 1));
		nRealRepeatCount++;
		// Ŀ¼ֻ֧�����ж�ά����
		if (nRepeatCount > 2)
		{
			AfxMessageBox(_T("��ѽ,д��ά�������ѽ!!!"));
			return FALSE;
		}

		nIndex = strLine.find("[", nIndex + 2);
	}
	// ���������Ӽ��
	// ������������ 	
	// FieldName: $�ٷ�֮%dӰ��ȵ���ʾ����
	// FieldType: STRING32
	// Type:	  EDITSTRING
	// char	mShowName[MAX_MW_PERCENT_NUM][ STRING32 ];
	//if (nRealRepeatCount  != nRepeatCount)
	//{
	//	AfxMessageBox(_T("��ѽ,������ע��д�ò�һ��!!!"));
	//	return FALSE;
	//}

	CFieldInfo fieldInfo;
	char acMaxFieldName[MAX_PATH] = {0};
	char acResultName[MAX_PATH] = {0};
	// ��ά��
	if (nRepeatCount == 2)
	{
		for (int i = 0; i < nRepateNum[0]; i++)
		{
			for (int j = 0; j < nRepateNum[1]; j++)
			{
				for (size_t k = 0; k < m_RepeatvecFieldInfo.size(); k++)
				{
					// ���ֶ�����ȡ�������¸�ʽ����Ŀ���ַ���
					strncpy(acMaxFieldName, m_RepeatvecFieldInfo[k].m_strFieldName.c_str(), MAX_PATH);
					sprintf(acResultName, acMaxFieldName, i, j);
					fieldInfo.m_strFieldName = acResultName;
					fieldInfo.m_strFieldType = m_RepeatvecFieldInfo[k].m_strFieldType;
					fieldInfo.m_strType = m_RepeatvecFieldInfo[k].m_strType;

					info.m_vecFieldInfo.push_back(fieldInfo);
				}
			}
		}
	}
	// һά��
	else 
	{
		for (int i = 0; i < nRepateNum[0]; i++)
		{
			for (size_t j = 0; j < m_RepeatvecFieldInfo.size(); j ++)
			{
				strncpy(acMaxFieldName, m_RepeatvecFieldInfo[j].m_strFieldName.c_str(), MAX_PATH);
				sprintf(acResultName, acMaxFieldName, i);
				fieldInfo.m_strFieldName = acResultName;
				fieldInfo.m_strFieldType = m_RepeatvecFieldInfo[j].m_strFieldType;
				fieldInfo.m_strType = m_RepeatvecFieldInfo[j].m_strType;
				info.m_vecFieldInfo.push_back(fieldInfo);
			}
		}
	}
		
	return TRUE;
}



//չ��Ƕ��ģ���ֶ�
BOOL CParseHeadFile::UnfoldNestTemplate(CTemplateInfo& rRoot, CTemplateInfo* info, string strline)
{
	int dimensional = 0;		//����ά��
	CTemplateInfo *iNestTp;
	string strName = strline;
	string nestTpName = GetFirstWord(strName);
	int iDIm[2];
	int iposb = 0, ipose = 0, ipos = 0;

	ipos = strline.find("[");
	while(ipos != string::npos)
	{
		dimensional++;
		ipos = strline.find("[", ipos+2);
	}

	nestTempMap::iterator it = rRoot.m_nestTemplates.find(nestTpName);
	if(it == rRoot.m_nestTemplates.end())
	{
		return FALSE;//δ�ҵ������ͷ��أ���Ҫ�ܿ���������������ɵĻ���
	}
	iNestTp = it->second;
	for(int i = 0; i < dimensional; i++)
	{
		iposb = strline.find("[", iposb+1)+1;
		ipose = strline.find("]", iposb);
		string strCount = strline.substr(iposb, ipose - iposb);
		iDIm[i] = GetFirstDigit(strCount);
	}


	char acMaxFieldName[MAX_PATH];
	char acResultName[MAX_PATH];
	CFieldInfo fieldInfo;

	if(dimensional == 0)
	{
		for(int i = 0; i < iNestTp->m_vecFieldInfo.size(); i++)
		{
			strncpy(acResultName, info->m_groupfield.m_strFieldName.c_str(), MAX_PATH);
			strcat(acResultName, iNestTp->m_vecFieldInfo[i].m_strFieldName.c_str());
			fieldInfo.m_strFieldName = acResultName;
			fieldInfo.m_strFieldType = iNestTp->m_vecFieldInfo[i].m_strFieldType;
			fieldInfo.m_strType = iNestTp->m_vecFieldInfo[i].m_strType;
			int nParaSignPos = fieldInfo.m_strFieldName.find('$');
			if (nParaSignPos != string::npos && nParaSignPos != 0)
			{
				fieldInfo.m_strFieldName.erase(nParaSignPos, 1);
				fieldInfo.m_strFieldName.insert(0, "$");
			}
			info->m_vecFieldInfo.push_back(fieldInfo);
		}
	}

	if(dimensional == 1)
	{
		for(int i = 0; i < iDIm[0]; i++)
		{
			for(int j = 0; j < iNestTp->m_vecFieldInfo.size(); j++)
			{
				strncpy(acMaxFieldName, info->m_groupfield.m_strFieldName.c_str(), MAX_PATH);
				strcat(acMaxFieldName, iNestTp->m_vecFieldInfo[j].m_strFieldName.c_str());
				sprintf(acResultName, acMaxFieldName, i);
				fieldInfo.m_strFieldName = acResultName;
				fieldInfo.m_strFieldType = iNestTp->m_vecFieldInfo[j].m_strFieldType;
				fieldInfo.m_strType = iNestTp->m_vecFieldInfo[j].m_strType;
				int nParaSignPos = fieldInfo.m_strFieldName.find('$');
				if (nParaSignPos != string::npos && nParaSignPos != 0)
				{
					fieldInfo.m_strFieldName.erase(nParaSignPos, 1);
					fieldInfo.m_strFieldName.insert(0, "$");
				}
				info->m_vecFieldInfo.push_back(fieldInfo);
			}
		}
	}
	else if(dimensional == 2)
	{
		for(int i = 0; i < iDIm[0]; i++)
		{
			for(int j = 0; j < iDIm[1]; j++)
			{
				for(int k = 0; k < iNestTp->m_vecFieldInfo.size(); k++)
				{
					strncpy(acMaxFieldName, info->m_groupfield.m_strFieldName.c_str(), MAX_PATH);
					strcat(acMaxFieldName, iNestTp->m_vecFieldInfo[k].m_strFieldName.c_str());
					sprintf(acResultName, acMaxFieldName, i, j);
					fieldInfo.m_strFieldName = acResultName;
					fieldInfo.m_strFieldType = iNestTp->m_vecFieldInfo[k].m_strFieldType;
					fieldInfo.m_strType = iNestTp->m_vecFieldInfo[k].m_strType;
					int nParaSignPos = fieldInfo.m_strFieldName.find('$');
					if (nParaSignPos != string::npos && nParaSignPos != 0)
					{
						fieldInfo.m_strFieldName.erase(nParaSignPos, 1);
						fieldInfo.m_strFieldName.insert(0, "$");
					}
					info->m_vecFieldInfo.push_back(fieldInfo);
				}
			}
		}
	}
	return TRUE;
}

// ����ģ����
BOOL CParseHeadFile::ParseTemplateBody(CTemplateInfo& rRoot, CTemplateInfo& info, ifstream& InFile, int nRepatedNum1, int nRepeatedNum2)
{
	string::size_type nIndex = 0;
	char acBuffer[emLine_Max_Char] = {0};
	string strLine;
	string strclassName;

	CFieldInfo fieldInfo;
	// �Ƿ����ظ��ֶ�
	BOOL bRepeat = FALSE;
	// �Ƿ��Ѿ�����������,�����ж��Ƿ�����ö�ٻ���ȫ��ö��
	static int bInClass = 0;
	//�Ƿ�������������
	BOOL bUnfold = FALSE;

	while(InFile.getline(acBuffer, emLine_Max_Char))
	{
		strLine = acBuffer;

		//����Ƕ��ģ�崦��
		if((nIndex = strLine.find("struct")) != string::npos || 
			(nIndex = strLine.find("class")) != string::npos)
		{
			bInClass++;

			if(bInClass > 1)
			{
				string nestTpName = GetFirstWord(strLine.substr(nIndex + 6));
				CTemplateInfo *iNestTemp = new CTemplateInfo();
				if(ParseTemplateBody(rRoot, *iNestTemp, InFile))	//�ݹ�Ƕ�״�����������֮ǰ��ʽ������ȡ�������ֶΣ���������
				{
					std::pair<string, CTemplateInfo*> pa(nestTpName, iNestTemp);
					rRoot.m_nestTemplates.insert(pa);
				}
			}
			continue;
		}

		//һ����Ľ���
		else if(strLine.find("};") != string::npos)
		{
			bInClass --;
			if(bInClass <= 0)
			{
				continue;
			}
			else if(info.m_vecFieldInfo.empty())
			{
				return FALSE;
			}
			else
			{
				return TRUE;
			}
		}
		
		// �ֶ���
		if ((nIndex = strLine.find("FieldName")) != string::npos)
		{
			nIndex = strLine.find(":", nIndex);
			if (nIndex != string::npos)
			{
				fieldInfo.m_strFieldName = GetFirstWord(strLine.substr(nIndex + 1));
			}
			if((nIndex = strLine.find("%d")) !=  string::npos)
			{
				bRepeat = TRUE;
			}
			continue;
		}
		// �ֶ�ֵ����
		else if ((nIndex = strLine.find("FieldType")) != string::npos)
		{
			nIndex = strLine.find(":", nIndex);
			if (nIndex != string::npos)
			{
				fieldInfo.m_strFieldType = GetFirstWord(strLine.substr(nIndex + 1));
			}
			continue;
		}
		// �ֶ�����༭����
		else if (((nIndex = strLine.find("Type")) != string::npos) && ((nIndex = strLine.find(":", nIndex)) != string::npos))
		{
			//nIndex = strLine.find(":", nIndex);
			if (nIndex != string::npos)
			{
				fieldInfo.m_strType = GetFirstWord(strLine.substr(nIndex + 1));
				if(fieldInfo.m_strType == "CLASS")		//��Ҫ��Ƕ�����ж�ȡ�ֶΣ���չ��
				{
					info.m_groupfield = fieldInfo;
					bRepeat = FALSE;
					bUnfold = TRUE;
					continue;
				}
				// ��ʾ�Ѿ�����һ���ֶ�
				if (bRepeat)
				{
					m_RepeatvecFieldInfo.push_back(fieldInfo);
				}
				else
				{
					info.m_vecFieldInfo.push_back(fieldInfo);
				}
			}
			continue;
		}
		// ��Ƕ��ö��
		else if((nIndex = strLine.find("enum")) != string::npos)
		{
			// �Ƿ���������,�ھ���Ƕ�׵�,���ھ���ȫ�ֵ�
			if (bInClass)
			{			
				ParseEnum(&info, InFile, strLine.substr(nIndex + m_nEnumLen));
			}
			else
			{
				ParseEnum(NULL, InFile, strLine.substr(nIndex + m_nEnumLen));
			}
			continue;
		}
		//չ��Ƕ��ģ��
		else if(bUnfold)
		{
			string strTemp = strLine;
			strclassName = GetFirstWord(strTemp);

			nestTempMap::iterator it = rRoot.m_nestTemplates.find(strclassName);
			if(it != rRoot.m_nestTemplates.end())
			{
				UnfoldNestTemplate(rRoot, &info, strLine);
			}

			bUnfold = FALSE;
			continue;
		}
		// �Ѿ�����repeated�ľ�ͷ��
		else if (strLine.find("[") != string::npos)
		{
			if(bRepeat)								
			{
				ParseRepeateField(info, strLine);	//����֮ǰ��ʽ
				m_RepeatvecFieldInfo.clear();
				bRepeat = FALSE;
			}
			continue;
		}
		// �Ѿ���һ����Ľ�����
		else if (strLine.find("ENDMAKE") != string::npos)
		{
			bInClass = 0;
			ReleaseTp(info);
			return TRUE;
		}

	}
	return TRUE;
}

void CParseHeadFile::ReleaseTp(CTemplateInfo &info)
{
	//ɾ��Ƕ��ģ����ռ�ռ�
	if(info.m_nestTemplates.empty())
	{
		return;
	}
	nestTempMap::iterator itBegin = info.m_nestTemplates.begin();
	for(;itBegin != info.m_nestTemplates.end();itBegin++)
	{
		if(itBegin->second->m_nestTemplates.empty())
		{
			delete itBegin->second;
		}
		else
		{
			ReleaseTp(*(itBegin->second));
		}
	}
	info.m_nestTemplates.clear();
}

// ����ģ��
BOOL CParseHeadFile::ParseTemplate(ifstream& InFile)
{

	char acBuffer[emLine_Max_Char] = {0};
	string strLine;

	CTemplateInfo templateInfo;
	// ����ͷ
	ParseTemplateHead(templateInfo, InFile);
	// ����ģ����
	ParseTemplateBody(templateInfo, templateInfo, InFile);
	m_vecTemplate.push_back(templateInfo);
	return TRUE;
}

// ���ַ����ĵ�һ���ǿո���TAB��ʼ�Ĵ�ת��������
int CParseHeadFile::GetFirstDigit(string& strSource)
{
	string strDigit = GetFirstWord(strSource);

	// �����
	hash_map<string, int>::iterator it = m_hashMacro.find(strDigit);
	if (it != m_hashMacro.end())
	{
		return it->second;
	}
	// ���Ǻ�Ӧ�þ���ֱ�ӵ���ֵ
	if (strDigit.length() < 0)
	{
		return 0;
	}
	if (strDigit[0] < 0x30 || strDigit[0] >= 0x40)
	{
		return 0;
	}

	return atoi(strDigit.c_str());
}

// �ӵ�ǰ��ģ���༰�丸�������ö����Ϣ
BOOL CParseHeadFile::GetEnumInfo(CTemplateInfo& rInfo, string strEnum, hash_map<string, CEnumInfo>::iterator& itResult)
{
	// ����ȫ��ö����Ϣ
	hash_map<string, CEnumInfo>::iterator it = rInfo.m_hashEnum.find(strEnum);
	if (it == rInfo.m_hashEnum.end())
	{
		// ����и�ģ��,���Ҹ�ģ���Ƕ��ö��
		if (rInfo.m_strParent != "")
		{
			size_t nParentIndex = 0;
			for (size_t i = 0; i < m_vecTemplate.size(); i++)
			{
				if (m_vecTemplate[i].m_strName == rInfo.m_strParent)
				{
					nParentIndex = i;
				}
			}

			CTemplateInfo& rParentInfo = m_vecTemplate[nParentIndex];
			return GetEnumInfo(rParentInfo, strEnum, itResult);
		}

	}
	else
	{
		itResult = it;
		return TRUE;
	}
	return FALSE;
}

// ��ȫ��ö����Ϣ�����,����Ҳ��ž������е�ģ���Ƕ��ö����Ϣ
BOOL CParseHeadFile::GetEnumInfo(string strEnum, hash_map<string, CEnumInfo>::iterator& itResult)
{
	// ����ȫ�ֵ�
	hash_map<string, CEnumInfo>::iterator it = m_hashEnum.find(strEnum);
	if (it == m_hashEnum.end())
	{
		// �������е�ģ��Ƕ��ö����Ϣ
		for (size_t i = 0; i < m_vecTemplate.size(); i++)
		{
			it = m_vecTemplate[i].m_hashEnum.find(strEnum);
			if (it != m_vecTemplate[i].m_hashEnum.end())
			{
				itResult = it;
				return TRUE;
			}
		}
	}
	else
	{
		itResult = it;
		return TRUE;
	}
	
	return FALSE;
}
