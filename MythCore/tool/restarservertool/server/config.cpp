/************************************************************
  Copyright ?  2002-2003,
  FileName:     ConfigFile.cpp
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
    10.ToLower
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
***********************************************************/
#include "config.h"
#include <string.h>
#include <stdlib.h>

typedef unsigned char BYTE;

/*************************************************
  Function:     CConfigFile
  Description:
        CConfigFile�Ĺ��캯�������ڳ�ʼ����ĳ�Ա��������ȡָ���������ļ���
        ����ļ���ȡ�ɹ�������m_bIsOpenΪtrue
  Calls:        CConfigFile::OpenFile
  Called By:
  Input:
        pszFilename     - ��NULL��β�������ļ���
  Output:
  Return:
  Others:
*************************************************/

CConfigFile::CConfigFile()
{
    m_pszFilename = NULL;
    m_pszContent = NULL;
    m_pszShadow = NULL;
    m_nSize = 0;
    m_bIsOpen = false;
}

/*************************************************
  Function:     OpenFile
  Description:
        ��ȡָ���������ļ���
        ����ļ���ȡ�ɹ�������m_bIsOpenΪtrue
  Calls:
  Called By:    CConfigFile::CConfigFile
  Input:
        pszFilename     - ��NULL��β�������ļ���
  Output:
  Return:
  Others:
*************************************************/
int CConfigFile::OpenFile(const char *pszFilename)
{
    FILE    *fp;
    size_t  len;
    int     result;

    /* �ͷ���Դ */
    CloseFile();

    if (NULL == pszFilename)
    {
        return -1;
    }

    m_pszFilename = strdup(pszFilename);

    fp = fopen(m_pszFilename, "rb");

    if (NULL == fp)
    {
        return -1;
    }

    result = fseek(fp, 0L, SEEK_END);
    if (0 != result)
    {
        fclose(fp);
        return -1;
    }

    len = (size_t)ftell(fp);
    m_pszContent = (char *)new char [len+1];
    m_pszShadow = (char *)new char [len+1];

    if ((NULL == m_pszContent) || (NULL == m_pszShadow))
    {
        fclose(fp);
        return -1;
    }

    result = fseek(fp, 0L, SEEK_SET);
    if (0 != result)
    {
        fclose(fp);
        return -1;
    }

    m_nSize = fread(m_pszContent, 1, len, fp);
    m_pszContent[m_nSize] = '\0';

    /* ����Ӱ���ڴ棬������ȫ����Сд���ַ��� */
    memcpy(m_pszShadow, m_pszContent, m_nSize + 1);
    ToLower(m_pszShadow, m_nSize + 1);

    fclose(fp);
    m_bIsOpen = true;
	return 0;
}

/*************************************************
  Function:     CloseFile
  Description:
        ��ȡָ���������ļ���
        ����ļ���ȡ�ɹ�������m_bIsOpenΪtrue
  Calls:
  Called By:    CConfigFile::~CConfigFile
  Input:
        pszFilename     - ��NULL��β�������ļ���
  Output:
  Return:
  Others:
*************************************************/
void CConfigFile::CloseFile()
{
    /* �ͷ���Դ */
    if (m_pszFilename)
    {
        free(m_pszFilename);
		m_pszFilename = NULL;
    }

    if (m_pszContent)
    {
        delete [] m_pszContent;
        m_pszContent = NULL;
    }

    if (m_pszShadow)
    {
        delete [] m_pszShadow;
        m_pszShadow = NULL;
    }

    m_nSize = 0;
    m_bIsOpen = false;
}

/*************************************************
  Function:     ~CConfigFile
  Description:
        CConfigFile�����������ͷ��������Դ
  Calls:        CConfigFile::CloseFile
  Called By:
  Input:
  Output:
  Return:
  Others:
*************************************************/
CConfigFile::~CConfigFile()
{
    CloseFile();
}

/*************************************************
  Function:     IsOpen
  Description:
        ���ض�ȡ�����ļ��Ƿ�ɹ��ı�־
  Calls:
  Called By:    CConfigFile::GetItemValue,
                CConfigFile::uT_main
  Input:
  Output:
  Return:       ��������ļ���ȡ�ɹ�������true�����򷵻�false
  Others:
*************************************************/
unsigned int CConfigFile::IsOpen()
{
    return m_bIsOpen;
}

