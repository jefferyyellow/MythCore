#include "StdAfx.h"
#include "ParseHeadFile.h"


// 从一个字符串中截取第一个单词,过滤前面的空间和TAB键
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
		// 如果不是空格且不是TAB键
		if(strSource[i] != 32 && strSource[i] != 9)
		{
			nBeginIndex = i;
			break;
		}
	}

	int nEndIndex = nLength;
	for (int i = nBeginIndex; i < nLength; i++)
	{
		// 如果是空格或者TAB键
		if(strSource[i] == 32 || strSource[i] == 9)
		{
			nEndIndex = i;
			break;
		}
	}

	// 将字符串取出来
	string strResult = strSource.substr(nBeginIndex, nEndIndex - nBeginIndex);
	// 截取原字符串
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

// 开始分析模板头文件
BOOL CParseHeadFile::BeginParseFile(String strFilePath)
{
	// 打开模板头文件
	ifstream InFile;
	InFile.open(strFilePath.c_str());
	if (!InFile.is_open())
	{
		AfxMessageBox(_T("找不着模板头文件,指定路径不正确!"));
		return FALSE;
	}


	string::size_type nIndex = 0;
	char acBuffer[emLine_Max_Char] = {0};
	
	string strLine = "";
	while(InFile.getline(acBuffer, emLine_Max_Char))
	{
		strLine = acBuffer;
		// 分析宏
		if(( nIndex = strLine.find("#define")) != string::npos)
		{
			ParseMacro(strLine.substr(nIndex + m_nDefineLen));
		}
		// 分析全局的枚举
		else if((nIndex = strLine.find("enum")) != string::npos)
		{
			ParseEnum(NULL, InFile, strLine.substr(nIndex + m_nEnumLen));
		}
		// 开始分析一个单独的模板
		else if ((nIndex = strLine.find("BEGINMAKE")) != string::npos)
		{
			ParseTemplate(InFile);
		}
	}
	
	InFile.close();

	//AfxMessageBox(_T("恭喜你,数据分析完成!!!"));
	return TRUE;
}

// 分析宏
BOOL CParseHeadFile::ParseMacro(string strLine)
{

	string strMacroName = GetFirstWord(strLine);
	string strMacroValue = GetFirstWord(strLine);
	// 如果宏不是数据宏,直接返回FALSE
	if (strMacroValue[0] < 0x30 || strMacroValue[0] >= 0x40)
	{
		return FALSE;
	}
	// 将其加入宏表
	m_hashMacro[strMacroName] = atoi(strMacroValue.c_str());
	return TRUE;
}

// 分析枚举,如果pinfo为NULL表示全局枚举,如果不为NULL,表示某一个模板的嵌套枚举
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
		// 是否已经到了枚举的结尾处
		if (strLine.find("}") != string::npos)
		{
			break;
		}
		int nValueIndex = 0;
		// 表示有键值对
		if ((nValueIndex = (int)strLine.find('=')) != string::npos)
		{
			CEnumItemInfo ItemInfo;
			// +1表示跳过"="
			ItemInfo.m_strValue = GetFirstWord(strLine.substr(nValueIndex + 1));
			
			int nCommentIndex = (int)strLine.find('/', nValueIndex + 1);
			// +2表示跳过"//"
			ItemInfo.m_strKey = GetFirstWord(strLine.substr(nCommentIndex + 2));
			// 如果枚举最后一个不是",",就给他在最后加上一个","
			if (ItemInfo.m_strValue[ItemInfo.m_strValue.length() - 1] != ',')
			{
				ItemInfo.m_strValue.append(1, ',');
			}
			// 将枚举项加入到枚举信息中
			enumInfo.m_vecEnumInfo.push_back(ItemInfo);
		}
	}

	// 加入到枚举表中
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

