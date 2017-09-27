#pragma once
/********************************************************************
	created:	15/12/2011   10:30
	filename: 	TemplateWrite.h
	author:		jeffery

	purpose:	用于将模板生成二进制文件
*********************************************************************/
class CTemplateManager;
class PBTplTemplate;

#define CREATE_DATA_TO_PB(TemplateName, TemplateClassName, PBParent, PBName) \
	if (TemplateName == strTempName) \
	{ \
		if (sizeof(TemplateClassName) != nTempLen) \
		{ \
			return FALSE; \
		} \
		TemplateClassName* pTplConfig = (TemplateClassName*)pTempData; \
		pTplConfig->createToPB(PBParent->add_##PBName()); \
	} 




class CTemplateWrite
{
public:
	enum
	{
		emSingleTemplateMaxSize = 40960,
		emAllTemplateMaxSize = 25 * 1024 * 1024,
	};

public:
	CTemplateWrite(void);
	~CTemplateWrite(void);

public:
	// 将数据写成PB格式的二进制
	BOOL	WriteToPB(CTemplateManager* pTemplate);
	// 创建xml版本的前端
	BOOL	WritePBToXml(PBTplTemplate& rTemplate);
	// 往PB结构里加入数据
	BOOL	AddPBData(PBTplTemplate* pTplTemplate, CString& strTempName, char* pTempData, int nTempLen);
	// 校验数据
	BOOL	VerifyValue(CTemplateManager* pTemplateManager, CString& strOutError);
	// 校验单选一数据
	BOOL	VerifyComboSingle(string strType, string& strValue);
	// 校验多选数据
	BOOL	VerifyCheckMuti(string strType, string& strValue);
	// 校验数字数据
	BOOL	VerifyEditNumber(string strType, string& strValue);
	// 校验字符串数据
	BOOL	VerifyEditString(string strFieldType, string& strValue);
	// 校验TEMPPATH数据
	BOOL	VerifyTempPath(string strFieldType, string& strValue);
	// 将字符串转换成UTF8
	void	ASCIIToUTF8(const char* pSr, int nSrcLen, char*pDst, int nDstLen);

public:
	BOOL	GetWritingPB(){return m_bWritingPB;}
	void	SetWritingPB(BOOL bWritingPB){m_bWritingPB = bWritingPB;}
	void	SetProcess(int nProgress);
	void	SetProcessText(wchar_t* pText);

public:
	static CTemplateWrite*	GetSingletonPtr(){return m_pInst;}
	
private:
	static CTemplateWrite* m_pInst;
	BOOL	m_bWritingPB;

};
