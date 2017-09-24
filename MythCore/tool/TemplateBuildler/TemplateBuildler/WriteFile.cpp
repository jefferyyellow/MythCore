#include "StdAfx.h"
#include "WriteFile.h"
#include "ParseHeadFile.h"
#include <DbgHelp.h>

CWriteFile::CWriteFile(void)
{
}

CWriteFile::~CWriteFile(void)
{
}


BOOL CWriteFile::BeginWriteFile(CParseHeadFile &ParseHeadFile, CString strOutPath)
{

	string strOut = ws2s(strOutPath.GetBuffer());

	ofstream OutFile;
	for (size_t i = 0; i < ParseHeadFile.m_vecTemplate.size(); i++)
	{
		CTemplateInfo& rInfo = ParseHeadFile.m_vecTemplate[i];
		if (rInfo.m_bVirtual)
		{
			continue;
		}

		string strFilePath = strOut + string("/Template/")+ rInfo.m_strPath + string("/");
		ReplaceString(strFilePath, '/', '\\');
		
		fstream _file;
		_file.open(strFilePath.c_str(),ios::in);
		if (!_file)
		{
			MakeSureDirectoryPathExists(strFilePath.c_str());
		}
		_file.close();

		OutFile.open((s2ws(strFilePath + string("#") + rInfo.m_strName + ".txt")).c_str(), ios_base::out | ios_base::trunc);
		
		if (!OutFile.is_open())
		{
			TCHAR acBuffer[MAX_PATH] = {0};
			_snwprintf(acBuffer, sizeof(acBuffer), _T("%s\n%s"), s2ws(strFilePath + string("#") + rInfo.m_strName + ".txt").c_str(), _T("无法打开所写文件!"));
			AfxMessageBox(acBuffer);
			//AfxMessageBox(_T("无法打开所写文件!"));
			return FALSE;
		}
		if (rInfo.m_strParent != "")
		{
			// 写父模板的字段
			WriteParentField(ParseHeadFile, OutFile, rInfo.m_strParent);
		}
	
		// 写当前模板的字段
		WriteTemplateField(ParseHeadFile, OutFile, rInfo);
		OutFile.close();
	}

	AfxMessageBox(_T("生成完成"));
	return TRUE;
}

wstring CWriteFile::s2ws(const string& s)
{
	string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";

	setlocale(LC_ALL, "chs"); 

	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;

	TCHAR acDest[MAX_PATH] = {0};

	mbstowcs(acDest,_Source,_Dsize);
	wstring result = acDest;
	setlocale(LC_ALL, curLocale.c_str());

	return result;
}

// 宽字符转多字节，这个函数效率很低，高效率函数不同调用
string CWriteFile::ws2s(const wstring& ws)
{
	string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";

	setlocale(LC_ALL, "chs"); 
	const wchar_t* _Source = ws.c_str();
	size_t _Dsize = ws.size() * 2 + 1;
	char acDest[MAX_PATH * 2] = {0};

	wcstombs(acDest, _Source, _Dsize);
	string result = acDest;


	setlocale(LC_ALL, curLocale.c_str());
	return result;
}

// 写父模板字段
BOOL CWriteFile::WriteParentField(CParseHeadFile &ParseHeadFile, ofstream& OutFile, string strParentName)
{
	int nParentIndex = -1;
	for (size_t i = 0; i < ParseHeadFile.m_vecTemplate.size(); i++)
	{
		if (ParseHeadFile.m_vecTemplate[i].m_strName == strParentName)
		{
			nParentIndex = (int)i;
			break;
		}
	}
	// 没找着父模板,
	if (nParentIndex < 0)
	{
		//AfxMessageBox(_T("非法的父模板名称"));
		return FALSE;
	}

	// 写父模板字段
	CTemplateInfo& rInfo = ParseHeadFile.m_vecTemplate[nParentIndex];
	if (rInfo.m_strParent != "")
	{
		WriteParentField(ParseHeadFile, OutFile, rInfo.m_strParent);
	}

	// 写自己的字段
	WriteTemplateField(ParseHeadFile, OutFile, rInfo);
	return TRUE;
}

// 写模板字段
BOOL CWriteFile::WriteTemplateField(CParseHeadFile &ParseHeadFile, ofstream& OutFile, CTemplateInfo& rInfo)
{
	string::size_type nComboEnumIndex = string::npos;
	// 遍历所有的字段
	for (size_t j = 0; j < rInfo.m_vecFieldInfo.size(); j ++)
	{
		CFieldInfo& FieldInfo = rInfo.m_vecFieldInfo[j];
		OutFile<<"FieldName: ";
		OutFile<<FieldInfo.m_strFieldName;
		OutFile<<char(0)<<endl;

		OutFile<<"FieldType: ";
		OutFile<<FieldInfo.m_strFieldType;
		OutFile<<char(0)<<endl;

		// 如果类型是COMBOENUM,需要展开
		if((nComboEnumIndex = FieldInfo.m_strType.find("COMBOENUM")) != string::npos)
		{
			// 得到枚举名
			string::size_type nEndIndex = FieldInfo.m_strType.find(")");
			string strEnum = GetFirstWord(FieldInfo.m_strType.substr(nComboEnumIndex + 10, nEndIndex - nComboEnumIndex - 10));
			// 通过枚举名得到枚举的信息
			hash_map<string, CEnumInfo>::iterator it;
			//rInfo.m_hashEnum.find(strEnum);
			if (!ParseHeadFile.GetEnumInfo(rInfo, strEnum, it))
			{
				if (!ParseHeadFile.GetEnumInfo(strEnum, it))
				{
					CString strOutput = _T("找不着引用的枚举:") + CString(s2ws(strEnum).c_str());

					AfxMessageBox(strOutput);
					return FALSE;
				}
			}

			// 将枚举信息展开成想要的字符串
			CEnumInfo& enumInfo = it->second;
			string strComboEnum = "COMBOSINGLE(";
			for (size_t i = 0; i < enumInfo.m_vecEnumInfo.size(); i++)
			{
				strComboEnum += " ";
				strComboEnum += enumInfo.m_vecEnumInfo[i].m_strKey;
				strComboEnum += "=";
				strComboEnum += enumInfo.m_vecEnumInfo[i].m_strValue;
			}
			strComboEnum[strComboEnum.length() - 1] = ')';
			OutFile<<"Type: ";
			OutFile<<strComboEnum;
			OutFile<<char(0)<<endl;

		}
		else
		{
			OutFile<<"Type: ";
			OutFile<<FieldInfo.m_strType;
			OutFile<<char(0)<<endl;
		}
	}

	return TRUE;
}

void CWriteFile::ReplaceString(string& strReplaceString, char source, char dest)
{
	for (size_t i = 0; i < strReplaceString.length(); i++)
	{
		if (strReplaceString[i] == source)
		{
			strReplaceString[i] = dest;
		}
	}
}