#include "parseheader.h"
#include <stdio.h>

void CParseHeader::init()
{
	CVariableDefault tTemp;

	tTemp.setType("int");
	tTemp.setValue("0");
	mVariableDefaultList.push_back(tTemp);

	tTemp.setType("float");
	tTemp.setValue("0.0f");
	mVariableDefaultList.push_back(tTemp);

	tTemp.setType("char");
	tTemp.setValue("0");
	mVariableDefaultList.push_back(tTemp);
}


void CParseHeader::parseHeaderFile(const char* pFilePath)
{
	if (NULL == pFilePath)
	{
		return;
	}

	mFileContent.clear();
	FILE* pFile = fopen(pFilePath, "r");
	if (NULL == pFile)
	{
		return;
	}
	char acBuffer[MAX_LINE_CHAR_NUM] = {0};

	// ��ȡ�ļ������ݣ���������
	while (fgets(acBuffer, MAX_PATH, pFile) != NULL)
	{
		char *pNewLine = new char[MAX_LINE_CHAR_NUM];
		strncpy(pNewLine, acBuffer, sizeof(acBuffer) - 1);
		pNewLine[strlen(acBuffer)] = '\0';
		mFileContent.push_back(pNewLine);
	}
	
	for (mCurLineIndex = 0; mCurLineIndex < (int)mFileContent.size(); ++mCurLineIndex)
	{
		parseLine(mFileContent[mCurLineIndex], strlen(mFileContent[mCurLineIndex]));
	}

}

void CParseHeader::parseLine(const char* pLine, int nLineLength)
{
	if (NULL == pLine)
	{
		return;
	}

	if (checkComment(pLine, nLineLength))
	{
		return;
	}

	// �����Ǻ�����
	if (checkFunc(pLine, nLineLength))
	{
		return;
	}


	char acWord[MAX_PATH] = { 0 };
	int nStart = 0;

	getFirstWord(pLine, nStart, nLineLength, acWord);
	if (nStart >= nLineLength)
	{
		return;
	}

	// һ���࿪ʼ�ˣ���ʼһ���µ���
	if (strncmp(acWord, "class", MAX_PATH - 1) == 0
		|| strncmp(acWord, "struct", MAX_PATH - 1) == 0)
	{
		// ����һ���µ���
		CPlusClass* pNewClass = new CPlusClass;
		if (NULL == mCurClass)
		{
			mClassList.push_back(pNewClass);
		}
		else
		{
			CPlusClass::SUB_CLASS_VECTOR& rSubClassList = mCurClass->getSubClassList();
			rSubClassList.push_back(pNewClass);
		}
		// ����ǰ��������Ϊ�ⲿ��
		pNewClass->setOutClass(mCurClass);
		//���µ�������Ϊ��ǰ��
		mCurClass = pNewClass;

		getFirstWord(pLine, nStart, nLineLength, acWord);
		mCurClass->setName(acWord);
		return;
	}

	if (strncmp(acWord, "{", MAX_PATH - 1) == 0)
	{
		return;
	}

	// һ��������ˣ�����ǰ�����ÿ�
	if (strncmp(acWord, "}", MAX_PATH - 1) == 0)
	{
		// �ص������
		mCurClass = mCurClass->getOutClass();
		return;
	}
	// û�е�ǰ�࣬�����ڸ�����֮��Ŀ�϶�Ĵ��룬������֮�䷵��
	if (NULL == mCurClass)
	{
		return;
	}


	if (strncmp(acWord, "typedef", strlen("typedef")) == 0 ||
		strncmp(acWord, "friend", strlen("friend")) == 0 ||
		strncmp(acWord, "public", strlen("public")) == 0 ||
		strncmp(acWord, "private", strlen("private")) == 0 ||
		strncmp(acWord, "protect", strlen("protect")) == 0)
	{
		return;
	}

	// ��̬����������init���ʼ��,���Բ����Ǿ�̬�������Ǿ�̬������ֱ������
	if (strncmp(acWord, "static", MAX_PATH - 1) == 0 ||
		strncmp(acWord, "auto", MAX_PATH - 1) == 0 ||
		strncmp(acWord, "const", MAX_PATH - 1) == 0 ||
		strncmp(acWord, "mutable", MAX_PATH - 1) == 0 ||
		strncmp(acWord, "register", MAX_PATH - 1) == 0 ||
		strncmp(acWord, "static", MAX_PATH - 1) == 0 ||
		strncmp(acWord, "volatile", MAX_PATH - 1) == 0)
	{
		getFirstWord(pLine, nStart, nLineLength, acWord);
		if (nStart >= nLineLength)
		{
			return;
		}
	}

	CPlusClass::VARIABLE_VECTOR& rVariableList = mCurClass->getVariableList();
	char strType[TYPE_NAME_LENGTH];
	strncpy(strType, acWord, sizeof(strType) - 1);


	while (true)
	{
		getFirstVariable(pLine, nStart, nLineLength, acWord);
		if (strncmp(acWord, "", MAX_PATH - 1) == 0)
		{
			break;
		}

		CVariable* pNewVariable = new CVariable;
		pNewVariable->setType(strType);
		pNewVariable->setName(acWord);
		pNewVariable->setArrayDimension(calcArrayDimension(pNewVariable, pLine, nLineLength));

		char acBuffer[MAX_PATH] = {};
		bool bRet = getDefaultValue(mFileContent[mCurLineIndex - 1], strlen(mFileContent[mCurLineIndex - 1]), acBuffer);
		if (bRet)
		{
			pNewVariable->setDefaultValue(acBuffer);
		}
		rVariableList.push_back(pNewVariable);

		if (nStart >= nLineLength)
		{
			break;
		}
	}

}


