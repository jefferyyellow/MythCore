#pragma once
/********************************************************************
	created:	12/12/2011   11:29
	filename: 	WriteFile.h
	author:		jeffery

	purpose:	���ڽ���������������д��ԭʼģ���ļ�
*********************************************************************/
class CParseHeadFile;
class CTemplateInfo;
class CWriteFile
{
public:
	CWriteFile(void);
	~CWriteFile(void);

public:
	// ��ʼ������������ͷ�ļ�д��ԭʼģ���ļ�
	BOOL	BeginWriteFile(CParseHeadFile &ParseHeadFile, CString strOutPath);
	// �ݹ�д�븸ģ���ļ�
	BOOL	WriteParentField(CParseHeadFile &ParseHeadFile, ofstream& OutFile, string strParentName);
	// д��ģ���ֶ�
	BOOL	WriteTemplateField(CParseHeadFile &ParseHeadFile, ofstream& OutFile, CTemplateInfo& rInfo);
	wstring	s2ws(const string& s);
	string	ws2s(const wstring& ws);
	void	ReplaceString(string& strReplaceString, char source, char dest);
};
