#include "stdafx.h"
#include "CDirtyWord.h"
#pragma warning(disable:4996)

#define MAXWORD 80
namespace std
{
    CDirtyWord::CDirtyWord()
    {
        m_PathDirth[0] = '\0';
        m_PathCf[0] = '\0';
        m_PathEf[0] = '\0';
    }


    int CDirtyWord::IsValid(char* cpStr)
    {
        if (NULL == cpStr)
        {
            return -1;
        }
        
        int number = strlen(cpStr);
        for (int i = 0; i < number; i++)
        {
            //汉字的ASCII码为两个 负数
            if (cpStr[i] < 0)
            {
                return i + 1;
            }
        }
        return 0;

    }


    int  CDirtyWord::Getpath(const char *ccpSrcPath, char cDestPath[])
    {
        if (NULL == ccpSrcPath)
        {
            return -1;
        }
        if (NULL == cDestPath)
        {
            return -2;
        }

        char *pNewpath = (char*)malloc(strlen(ccpSrcPath) + 1);
        strncpy(pNewpath, ccpSrcPath, strlen(ccpSrcPath) + 1);
        int number = strlen(pNewpath);
        for (int i = number - 1; i > 0; i--)
        {
            if (pNewpath[i] == '\\')
            {
                break;
            }
            else
            {
                pNewpath[i] = '\0';
            }
        }
        strncpy(cDestPath, pNewpath, strlen(ccpSrcPath) + 1);
        free(pNewpath);
        return 0;
    }


    int CDirtyWord::DoIt(const char * ccpPath)
    {
        if (NULL == ccpPath)
        {
            return -1;
        }

        char cReadstr[MAXWORD];
        int i = 0;
        m_FpSour = fopen(ccpPath, "r");
        if (NULL == m_FpSour)
        {
            return 0;
        }

        Getpath(ccpPath, m_PathDirth);  
        strcat(m_PathDirth, "dirtyword.txt");
        Getpath(ccpPath, m_PathCf);
        strcat(m_PathCf, "cf.txt");
        Getpath(ccpPath, m_PathEf);
        strcat(m_PathEf, "ef.txt");

        m_FpDirty = fopen(m_PathDirth, "w");
        if (NULL == m_FpDirty)
        {
            return 0;
        }

        m_FpCf = fopen(m_PathCf, "w");
        if (NULL == m_FpDirty)
        {
            return 0;
        }

        m_FpEf = fopen(m_PathEf, "w");
        if (NULL == m_FpDirty)
        {
            return 0;
        }

        while (fgets(cReadstr, MAXWORD, m_FpSour))
        {
            fputs(cReadstr, m_FpDirty);

            if (i = IsValid(cReadstr))
            {
                int number = strlen(cReadstr);
                cReadstr[number - 1] = '\0';
                fprintf(m_FpCf, "1 %s|%c%c\n", cReadstr, cReadstr[i - 1], cReadstr[i]);
            }
            else
            {
                fprintf(m_FpEf, "1 %s", cReadstr);
            }
            memset(cReadstr, 0, MAXWORD);
        }

        fclose(m_FpSour);
        m_FpSour = NULL;
        fclose(m_FpDirty);
        m_FpDirty = NULL;
        fclose(m_FpCf);
        m_FpCf = NULL;
        fclose(m_FpEf);
        m_FpEf = NULL;

        return 0;
    }


    CDirtyWord::~CDirtyWord()
    {
        if (NULL != m_FpSour)
        {
            fclose(m_FpSour);
        }
        if (NULL != m_FpDirty)
        {
            fclose(m_FpDirty);
        }
        if (NULL != m_FpCf)
        {
            fclose(m_FpCf);
        }
        if (NULL != m_FpEf)
        {
            fclose(m_FpEf);
        }

    }

}