/*************************************************
  Function:     GetItemValue����
  Description:
        ���ڴ���ȡָ�����������͵ļ�ֵ����������ڣ���ʹ��ָ����ȱʡֵ
  Calls:        CConfigFile::GetItemValue�ַ���
  Called By:    CConfigFile::uT_main
  Input:
        pszSectionName  - ��NULL��β���ַ���ָ�룬ָʾ����Key��Ƭ��
        pszKeyName      - ��NULL��β���ַ���ָ�룬ָʾ��Ҫ����ֵ��Key
        lDefaultValue   - ȡֵʧ�ܺ�ʹ�õ�ȱʡֵ
  Output:
        ulReturnedValue - ָ�����ڽ��ս���Ļ�������ַ
  Return:       �ɹ�����true, ʧ�ܷ���false
  Others:       ����������UNICODE�汾
*************************************************/
unsigned int CConfigFile::GetItemValue( const char *pszSectionName,
                                   const char *pszKeyName,
                                   int &lReturnedValue,
                                   int lDefaultValue)
{
    if (0 == GetItemValue(pszSectionName, pszKeyName, lReturnedValue))
    {
        lReturnedValue = lDefaultValue;
        return false;
    }

    return true;
}

/*************************************************
  Function:     GetItemValue����
  Description:
        ���ڴ���ȡָ�����������͵ļ�ֵ
  Calls:        CConfigFile::GetItemValue�ַ���
  Called By:    CConfigFile::uT_main
  Input:
        pszSectionName  - ��NULL��β���ַ���ָ�룬ָʾ����Key��Ƭ��
        pszKeyName      - ��NULL��β���ַ���ָ�룬ָʾ��Ҫ����ֵ��Key
  Output:
        ulReturnedValue - ָ�����ڽ��ս���Ļ�������ַ
  Return:       �ɹ�����true, ʧ�ܷ���false
  Others:       ����������UNICODE�汾
*************************************************/
unsigned int CConfigFile::GetItemValue( const char *pszSectionName,
                                   const char *pszKeyName,
                                   int &lReturnedValue )
{
    char szBuf[100];

    if (0 == GetItemValue(pszSectionName, pszKeyName, szBuf, 100))
    {
        return false;
    }

    lReturnedValue = atol(szBuf);

    return true;
}

unsigned int CConfigFile::GetItemValue( const char *pszSectionName,
                                   const char *pszKeyName,
                                   short &lReturnedValue )
{
    char szBuf[100];

    if (0 == GetItemValue(pszSectionName, pszKeyName, szBuf, 100))
    {
        return false;
    }

    lReturnedValue = (short)atoi(szBuf);

    return true;
}

unsigned int CConfigFile::GetItemValue( const char *pszSectionName,
                                   const char *pszKeyName,
                                   unsigned short &lReturnedValue )
{
    char szBuf[100];

    if (0 == GetItemValue(pszSectionName, pszKeyName, szBuf, 100))
    {
        return false;
    }

    lReturnedValue = (unsigned short)atoi(szBuf);

    return true;
}

/*************************************************
  Function:     GetItemValue�ַ���
  Description:
        ���ڴ���ȡָ�����ַ������͵ļ�ֵ����������ڣ���ʹ��ָ����ȱʡֵ
  Calls:        CConfigFile::GetItemValue�ַ���
  Called By:    CConfigFile::uT_main
  Input:
        pszSectionName  - ��NULL��β���ַ���ָ�룬ָʾ����Key��Ƭ��
        pszKeyName      - ��NULL��β���ַ���ָ�룬ָʾ��Ҫ����ֵ��Key
        nSize           - ָ�����ջ������Ĵ�С
        pszDefaultValue - ȡֵʧ�ܺ�ʹ�õ�ȱʡֵ
  Output:
        pszReturnedString - ָ�����ڽ��ս���Ļ�������ַ
  Return:       ���ػ������е���Ч�ַ��������������ַ�����β��NULL
  Others:       ����������UNICODE�汾
*************************************************/
unsigned int CConfigFile::GetItemValue(const char *pszSectionName,
                                  const char *pszKeyName,
                                  char *pszReturnedString,
                                  unsigned int nSize,
                                  const char *pszDefaultValue)
{
    unsigned int len;

    if (nSize <=0 )
        return 0;

    len = GetItemValue(pszSectionName, pszKeyName, pszReturnedString, nSize);

    if (0 == len)
    {
        strncpy(pszReturnedString, pszDefaultValue, nSize-1);
        pszReturnedString[nSize-1] = '\0';
        return strlen(pszReturnedString);
    }

    return len;
}