void CParseHeader::parseSourceFile(const char* pFilePath)
{
	if (NULL == pFilePath)
	{
		return;
	}
	mFileContent.clear();
	FILE* pFile = fopen(pFilePath, "r");
	if (NULL == pFile)
	{
		return;
	}
	char acBuffer[MAX_LINE_CHAR_NUM] = { 0 };

	// ��ȡ�ļ������ݣ���������
	while (fgets(acBuffer, MAX_PATH, pFile) != NULL)
	{
		char *pNewLine = new char[MAX_LINE_CHAR_NUM];
		strncpy(pNewLine, acBuffer, sizeof(acBuffer) - 1);
		pNewLine[strlen(acBuffer)] = '\0';
		mFileContent.push_back(pNewLine);
	}
	//writeSourceFile();
}

void CParseHeader::getFirstWord(const char* pLine, int& rStart, int nLineLength, char* pWord)
{
	// ɾ��ǰ���ո�
	bool bDelete = deleteHeadSpace(pLine, rStart, nLineLength);
	if (bDelete)
	{
		pWord[0] = '\0';
		rStart = nLineLength;
		return;
	}

	int nIndex = 0;
	bool bEnd = false;
	for (int i = rStart; i < nLineLength; ++i)
	{
		if (pLine[i] == ' '
			|| pLine[i] == '\t'
			|| pLine[i] == '\n'
			|| pLine[i] == ','
			|| pLine[i] == ';')
		{
			nIndex = i;
			break;
		}
		// //ע��
		if (pLine[i] == '/' && i + 1 < nLineLength && pLine[i + 1] == '/')
		{
			nIndex = i;
			bEnd = true;
			break;
		}

		if (pLine[i] == '/' && i + 1 < nLineLength && pLine[i + 1] == '*')
		{
			nIndex = i;
			bEnd = true;
			break;
		}

	}

	strncpy(pWord, pLine + rStart, nIndex - rStart);
	pWord[nIndex - rStart] = '\0';
	rStart = nIndex + 1;
	if (bEnd)
	{
		rStart = nLineLength;
	}
}

void CParseHeader::getFirstVariable(const char* pLine, int& rStart, int nLineLength, char* pWord)
{
	// ɾ��ǰ���ո�
	bool bDelete = deleteHeadSpace(pLine, rStart, nLineLength);
	if (bDelete)
	{
		pWord[0] = '\0';
		rStart = nLineLength;
		return;
	}

	int nIndex = 0;
	bool bEnd = false;
	for (int i = rStart; i < nLineLength; ++i)
	{
		if (pLine[i] == ' '		// ����ַ�
			|| pLine[i] == '\t'	// ����ַ�
			|| pLine[i] == '\n'	// �н���
			|| pLine[i] == ','	// ��������
			|| pLine[i] == ';'	// ������
			|| pLine[i] == '[') // ���鿪ʼ
		{
			nIndex = i;
			break;
		}
		// //ע��
		if (pLine[i] == '/' && i + 1 < nLineLength && pLine[i + 1] == '/')
		{
			nIndex = i;
			bEnd = true;
			break;
		}

		if (pLine[i] == '/' && i + 1 < nLineLength && pLine[i + 1] == '*')
		{
			nIndex = i;
			bEnd = true;
			break;
		}

	}

	strncpy(pWord, pLine + rStart, nIndex - rStart);
	pWord[nIndex - rStart] = '\0';

	if (pLine[nIndex] == '[')
	{
		for (int i = nIndex + 1; i < nLineLength; ++i)
		{
			if (pLine[i] == ']')
			{
				nIndex = i;
			}
		}
	}

	rStart = nIndex + 1;
	if (bEnd)
	{
		rStart = nLineLength;
	}
}

