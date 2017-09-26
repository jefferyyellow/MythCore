#ifndef _PB_TOOL_COMMON_H_
#define _PB_TOOL_COMMON_H_

#define  XML_ASSERT(x)          { if( NULL == (x) ) return NULL; }

namespace PBToolCommon
{
	// ���Ҫ�����ļ���·��������·���������򴴽�һ���µģ�������������
	int CheckDirection( const char* pDirName );

	// ���ݱ�����ȡ��������
	const char* GetPrimaryName(const char* nFormName);

	// ת���ַ�����Сд
	const char* ToLower( char pSrc[], int nLen );

	// �����ļ��������ļ�����Ϊ��������.back��
	int CopyFile(const char* pFilePath);
}

#endif