/*************************************************
  Function:     GetItemValue�ַ���
  Description:
        ���ڴ滺�������ҵ�KeyName����ֵ������ָ���Ŀռ䡣
        �������ֵ���ڿռ�Ĵ�С������ַ������н�β����
    ���ڻ����������һ���ֽڼ���NULL��
        ������������������ַ��Ǻ��ֱ���ʱ�����Զ���������������
  Calls:        CConfigFile::IsOpen,
                CConfigFile::LocateKey,
                CConfigFile::LocateSection
  Called By:    CConfigFile::uT_main
  Input:
        pszSectionName  - ��NULL��β���ַ���ָ�룬ָʾ����Key��Ƭ��
        pszKeyName      - ��NULL��β���ַ���ָ�룬ָʾ��Ҫ����ֵ��Key
        nSize           - ָ�����ջ������Ĵ�С
  Output:
        pszReturnedString - ָ�����ڽ��ս���Ļ�������ַ
  Return:       ���ػ������е���Ч�ַ��������������ַ�����β��NULL
  Others:       ����������UNICODE�汾
*************************************************/
unsigned int CConfigFile::GetItemValue(const char *pszSectionName,
                                  const char *pszKeyName,
                                  char *pszReturnedString,
                                  unsigned int nSize)
{
    char *pszSectionBegin, *pszSectionEnd;
    char *pszValueBegin, *pszValueEnd;
    unsigned int dwCount;

    /* �������Ƿ��ʼ���ɹ� */
    if (false == IsOpen())
    {
        return (unsigned int)0;
    }

    /* ��鴫������Ϸ��� */
    if ((NULL == pszSectionName)
        || (NULL == pszKeyName)
        || (NULL == pszReturnedString))
    {
        return (unsigned int)0;
    }

    if (nSize == 0)
    {
        return (unsigned int)0;
    }



    /* ����SectionName����λSection�Ŀ�ʼ�ͽ�βָ�� */
    if (false == LocateSection(pszSectionName, pszSectionBegin, pszSectionEnd))
    {


        return (unsigned int)0;
    }

    /* ��ָ����Χ�ڶ�λKeyName��Value */
    if (false == LocateKeyValue( pszKeyName,
                                 pszSectionBegin,
                                 pszSectionEnd,
                                 pszValueBegin,
                                 pszValueEnd ))
    {
        /* Keyû�ҵ�{����pszDefault��ֵ��Ϊ����ֵ} */


        return (unsigned int)0;
    }

    /* ����Ҫ��ֵ�������������У���ע�⻺�������� */
    dwCount = 0;

    for (; pszValueBegin < pszValueEnd && dwCount < (nSize-1); pszValueBegin++, dwCount++)
        pszReturnedString[dwCount] = *pszValueBegin;

    /* if (dwCount == nSize)
    {
        dwCount = nSize -1;
    } */

    pszReturnedString[dwCount] = '\0';

    /* �ַ������ضϣ��жϣ����һ���ַ��Ƿ�Ϊ˫�ֽ� */
    if ((dwCount == nSize-1) && ((BYTE)(pszReturnedString[dwCount-1]) > 0x7f))
    {
        /* ��˫�ֽڵ����һ���ַ�����Ϊ'\0' */
        /* Ϊ�˷�ֹ������������������Ժ���ַ�������Խ�� */
        /*     "\xa9" */
        pszReturnedString[dwCount-1] = '\0';
        dwCount --;
    }
    return (unsigned int)dwCount;
}

/*************************************************
  Function:     SetItemValue����
  Description:
        ��ָ�����������ͼ�ֵ���������ͬʱ�����ڴ�������ļ�
  Calls:        CConfigFile::SetItemValue�ַ���
  Called By:    CConfigFile::uT_main
  Input:
        pszSectionName  - ��NULL��β���ַ���ָ�룬ָʾ����Key��Ƭ��
        pszKeyName      - ��NULL��β���ַ���ָ�룬ָʾ��Ҫ���õ�Key
        uskeyValue      - �������ͣ�ָʾ��Ҫ���õ�ֵ
  Output:
  Return:       �����Ƿ�ɹ��ı�־���ɹ�������true�����򷵻�false
  Others:       ����������UNICODE�汾
*************************************************/
unsigned int CConfigFile::SetItemValue( const char *pszSectionName,
                                   const char *pszKeyName,
                                   int skeyValue )
{
    char szBuf[100];

#ifdef WIN32
    ltoa(skeyValue, szBuf, 10);
#else
    sprintf(szBuf, "%d", skeyValue);
#endif

    return SetItemValue(pszSectionName, pszKeyName, szBuf);
}