bool CParseHeader::checkFunc(const char* pLine, int nLineLength)
{
	if (strchr(pLine, '(') != NULL)
	{
		// ��������
		if (strchr(pLine, ';') != NULL)
		{
			return true;
		}

		char acBuffer[MAX_PATH] = { 0 };
		int nCurLine = mCurLineIndex;
		bool bInFunction = false;
		for (; nCurLine < (int)mFileContent.size(); ++nCurLine)
		{
			if (!bInFunction && strchr(mFileContent[nCurLine], ';') != NULL)
			{
				mCurLineIndex = nCurLine;
				return true;
			}

			if (strchr(mFileContent[nCurLine], '{') != NULL)
			{
				bInFunction  = true;
			}


			if (strchr(mFileContent[nCurLine], '}') != NULL)
			{
				mCurLineIndex  = nCurLine;
				return true;
			}
		}
	}

	return false;
}

bool CParseHeader::checkComment(const char* pLine, int nLineLength)
{
	for (int i = 0; i < nLineLength; ++i)
	{
		if (pLine[i] == '/' && (i + 1 < nLineLength) && pLine[i + 1] == '/')
		{
			return true;
		}
		// /*
		if (pLine[i] == '/' && (i + 1 < nLineLength) && pLine[i + 1] == '*')
		{
			int nCurLine = mCurLineIndex;
			for (; nCurLine < (int)mFileContent.size(); ++nCurLine)
			{
				if (NULL != strstr(mFileContent[nCurLine], "*/"))
				{
					mCurLineIndex = nCurLine;
					return true;
				}
			}
		}
	}

	return false;
}

// ����false��ʾ��
bool CParseHeader::deleteHeadSpace(const char* pLine, int& rStart, int nLineLength)
{
	for (int i = rStart; i < nLineLength; ++i)
	{
		if (pLine[i] != ' '
			&& pLine[i] != '\t' 
			&& pLine[i] != '\n'
			&& pLine[i] != ','
			&& pLine[i] != ';')
		{
			rStart = i;
			return false;
		}
	}
	rStart = nLineLength;
	return true;
}

bool CParseHeader::getDefaultValue(const char* pLine, int nLineLength, char* pDefaultValue)
{
	bool bComment = false;
	int nStart = 0;
	for (int i = 0; i < nLineLength; ++ i)
	{
		if (pLine[i] == '/' && i + 1 < nLineLength && pLine[i + 1] == '/')
		{
			bComment = true;
			nStart = i;
			break;
		}

		if (pLine[i] == '/' && i + 1 < nLineLength && pLine[i + 1] == '*')
		{
			bComment = true;
			nStart = i;
			break;
		}
	}

	if (!bComment)
	{
		return false;
	}

	const char* pPos = strstr(pLine, "default");
	if (NULL == pPos)
	{
		return false;
	}
	
	pPos += strlen("default:");
	nStart = pPos - pLine;
	getFirstWord(pLine, nStart, nLineLength, pDefaultValue);
	return true;
}


int CParseHeader::calcArrayDimension(CVariable* pVariable, const char* pLine, int nLineLength)
{
	int nCount = 0;
	for (int i = 0; i < nLineLength; ++ i)
	{
		if (pLine[i] == '[')
		{
			getMaxArrayLen((pLine + i + 1), nLineLength - i - 1, pVariable->getArrayMaxLen(nCount));
			++ nCount;
		}
	}

	pVariable->setArrayDimension(nCount);
	return nCount;
}

/// �õ�����ά������
void CParseHeader::getMaxArrayLen(const char* pSrc, int nLength, char* pMaxArrayLen)
{
	int i = 0;
	for (; i < nLength; ++i)
	{
		if (pSrc[i] != ' ')
		{
			break;
		}
	}

	for (; i < nLength; ++ i)
	{
		if (pSrc[i] == ' ' || pSrc[i] == ']')
		{
			strncpy(pMaxArrayLen, pSrc, i);
			pMaxArrayLen[i] = '\0';
		}
	}
}