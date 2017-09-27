#pragma once
/********************************************************************
	created:	15/12/2011   10:30
	filename: 	TemplateWrite.h
	author:		jeffery

	purpose:	���ڽ�ģ�����ɶ������ļ�
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
	// ������д��PB��ʽ�Ķ�����
	BOOL	WriteToPB(CTemplateManager* pTemplate);
	// ����xml�汾��ǰ��
	BOOL	WritePBToXml(PBTplTemplate& rTemplate);
	// ��PB�ṹ���������
	BOOL	AddPBData(PBTplTemplate* pTplTemplate, CString& strTempName, char* pTempData, int nTempLen);
	// У������
	BOOL	VerifyValue(CTemplateManager* pTemplateManager, CString& strOutError);
	// У�鵥ѡһ����
	BOOL	VerifyComboSingle(string strType, string& strValue);
	// У���ѡ����
	BOOL	VerifyCheckMuti(string strType, string& strValue);
	// У����������
	BOOL	VerifyEditNumber(string strType, string& strValue);
	// У���ַ�������
	BOOL	VerifyEditString(string strFieldType, string& strValue);
	// У��TEMPPATH����
	BOOL	VerifyTempPath(string strFieldType, string& strValue);
	// ���ַ���ת����UTF8
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
