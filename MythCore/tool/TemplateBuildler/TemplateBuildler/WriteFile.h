#pragma once
/********************************************************************
	created:	12/12/2011   11:29
	filename: 	WriteFile.h
	author:		jeffery

	purpose:	用于将分析出来的数据写成原始模板文件
*********************************************************************/
class CParseHeadFile;
class CTemplateInfo;
class CWriteFile
{
public:
	CWriteFile(void);
	~CWriteFile(void);

public:
	// 开始将分析出来的头文件写成原始模板文件
	BOOL	BeginWriteFile(CParseHeadFile &ParseHeadFile, CString strOutPath);
	// 递归写入父模板文件
	BOOL	WriteParentField(CParseHeadFile &ParseHeadFile, ofstream& OutFile, string strParentName);
	// 写入模板字段
	BOOL	WriteTemplateField(CParseHeadFile &ParseHeadFile, ofstream& OutFile, CTemplateInfo& rInfo);
	wstring	s2ws(const string& s);
	string	ws2s(const wstring& ws);
	void	ReplaceString(string& strReplaceString, char source, char dest);
};
