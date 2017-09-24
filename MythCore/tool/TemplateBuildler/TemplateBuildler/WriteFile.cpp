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
			_snwprintf(acBuffer, sizeof(acBuffer), _T("%s\n%s"), s2ws(strFilePath + string("#") + rInfo.m_strName + ".txt").c_str(), _T("�޷�����д�ļ�!"));
			AfxMessageBox(acBuffer);
			//AfxMessageBox(_T("�޷�����д�ļ�!"));
			return FALSE;
		}
		if (rInfo.m_strParent != "")
		{
			// д��ģ����ֶ�
			WriteParentField(ParseHeadFile, OutFile, rInfo.m_strParent);
		}
	
		// д��ǰģ����ֶ�
		WriteTemplateField(ParseHeadFile, OutFile, rInfo);
		OutFile.close();
	}

	AfxMessageBox(_T("�������"));
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

// ���ַ�ת���ֽڣ��������Ч�ʺܵͣ���Ч�ʺ�����ͬ����
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

// д��ģ���ֶ�
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
	// û���Ÿ�ģ��,
	if (nParentIndex < 0)
	{
		//AfxMessageBox(_T("�Ƿ��ĸ�ģ������"));
		return FALSE;
	}

	// д��ģ���ֶ�
	CTemplateInfo& rInfo = ParseHeadFile.m_vecTemplate[nParentIndex];
	if (rInfo.m_strParent != "")
	{
		WriteParentField(ParseHeadFile, OutFile, rInfo.m_strParent);
	}

	// д�Լ����ֶ�
	WriteTemplateField(ParseHeadFile, OutFile, rInfo);
	return TRUE;
}

// дģ���ֶ�
BOOL CWriteFile::WriteTemplateField(CParseHeadFile &ParseHeadFile, ofstream& OutFile, CTemplateInfo& rInfo)
{
	string::size_type nComboEnumIndex = string::npos;
	// �������е��ֶ�
	for (size_t j = 0; j < rInfo.m_vecFieldInfo.size(); j ++)
	{
		CFieldInfo& FieldInfo = rInfo.m_vecFieldInfo[j];
		OutFile<<"FieldName: ";
		OutFile<<FieldInfo.m_strFieldName;
		OutFile<<char(0)<<endl;

		OutFile<<"FieldType: ";
		OutFile<<FieldInfo.m_strFieldType;
		OutFile<<char(0)<<endl;

		// ���������COMBOENUM,��Ҫչ��
		if((nComboEnumIndex = FieldInfo.m_strType.find("COMBOENUM")) != string::npos)
		{
			// �õ�ö����
			string::size_type nEndIndex = FieldInfo.m_strType.find(")");
			string strEnum = GetFirstWord(FieldInfo.m_strType.substr(nComboEnumIndex + 10, nEndIndex - nComboEnumIndex - 10));
			// ͨ��ö�����õ�ö�ٵ���Ϣ
			hash_map<string, CEnumInfo>::iterator it;
			//rInfo.m_hashEnum.find(strEnum);
			if (!ParseHeadFile.GetEnumInfo(rInfo, strEnum, it))
			{
				if (!ParseHeadFile.GetEnumInfo(strEnum, it))
				{
					CString strOutput = _T("�Ҳ������õ�ö��:") + CString(s2ws(strEnum).c_str());

					AfxMessageBox(strOutput);
					return FALSE;
				}
			}

			// ��ö����Ϣչ������Ҫ���ַ���
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