/*************************************************
  Function:     SetItemValue�ַ���
  Description:
        ���ڴ滺�������ҵ�KeyName����ֵ������ָ���Ŀռ䣬�����������ļ���
        �������ֵ���ڿռ�Ĵ�С������ַ������н�β����
    ���ڻ����������һ���ֽڼ���NULL��
        ������������������ַ��Ǻ��ֱ���ʱ�����Զ���������������
  Calls:        CConfigFile::IsOpen,
                CConfigFile::LocateKey,
                CConfigFile::LocateSection
  Called By:    CConfigFile::uT_main
  Input:
        pszSectionName  - ��NULL��β���ַ���ָ�룬ָʾ����Key��Ƭ��
        pszKeyName      - ��NULL��β���ַ���ָ�룬ָʾ��Ҫ���õ�Key
        pszKeyValue     - ��NULL��β���ַ���ָ�룬ָʾ��Ҫ���õ�ֵ
  Output:
  Return:       �����Ƿ�ɹ��ı�־���ɹ�������true�����򷵻�false
  Others:       ����������UNICODE�汾
*************************************************/
unsigned int CConfigFile::SetItemValue(const char *pszSectionName,
                                const char *pszKeyName,
                                const char *pszKeyValue)
{
    char *pszSectionBegin, *pszSectionEnd;
    char *pszKeyBegin, *pszKeyEnd;
    char *pszContent, *pszShadow;
    char *pszBuf;
    size_t len;

    /* �������Ƿ��ʼ���ɹ� */
    if (false == IsOpen())
    {
        return false;
    }

    /* ��鴫��γ��Ϸ��� */
    if ((NULL == pszSectionName)
        || (NULL == pszKeyName)
        || (NULL == pszKeyValue))
    {
        return false;
    }



    /* ����SectionName����λSection�Ŀ�ʼ�ͽ�βָ�� */
    if (false == LocateSection(pszSectionName, pszSectionBegin, pszSectionEnd))
    {
        return false;
    }

    /* ��ָ����Χ������KeyName */
    if (false == LocateKeyRange( pszKeyName,
                                 pszSectionBegin,
                                 pszSectionEnd,
                                 pszKeyBegin,
                                 pszKeyEnd ))
    {
        /* Keyû�ҵ�����pszKeyBegin��pszKeyEnd����λ��Section�Ŀ�ʼ */
        pszKeyBegin = pszSectionBegin;
        pszKeyEnd = pszSectionBegin;
    }

    /* ���������ݵ��ַ��� */
#ifdef WIN32
    len = strlen(pszKeyName) + strlen(pszKeyValue) + 5;
#else
	len = strlen(pszKeyName) + strlen(pszKeyValue) + 4;
#endif

    /* ������Դ */
    pszBuf = (char *)new char [len + 1];
    pszContent = (char *)new char [m_nSize - (pszKeyEnd - pszKeyBegin) + len + 1];
    pszShadow = (char *)new char [m_nSize - (pszKeyEnd - pszKeyBegin) + len + 1];

    if ((NULL == pszBuf) || (NULL == pszContent) || (NULL == pszShadow))
    {
        if (pszBuf)
        {
            delete [] pszBuf;
        }

        if (pszContent)
        {
            delete [] pszContent;
        }

        if (pszShadow)
        {
            delete [] pszShadow;
        }

        return false;
    }

	memset(pszBuf, 0, len + 1);
	memset(pszContent, 0, len + 1);
	memset(pszShadow, 0, len + 1);
#ifdef WIN32
    sprintf(pszBuf, "%s = %s\r\n",pszKeyName, pszKeyValue);
#else
    sprintf(pszBuf, "%s = %s\n",pszKeyName, pszKeyValue);
#endif

    /* ���µ������滻ԭ�е����� */
    memcpy( (void *)pszContent,
            (void *)m_pszContent,
            (size_t)(pszKeyBegin - m_pszContent) );
    memcpy( (void *)(pszContent + (pszKeyBegin - m_pszContent)),
            (void *)pszBuf,
            len );
    memcpy( (void *)(pszContent + (pszKeyBegin - m_pszContent) + len),
            (void *)pszKeyEnd,
            m_nSize - (pszKeyEnd - m_pszContent) + 1 );

    delete [] pszBuf;
    delete [] m_pszContent;
    delete [] m_pszShadow;

    m_nSize = m_nSize - (pszKeyEnd - pszKeyBegin) + len;

    /* �����ļ��ڴ�ӳ�� */
    m_pszContent = pszContent;

    /* ����Ӱ���ڴ棬������ȫ����Сд���ַ��� */
    m_pszShadow = pszShadow;
    memcpy(m_pszShadow, m_pszContent, m_nSize + 1);
    ToLower(m_pszShadow, m_nSize + 1);

    /* �����ļ����� */
    FILE *fp;

    if (NULL == m_pszFilename)
    {
        return false;
    }

    fp = fopen(m_pszFilename, "wb");

    if (NULL == fp)
    {
        return false;
    }

    len = fwrite(m_pszContent, 1, m_nSize, fp);

    /* ��������룬���ò��� */
    if ((size_t)len != m_nSize)
    {
        /* ���ʵ��д����ֽ����������ĳ��Ȳ���� */
        fclose(fp);

        return false;
    }

    fclose(fp);

    return true;
}