// 分析模板头
BOOL CParseHeadFile::ParseTemplateHead(CTemplateInfo& info, ifstream& InFile)
{
	string::size_type nIndex = 0;

	char acBuffer[emLine_Max_Char] = {0};
	string strLine;

	while(InFile.getline(acBuffer, emLine_Max_Char))
	{
		strLine = acBuffer;
		// 是否需要输入表
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
		// 模板名
		else if ((nIndex = strLine.find("name")) != string::npos)
		{
			nIndex = strLine.find("=", nIndex);
			info.m_strName = GetFirstWord(strLine.substr(nIndex + 1));
			if (info.m_strName == "##")
			{
				info.m_strName = "";
			}

			//// 下面是调试语句
			if (info.m_strName == "(职业)属性表")
			{
				int i = 0;
			}
		}
		// 模板版本号
		else if ((nIndex = strLine.find("version")) != string::npos)
		{
			nIndex = strLine.find("=", nIndex);
			info.m_strVersion = GetFirstWord(strLine.substr(nIndex + 1));
			if (info.m_strVersion == "##")
			{
				info.m_strVersion = "";
			}
		}
		// 模板路径
		else if ((nIndex = strLine.find("path")) != string::npos)
		{
			nIndex = strLine.find("=", nIndex);
			info.m_strPath = GetFirstWord(strLine.substr(nIndex + 1));
			if (info.m_strPath == "##")
			{
				info.m_strPath = "";
			}
		}
		// 父模板名字
		else if ((nIndex = strLine.find("parent")) != string::npos)
		{
			nIndex = strLine.find("=", nIndex);
			info.m_strParent = GetFirstWord(strLine.substr(nIndex + 1));
			if (info.m_strParent == "##")
			{
				info.m_strParent = "";
			}
			// 从这里跳出来,表示头已经读完了
			break;
		}
	}


	return TRUE;
}

