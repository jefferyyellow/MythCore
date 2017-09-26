/************************************************************
  Copyright ?  2002-2003,
  FileName:     ConfigFile.h
  Author:        
  Version:      1.0
  Date:         2002��5��8��
  Description:
    ʵ�ֶ������ļ��Ķ�ȡ��
    ʹ�÷������£�
      1 ��������Ķ���ʵ�����ڹ��캯���д��������ļ����ļ���
      2 ���ó�Ա����IsOpen()�ж��Ƿ��ȡ�ɹ�
      3 ����GetItemValue�õ������ֵ
  Others: 
  Function List:
    1. CConfigFile::CConfigFile
       ���캯������ʼ����ĳ�Ա����ȡ�����ļ�
    2. CConfigFile::~CConfigFile
       �����������ͷ���Դ
    3. CConfigFile::IsOpen
       ���������ļ���ȡ״̬
    4. CConfigFile::GetItemValue�ַ���
       ȡָ���ļ�ֵ
    5. CConfigFile::LocateSection
       ��λsection�Ŀ�ʼ��ַ�ͽ�����ַ
    6. CConfigFile::LocateKeyValue
       ��ָ���Ļ�������Χ������Key��������Keyƥ���ֵ�Ŀ�ʼ��ַ�ͽ�����ַ
    7. CConfigFile::LocateStr
       ��һ���ַ�����������һ���ַ���
    8. CConfigFile::MapToContent
       ��Shadow�еĵ�ַӳ�䵽Content��
    9. CConfigFile::MapToShadow
       ��Content�еĵ�ַӳ�䵽Shadow��
    10.CConfigFile::uT_main
       ��Ĳ����ó�Ա����������̬����
    11.CConfigFile::uT_GetContentBuffer
    12.CConfigFile::uT_SetBuffer
    13.CConfigFile::uT_ClearBuffer
    14.CConfigFile::uT_SetContent
    15.CConfigFile::uT_SetShadow
    16.CConfigFile::uT_SetIsOpenFlag
    17.ToLower
       ���ַ����еĴ�д��ĸת����Сд��ĸ
    18.CConfigFile::OpenFile
       ���������ļ����ڴ�
    19.CConfigFile::CloseFile
       �ͷ������ļ����ص��ڴ��ռ�õ���Դ
    20.CConfigFile::SetItemValue�ַ���
       ���ڴ滺�������ҵ�KeyName����ֵ������ָ���Ŀռ�
    21.CConfigFile::LocateKeyRange
       ��ָ���Ļ�������Χ������Key��������Keyƥ���ֵ�Ŀ�ʼ��ַ�ͽ�����ַ
    22.CConfigFile::SetItemValue����
    23.CConfigFile::GetItemValue����
    24.CConfigFile::GetItemValue��ȱʡֵ����
    25.CConfigFile::GetItemValue��ȱʡֵ�ַ���
  History: 
             2002/05/08        1.0        ������ģ��
             2002/05/13        1.0        ����SetItemValue�ַ���,
                                             OpenFile, CloseFile,
                                             LocateKeyRange
             2002/06/08        1.0        ����SetItemValue����,
                                             ����GetItemValue����
             2002/06/10        1.0        ʹGetItemValue, SetItemValue֧����Դ����
             2002/06/11        1.0        ���Ӵ�ȱʡֵ��GetItemValue����, GetItemValue�ַ���
***********************************************************/

#ifndef __CONFIGFILE_H__
#define __CONFIGFILE_H__

#include <stdio.h>


class CConfigFile
{
private:
    char    *m_pszContent;      /* �����ļ���ԭʼ���� */
    char    *m_pszShadow;       /* �����ļ�������ȫ��ת����Сд */
    size_t  m_nSize;            /* �����ļ����ݵĳ��ȣ�����������NULL */
    short   m_bIsOpen;          /* �����ļ��Ƿ�򿪳ɹ��ı�־ */

public:
    char    *m_pszFilename;     /* �����Ҫ��ȡ�������ļ��� */
	CConfigFile();  
    ~CConfigFile();

    unsigned int IsOpen();
    unsigned int GetItemValue( const char *pszSectionName, 
                        const char *pszKeyName, 
                        char *pszReturnedString, 
                        unsigned int nSize );
    unsigned int SetItemValue( const char *pszSectionName, 
                        const char *pszKeyName, 
                        const char *pszKeyValue );
    unsigned int GetItemValue( const char *pszSectionName, 
                        const char *pszKeyName, 
                        int &lReturnedValue );
     unsigned int GetItemValue( const char *pszSectionName, 
                        const char *pszKeyName, 
                        short &lReturnedValue );
    unsigned int GetItemValue( const char *pszSectionName, 
                        const char *pszKeyName, 
                        unsigned short &lReturnedValue );
    unsigned int SetItemValue( const char *pszSectionName, 
                        const char *pszKeyName, 
                        int skeyValue );
    unsigned int GetItemValue( const char *pszSectionName, 
                        const char *pszKeyName, 
                        int &lReturnedValue,
                        int lDefaultValue );
    unsigned int GetItemValue( const char *pszSectionName, 
                        const char *pszKeyName, 
                        char *pszReturnedString, 
                        unsigned int nSize, 
                        const char *pszDefaultValue);

	int  OpenFile(const char *pszFilename);
    
	void CloseFile();

private:
    
    unsigned int LocateSection(const char *pszSectionName, 
                        char * &pszSectionBegin, 
                        char * &pszSectionEnd);
    unsigned int LocateKeyRange(const char *pszKeyName, 
                        const char *pszSectionBegin, 
                        const char *pszSectionEnd, 
                        char * &pszKeyBegin, 
                        char * &pszKeyEnd);
    unsigned int LocateKeyValue(const char *pszKeyName, 
                        const char *pszSectionBegin, 
                        const char *pszSectionEnd, 
                        char * &pszValueBegin, 
                        char * &pszValueEnd);
    char *LocateStr(    const char *pszCharSet, 
                        const char *pszBegin, 
                        const char *pszEnd );
	char *SearchMarchStr(const char *pszBegin, const char *pszCharSet);

    char *MapToContent(const char *p);
    char *MapToShadow(const char *p);

	void ToLower( char * pszSrc, size_t len);

};


#endif