/*************************************************
  Function:     LocateSection
  Description:
        ��ָ���Ļ�������Χ������Section��������Section�Ŀ�ʼ��ַ�ͽ�����ַ��
  Calls:        CConfigFile::LocateStr
                CConfigFile::MapToContent
                ::ToLower
  Called By:    CConfigFile::GetItemValue
                CConfigFile::SetItemValue
                CConfigFile::uT_main
  Input:
        pszSectionName  - ��NULL��β���ַ���ָ�룬ָʾ��Ҫ����ֵ��Section
  Output:
        pszSectionBegin - ���շ���ֵ�Ŀ�ʼ��ַ
                          ָ��Section�е���һ�е�һ���ֽ�
        pszSectionEnd   - ���շ���ֵ�Ľ�����ַ
                          ָ�����һ����Ч�ֽڵ���һ����ַ
  Return:       ��λ�ɹ�������true��ʧ�ܣ�����false
  Others:       ����������UNICODE�汾
*************************************************/
unsigned int CConfigFile::LocateSection( const char *pszSectionName,
                                 char * &pszSectionBegin,
                                 char * &pszSectionEnd )
{
    char    *pszLowerSection;
    char    *pszSectionBeginOnShadow;
    unsigned int   bIsFirstValidCharOnLine;
    char    *pR;

    /* �����Ϸ��Լ�� */
    if (NULL == pszSectionName)
    {
        return false;
    }

    /* ��������룬���ò��� */
    if ((NULL == m_pszContent) || (NULL == m_pszShadow))
    {
        return false;
    }

    /* ��SectionNameת����Сд */
    pszLowerSection = new char [strlen(pszSectionName) + 2 + 1];

    sprintf(pszLowerSection, "[%s]", pszSectionName);
    ToLower(pszLowerSection, strlen(pszLowerSection));

    /* ��Shadow�ж�λ��Ȼ�󣬼������ȷ��ָ�� */
    /* �õ�Key��Shadow�е�λ�� */
    pszSectionBeginOnShadow = LocateStr( pszLowerSection,
                                         m_pszShadow,
                                         m_pszShadow + m_nSize );

    if (NULL == pszSectionBeginOnShadow)
    {
        /* �ͷ���Դ */
        delete [] pszLowerSection;
        return false;
    }

    pszSectionBegin = MapToContent(pszSectionBeginOnShadow)
                                    + strlen(pszLowerSection);

    /* ��SectionBeginָ��ָ��Section����һ�������ֽ� */
    /* ������ĩ�ַ� */
    for (; pszSectionBegin < (m_pszContent + m_nSize); pszSectionBegin++)
    {
//#ifdef __LINUXCONFIG__
        if ((*pszSectionBegin == '\r') || (*pszSectionBegin == '\n'))
//#else
  //      if (*pszSectionBegin == '\n')
//#endif
        {
            break;
        }
    }

    /* ������ĩ�س����� */
    for (; pszSectionBegin < (m_pszContent + m_nSize); pszSectionBegin++)
    {
//#ifdef __LINUXCONFIG__
        if ((*pszSectionBegin != '\r') && (*pszSectionBegin != '\n'))
//#else
  //      if (*pszSectionBegin != '\n')
//#endif
        {
            break;
        }
    }

    /* �ͷ���Դ */
    delete [] pszLowerSection;

    /* Ѱ����һ����Ч�ַ���'['��ͷ���� */
    bIsFirstValidCharOnLine = true;
    pR = pszSectionBegin;
    for (; pR < (m_pszContent + m_nSize + 1); pR++)
    {
        if (bIsFirstValidCharOnLine && *pR == '[')
        {
            break;
        }

        if (*pR == '\0')
        {
            break;
        }
//#ifdef __LINUXCONFIG__
        if (*pR == '\r' || *pR == '\n')
//#else
  //      if (*pR == '\n')
//#endif
        {
            bIsFirstValidCharOnLine = true;
            /* pszSectionEnd = pR; */
        }
        else if ((*pR != ' ') && (*pR != '\t'))
        {
            bIsFirstValidCharOnLine = false;
        }
    }

    pszSectionEnd = pR;

    return true;
}