// 分析重复字段,"%d"情况
BOOL CParseHeadFile::ParseRepeateField(CTemplateInfo& info, string strLine)
{
	// 得到注释的%d的数目
	string strCommentLine = m_RepeatvecFieldInfo[0].m_strFieldName;
	int nRepeatCount = 0;
	string::size_type nIndex = strCommentLine.find("%d");
	while (nIndex != string::npos)
	{
		nRepeatCount++;
		nIndex = strCommentLine.find("%d", nIndex + 2);
	}

	// 得到真正声明的处数组的维数及每维的大小
	int nRealRepeatCount = 0;
	nIndex = strLine.find("[");
	int nRepateNum[2] = {0};
	while (nIndex != string::npos)
	{
		string::size_type nEndIndex =  strLine.find("]", nIndex);
		nRepateNum[nRealRepeatCount] = GetFirstDigit(strLine.substr(nIndex + 1, nEndIndex - nIndex - 1));
		nRealRepeatCount++;
		// 目录只支持运行二维数组
		if (nRepeatCount > 2)
		{
			AfxMessageBox(_T("二呀,写三维数组干嘛呀!!!"));
			return FALSE;
		}

		nIndex = strLine.find("[", nIndex + 2);
	}
	// 不能这样子检查
	// 会出现这种情况 	
	// FieldName: $百分之%d影响度的显示名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	// char	mShowName[MAX_MW_PERCENT_NUM][ STRING32 ];
	//if (nRealRepeatCount  != nRepeatCount)
	//{
	//	AfxMessageBox(_T("二呀,声明与注释写得不一样!!!"));
	//	return FALSE;
	//}

	CFieldInfo fieldInfo;
	char acMaxFieldName[MAX_PATH] = {0};
	char acResultName[MAX_PATH] = {0};
	// 二维的
	if (nRepeatCount == 2)
	{
		for (int i = 0; i < nRepateNum[0]; i++)
		{
			for (int j = 0; j < nRepateNum[1]; j++)
			{
				for (size_t k = 0; k < m_RepeatvecFieldInfo.size(); k++)
				{
					// 将字段名字取出来重新格式化成目标字符串
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
	// 一维的
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



//展开嵌套模板字段
BOOL CParseHeadFile::UnfoldNestTemplate(CTemplateInfo& rRoot, CTemplateInfo* info, string strline)
{
	int dimensional = 0;		//数组维度
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
		return FALSE;//未找到的类型返回，主要避开基本数据类型造成的混淆
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

// 分析模板体
BOOL CParseHeadFile::ParseTemplateBody(CTemplateInfo& rRoot, CTemplateInfo& info, ifstream& InFile, int nRepatedNum1, int nRepeatedNum2)
{
	string::size_type nIndex = 0;
	char acBuffer[emLine_Max_Char] = {0};
	string strLine;
	string strclassName;

	CFieldInfo fieldInfo;
	// 是否是重复字段
	BOOL bRepeat = FALSE;
	// 是否已经进入类声明,用于判断是否是类枚举还是全局枚举
	static int bInClass = 0;
	//是否是类类型数组
	BOOL bUnfold = FALSE;

	while(InFile.getline(acBuffer, emLine_Max_Char))
	{
		strLine = acBuffer;

		//进入嵌套模板处理
		if((nIndex = strLine.find("struct")) != string::npos || 
			(nIndex = strLine.find("class")) != string::npos)
		{
			bInClass++;

			if(bInClass > 1)
			{
				string nestTpName = GetFirstWord(strLine.substr(nIndex + 6));
				CTemplateInfo *iNestTemp = new CTemplateInfo();
				if(ParseTemplateBody(rRoot, *iNestTemp, InFile))	//递归嵌套处理函数，兼容之前格式，若读取类中无字段，不做操作
				{
					std::pair<string, CTemplateInfo*> pa(nestTpName, iNestTemp);
					rRoot.m_nestTemplates.insert(pa);
				}
			}
			continue;
		}

		//一个类的结束
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
		
		// 字段名
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
		// 字段值类型
		else if ((nIndex = strLine.find("FieldType")) != string::npos)
		{
			nIndex = strLine.find(":", nIndex);
			if (nIndex != string::npos)
			{
				fieldInfo.m_strFieldType = GetFirstWord(strLine.substr(nIndex + 1));
			}
			continue;
		}
		// 字段数组编辑类型
		else if (((nIndex = strLine.find("Type")) != string::npos) && ((nIndex = strLine.find(":", nIndex)) != string::npos))
		{
			//nIndex = strLine.find(":", nIndex);
			if (nIndex != string::npos)
			{
				fieldInfo.m_strType = GetFirstWord(strLine.substr(nIndex + 1));
				if(fieldInfo.m_strType == "CLASS")		//需要从嵌套类中读取字段，并展开
				{
					info.m_groupfield = fieldInfo;
					bRepeat = FALSE;
					bUnfold = TRUE;
					continue;
				}
				// 表示已经读完一个字段
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
		// 内嵌的枚举
		else if((nIndex = strLine.find("enum")) != string::npos)
		{
			// 是否在类里面,在就是嵌套的,不在就是全局的
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
		//展开嵌套模板
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
		// 已经到了repeated的尽头了
		else if (strLine.find("[") != string::npos)
		{
			if(bRepeat)								
			{
				ParseRepeateField(info, strLine);	//兼容之前格式
				m_RepeatvecFieldInfo.clear();
				bRepeat = FALSE;
			}
			continue;
		}
		// 已经是一个块的结束了
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
	//删除嵌套模板所占空间
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

// 分析模板
BOOL CParseHeadFile::ParseTemplate(ifstream& InFile)
{

	char acBuffer[emLine_Max_Char] = {0};
	string strLine;

	CTemplateInfo templateInfo;
	// 分析头
	ParseTemplateHead(templateInfo, InFile);
	// 分析模板体
	ParseTemplateBody(templateInfo, templateInfo, InFile);
	m_vecTemplate.push_back(templateInfo);
	return TRUE;
}

// 将字符串的第一个非空格与TAB开始的串转换成数字
int CParseHeadFile::GetFirstDigit(string& strSource)
{
	string strDigit = GetFirstWord(strSource);

	// 处理宏
	hash_map<string, int>::iterator it = m_hashMacro.find(strDigit);
	if (it != m_hashMacro.end())
	{
		return it->second;
	}
	// 不是宏应该就是直接的数值
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

// 从当前的模板类及其父类里查找枚举信息
BOOL CParseHeadFile::GetEnumInfo(CTemplateInfo& rInfo, string strEnum, hash_map<string, CEnumInfo>::iterator& itResult)
{
	// 查找全局枚举信息
	hash_map<string, CEnumInfo>::iterator it = rInfo.m_hashEnum.find(strEnum);
	if (it == rInfo.m_hashEnum.end())
	{
		// 如果有父模板,查找父模板的嵌套枚举
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

// 在全局枚举信息里查找,如果找不着就能所有的模板的嵌套枚举信息
BOOL CParseHeadFile::GetEnumInfo(string strEnum, hash_map<string, CEnumInfo>::iterator& itResult)
{
	// 先找全局的
	hash_map<string, CEnumInfo>::iterator it = m_hashEnum.find(strEnum);
	if (it == m_hashEnum.end())
	{
		// 遍历所有的模板嵌套枚举信息
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