/*************************************************
  Function:     LocateKeyRange
  Description:
        ��ָ���Ļ�������Χ������Key��������Keyƥ���ֵ�Ŀ�ʼ��ַ�ͽ�����ַ��
        ע�⣺ָ����Χ�Ľ�β��ַ�ͷ��ص�ȡֵ������ַ������ָ�����һ����Ч
    �ռ����ĵ�ַ��
  Calls:        CConfigFile::LocateKeyRange
                CConfigFile::LocateStr
                CConfigFile::MapToContent
                CConfigFile::MapToShadow
                ::ToLower
  Called By:    CConfigFile::LocateKeyRange
                CConfigFile::SetItemValue
  Input:
        pszKeyName      - ��NULL��β���ַ���ָ�룬ָʾ��Ҫ����ֵ��Key
        pszSectionBegin - ָ����Ҫ��������Ŀ�ʼ��ַ
        pszSectionEnd   - ָ����Ҫ������������һ���ַ�����һ����ַ
  Output:
        pszKeyBegin     - ����Key��Ŀ�ʼ��ַ
        pszKeyEnd       - ����Key�����һ�еĿ�ʼ�ֽڵ�ַ
                          ָ�����һ����Ч�ַ�����һ����ַ
  Return:       ��λ�ɹ�������true��ʧ�ܣ�����false
  Others:       ����������UNICODE�汾
*************************************************/
unsigned int CConfigFile::LocateKeyRange( const char *pszKeyName,
                                   const char *pszSectionBegin,
                                   const char *pszSectionEnd,
                                   char * &pszKeyBegin,
                                   char * &pszKeyEnd )
{
    char *pszLowerKey;

    /* �����Ϸ��Լ�� */
    if ((NULL == pszKeyName) || (NULL == pszSectionBegin)
        || (NULL == pszSectionEnd))
    {
        return false;
    }

    if (pszSectionBegin >= pszSectionEnd)
    {
        return false;
    }

    /* ��������룬���ò��� */
    if ((NULL == m_pszContent) || (NULL == m_pszShadow))
    {
        return false;
    }

    /* ��KeyNameת����Сд */
    pszLowerKey = strdup(pszKeyName);
    ToLower(pszLowerKey, strlen(pszLowerKey));

    /* ��Shadow�ж�λ��Ȼ�󣬼������ȷ��ָ�� */
    char    *pszKeyBeginOnShadow;

    /* �õ�Key��Shadow�е�λ�� */
    pszKeyBeginOnShadow = LocateStr( pszLowerKey,
                                     MapToShadow(pszSectionBegin),
                                     MapToShadow(pszSectionEnd) );

    if (NULL == pszKeyBeginOnShadow)
    {
        /* �ͷ���Դ */
        free(pszLowerKey);
        return false;
    }

    /* �ͷ���Դ */
    free(pszLowerKey);

    pszKeyBegin = MapToContent(pszKeyBeginOnShadow);

    pszKeyEnd = pszKeyBegin + strlen(pszKeyName);

    /* ��ָ����Χ��Ѱ�ҹؼ��ֺ����'=' */
    for (; pszKeyEnd < pszSectionEnd; pszKeyEnd++)
    {
        if ((*pszKeyEnd != ' ') && (*pszKeyEnd != '\t'))
        {
            break;
        }
    }

    if (*pszKeyEnd != '=')
    {
        /* �ҵ����ַ������ǹؼ��֣����õݹ鷽ʽ����ָ����Χ�е���һ��λ�� */
        char *pszSearchBegin;       /* ָʾ��������Ŀ�ʼλ�� */
                                    /* ������LocateKeyRange�У���pszValueBegin�޸�
                                    ��Ӱ����������Ŀ�ʼλ�� */

        pszSearchBegin = pszKeyEnd;

        return LocateKeyRange( pszKeyName,
                               pszSearchBegin,
                               pszSectionEnd,
                               pszKeyBegin,
                               pszKeyEnd );
    }

    /* ����'='������ַ� */
    for (pszKeyEnd++; pszKeyEnd < pszSectionEnd; pszKeyEnd++)
    {
//#ifdef __LINUXCONFIG__
        if ((*pszKeyEnd == '\r') || (*pszKeyEnd == '\n'))
//#else
  //      if (*pszKeyEnd == '\n')
//#endif
        {
            break;
        }
    }

    /* ��λ�����ȡֵ��Χ */
    for (; pszKeyEnd < pszSectionEnd; pszKeyEnd++)
    {
//#ifdef __LINUXCONFIG__
        if ((*pszKeyEnd != '\r') && (*pszKeyEnd != '\n'))
//#else
  //      if (*pszKeyEnd != '\n')
//#endif
        {
            break;
        }
    }

    if (pszKeyEnd > pszKeyBegin)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*************************************************
  Function:     LocateKeyValue
  Description:
        ��ָ���Ļ�������Χ������Key��������Keyƥ���ֵ�Ŀ�ʼ��ַ�ͽ�����ַ��
        ע�⣺ָ����Χ�Ľ�β��ַ�ͷ��ص�ȡֵ������ַ������ָ�����һ����Ч
    �ռ����ĵ�ַ��
  Calls:        CConfigFile::LocateKeyValue
                CConfigFile::LocateStr
                CConfigFile::MapToContent
                CConfigFile::MapToShadow
                ::ToLower
  Called By:    CConfigFile::GetItemValue
                CConfigFile::LocateKeyValue
                CConfigFile::uT_main
  Input:
        pszKeyName      - ��NULL��β���ַ���ָ�룬ָʾ��Ҫ����ֵ��Key
        pszSectionBegin - ָ����Ҫ��������Ŀ�ʼ��ַ
        pszSectionEnd   - ָ����Ҫ������������һ���ַ�����һ����ַ
  Output:
        pszValueBegin   - ���շ���ֵ�Ŀ�ʼ��ַ
        pszValueEnd     - ���շ���ֵ�Ľ�����ַ
                          ָ�����һ����Ч�ַ�����һ����ַ
  Return:       ��λ�ɹ�������true��ʧ�ܣ�����false
  Others:       ����������UNICODE�汾
*************************************************/
unsigned int CConfigFile::LocateKeyValue( const char *pszKeyName,
                                   const char *pszSectionBegin,
                                   const char *pszSectionEnd,
                                   char * &pszValueBegin,
                                   char * &pszValueEnd )
{
    char *pszLowerKey;

    /* �����Ϸ��Լ�� */
    if ((NULL == pszKeyName) || (NULL == pszSectionBegin)
        || (NULL == pszSectionEnd))
    {
        return false;
    }

    if (pszSectionBegin >= pszSectionEnd)
    {
        return false;
    }

    /* ��������룬���ò��� */
    if ((NULL == m_pszContent) || (NULL == m_pszShadow))
    {
        return false;
    }

    /* ��KeyNameת����Сд */
    pszLowerKey = strdup(pszKeyName);
    ToLower(pszLowerKey, strlen(pszLowerKey));

    /* ��Shadow�ж�λ��Ȼ�󣬼������ȷ��ָ�� */
    char    *pszKeyBeginOnShadow;

    /* �õ�Key��Shadow�е�λ�� */
    pszKeyBeginOnShadow = LocateStr( pszLowerKey,
                                     MapToShadow(pszSectionBegin),
                                     MapToShadow(pszSectionEnd) );

    if (NULL == pszKeyBeginOnShadow)
    {
        /* �ͷ���Դ */
        free(pszLowerKey);
        return false;
    }

    /* �ͷ���Դ */
    free(pszLowerKey);

    pszValueBegin = MapToContent(pszKeyBeginOnShadow) + strlen(pszKeyName);

    /* ��ָ����Χ��Ѱ�ҹؼ��ֺ����'=' */
    for (; pszValueBegin < pszSectionEnd; pszValueBegin++)
    {
        if ((*pszValueBegin != ' ') && (*pszValueBegin != '\t'))
        {
            break;
        }
    }

    if (*pszValueBegin != '=')
    {
        /* �ҵ����ַ������ǹؼ��֣����õݹ鷽ʽ����ָ����Χ�е���һ��λ�� */
        char *pszSearchBegin;       /* ָʾ��������Ŀ�ʼλ�� */
                                    /* ������LocateKeyValue�У���pszValueBegin�޸�
                                    ��Ӱ����������Ŀ�ʼλ�� */

        pszSearchBegin = pszValueBegin;

        return LocateKeyValue( pszKeyName,
                               pszSearchBegin,
                               pszSectionEnd,
                               pszValueBegin,
                               pszValueEnd );
    }

    /* ����'='����Ŀո� */
    for (pszValueBegin++; pszValueBegin < pszSectionEnd; pszValueBegin++)
    {
//#ifdef __LINUXCONFIG__
        if ((*pszValueBegin == '\r') || (*pszValueBegin == '\n')
            || ((*pszValueBegin != '\t') && (*pszValueBegin != ' ')))
//#else
  //      if ((*pszValueBegin == '\n')
    //        || ((*pszValueBegin != '\t') && (*pszValueBegin != ' ')))
//#endif
        {
            break;
        }
    }

    pszValueEnd = pszValueBegin;

    /* ���˿ո�󣬶�λ�����ȡֵ��Χ */
    for (; pszValueEnd < pszSectionEnd; pszValueEnd++)
    {
//#ifdef __LINUXCONFIG__
        if ((*pszValueEnd == '\t')
            || (*pszValueEnd == '\r') || (*pszValueEnd == '\n'))
//#else
  //      if ((*pszValueEnd == '\t')
    //        ||(*pszValueEnd == '\n'))
//#endif
        {
            break;
        }
    }

    if (pszValueEnd > pszValueBegin)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*************************************************
  Function:     LocateStr
  Description:
        �ڻ�������ָ���ķ�Χ������CharSet������CharSet�ڻ������еĿ�ʼ��ַ��
        ������������ĸ�Ĵ�Сд
        ע�⣺����������Ӧ����NULL��β���ַ�����
  Calls:
  Called By:    CConfigFile::LocateKeyValue
                CConfigFile::LocateKeyRange
                CConfigFile::LocateSection
                CConfigFile::uT_main
  Input:
        pszCharSet      - ��NULL��β���ַ���ָ�룬ָʾ��Ҫ�������ַ���
        pszBegin        - ָ����Ҫ��������Ŀ�ʼ��ַ
        pszEnd          - ָ����Ҫ������������һ���ַ�����һ����ַ
  Output:
  Return:       �����ɹ�������һ����Ч��ָ�룻ʧ�ܣ�����NULL
  Others:
*************************************************/
char *CConfigFile::LocateStr( const char *pszCharSet,
                              const char *pszBegin,
                              const char *pszEnd )
{
    char *pFind;

    /* �����Ϸ��Լ�� */
    if ((NULL == pszCharSet) || (NULL == pszBegin)
        || (NULL == pszEnd))
    {
        return NULL;
    }

    if (pszBegin >= pszEnd)
    {
        return NULL;
    }

    /* �����ַ����ڻ������е�λ�� */
    pFind = SearchMarchStr(pszBegin, pszCharSet);
	//pFind = strstr(pszBegin, pszCharSet);

    if ((NULL == pFind) || ((pFind + strlen(pszCharSet)) > pszEnd))
    {
        return NULL;
    }
    else
    {
        return pFind;
    }
}

char *CConfigFile::SearchMarchStr(const char *pszBegin, const char *pszCharSet)
{
	char *pFind;
	char *pFind1;
	const char *pTempBegin = pszBegin;


	while(1)
	{
		pFind = (char*)strstr(pTempBegin, pszCharSet);
		if (NULL == pFind)
		{
			return NULL;
		}

		if (pTempBegin < pFind)
		{
			pFind1 = pFind - 1;
//#ifdef __LINUXCONFIG__
            if (' ' != *pFind1 && '\t' != *pFind1 && '\r' != *pFind1 && '\n' != *pFind1)
//#else
//			if (' ' != *pFind1 && '\t' != *pFind1 && '\n' != *pFind1)
//#endif
			{
                pTempBegin = pFind + strlen(pszCharSet);
				continue;
			}
		}

		pFind1 = pFind + strlen(pszCharSet);
//#ifdef __LINUXCONFIG__
		if (' ' == *pFind1 || '=' == *pFind1 || '\t' == *pFind1 || '\r' == *pFind1|| '\n' == *pFind1 )
//#else
//		if (' ' == *pFind1 || '=' == *pFind1 || '\t' == *pFind1 || '\n' == *pFind1)
//#endif
		{
			return pFind;
		}
		pTempBegin = pFind + strlen(pszCharSet);
	}

	return NULL;
}

/*************************************************
  Function:     MapToContent
  Description:
        ��Shadow�ĵ�ַӳ�䵽Content��
  Calls:
  Called By:    CConfigFile::LocateKeyValue
                CConfigFile::LocateKeyRange
                CConfigFile::LocateSection
                CConfigFile::uT_main
  Input:
        p               - ָ��Shadow�еĵ�ַ
  Output:
  Return:       ���p��Shadow�е���Ч��ַ������ָ��Content�еĶ�Ӧ��ַ
  Others:
*************************************************/
char *CConfigFile::MapToContent(const char *p)
{
    return (m_pszContent + (p - m_pszShadow));
}

/*************************************************
  Function:     MapToShadow
  Description:
        ��Content�ĵ�ַӳ�䵽Shadow��
  Calls:
  Called By:    CConfigFile::LocateKeyValue
                CConfigFile::LocateKeyRange
                CConfigFile::uT_main
  Input:
        p               - ָ��Content�еĵ�ַ
  Output:
  Return:       ���p��Content�е���Ч��ַ������ָ��Shadow�еĶ�Ӧ��ַ
  Others:
*************************************************/
char *CConfigFile::MapToShadow(const char *p)
{
    return (m_pszShadow + (p - m_pszContent));
}

/*************************************************
  Function:     ToLower
  Description:
        ���ַ����еĴ�д��ĸ���Сд��ĸ��
        strlwr�ڴ���ĳЩ���ֱ���ʱ����������º��ֱ��뱻�ı�
  Calls:
  Called By:    CConfigFile::CConfielFile
                CConfigFile::LocateKeyValue
                CConfigFile::LocateKeyRange
                CConfigFile::LocateSection
                CConfigFile::uT_main
                CConfigFile::uT_SetBuffer
  Input:
        pszSrc          - ��Ҫ������ַ�����ַ
        len             - ��Ҫ����ĳ���
  Output:
        pszSrc          - ��Ŵ�����ɷ��ص�����
  Return:
  Others:
*************************************************/
void CConfigFile::ToLower( char * pszSrc, size_t len)
{
    BYTE    cb;
    size_t  i;

    if (NULL == pszSrc)
    {
        return;
    }

    for (i=0; i<len; i++)
    {
        cb = *(BYTE *)(pszSrc + i);
        if (cb >='A' && cb<='Z')
        {
            *(BYTE *)(pszSrc + i) = (BYTE)(cb + 32);
        }
    }
}

//MY_NAMESPACE_END
