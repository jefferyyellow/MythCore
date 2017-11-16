#include "parseheader.h"
#include <stdio.h>
#include "tinyxml2.h"
using namespace tinyxml2;

void CParseHeader::clear()
{
	for (int i = 0; i < mClassList.size(); ++i)
	{
		delete mClassList[i];
	}

	mClassList.clear();
	mCurClass = NULL;
	mCurLineIndex = 0;
	mCurClassIndex = 0;
	clearContent();
}

void CParseHeader::clearContent()
{
	for (int i = 0; i < mFileContent.size(); ++i)
	{
		delete[]mFileContent[i];
	}

	mFileContent.clear();
}


/// ����Ĭ��ֵ�����ļ�
bool CParseHeader::loadDefaultValueXml(const char* pXmlFile)
{
	if (NULL == pXmlFile)
	{
		return false;
	}

	tinyxml2::XMLDocument tDocument;
	if (XML_SUCCESS != tDocument.LoadFile(pXmlFile))
	{
		return false;
	}

	XMLElement* pRootElement = tDocument.RootElement();
	if (NULL == pRootElement)
	{
		return false;
	}

	XMLElement* pDefaultValue = pRootElement->FirstChildElement("DefaultValue");
	for (; NULL != pDefaultValue; pDefaultValue = pDefaultValue->NextSiblingElement("DefaultValue"))
	{
		CVariableDefault tDefault;
		const char* pType = pDefaultValue->Attribute("Type");
		if (NULL != pType)
		{
			tDefault.setType(pType);
		}

		const char* pValue = pDefaultValue->Attribute("Value");
		if (NULL != pValue)
		{
			tDefault.setValue(pValue);
		}

		mVariableDefaultList.push_back(tDefault);
	}

	return true;
}

void CParseHeader::parseHeaderFile(const char* pFilePath)
{
	if (NULL == pFilePath)
	{
		return;
	}

	clearContent();
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
	// ��̬�����ͺ���������
	if (strncmp(acWord, "static", MAX_PATH - 1) == 0)
	{
		return;
	}

	// const int i;��������������α����ģ���nStart�Ѿ�ָ��int�����λ���ˣ����˵�intǰ��
	if (strncmp(acWord, "auto", MAX_PATH - 1) != 0 &&
		strncmp(acWord, "const", MAX_PATH - 1) != 0 &&
		strncmp(acWord, "mutable", MAX_PATH - 1) != 0 &&
		strncmp(acWord, "register", MAX_PATH - 1) != 0 &&
		strncmp(acWord, "volatile", MAX_PATH - 1) != 0)
	{
		// �ص�����ǰ
		nStart = nStart - strlen(acWord);
	}

	// �õ���������
	char acVariableType[CLASS_NAME_LENGTH] = {0};
	if (!getVariableTypeName(pLine, nStart, nLineLength, acVariableType))
	{
		return;
	}

	char strType[CLASS_NAME_LENGTH] = { 0 };
	processVariableType(acVariableType, strType, sizeof(acVariableType));
	
	CPlusClass::VARIABLE_VECTOR& rVariableList = mCurClass->getVariableList();
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
	clearContent();
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
			|| pLine[i] == ';'
			|| pLine[i] == ':')
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

// �õ���һ������
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
		if (pLine[i] == ' '		// ���ַ�
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
		// /*ע��
		if (pLine[i] == '/' && i + 1 < nLineLength && pLine[i + 1] == '*')
		{
			nIndex = i;
			bEnd = true;
			break;
		}

	}

	strncpy(pWord, pLine + rStart, nIndex - rStart);
	pWord[nIndex - rStart] = '\0';

	// ���������������
	if (pLine[nIndex] == '[')
	{
		// �ҵ�����Ľ���]
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

/// �õ���һ���������Ŀ�ʼ��
bool CParseHeader::getVariableTypeName(const char* pLine, int& rStart, int nLineLength, char* pVariableName)
{
	if (NULL == pLine)
	{
		return false;
	}
	
	int nPos = -1;
	for (int i = rStart; i < nLineLength; ++ i)
	{
		// ,����Ϊ���������֮��
		if (',' == pLine[i] || ';' == pLine[i] || '[' == pLine[i])
		{
			nPos = i;
			break;
		}
	}
	// �Ҳ������źͷֺţ�һ�㲻�����
	if (-1 == nPos)
	{
		rStart = nLineLength;
		return false;
	}


	// �����ң��ҵ���һ������������Ҫ�����пո�
	bool bInName = false;
	for (int i = nPos - 1; i >= 0; -- i)
	{
		// ��һ�����ǿո��tab��
		if (pLine[i] != ' ' && pLine[i] != '\t')
		{
			bInName = true;
		}

		if (bInName)
		{
			if (pLine[i] == ' ' || pLine[i] == '\t')
			{
				strncpy(pVariableName, pLine + rStart - 1, i - rStart + 1);
				rStart = i; 
				return true;
			}
		}
	}
	strncpy(pVariableName, pLine, nPos);
	return false;
}

void CParseHeader::processVariableType(char* pSrc, char* pDst, int nLength)
{
	int nCount = 0;
	for (int i = 0; i < nLength; ++ i)
	{
		// ����
		if (pSrc[i] == '\0')
		{
			break;
		}

		// ����Ƕ���ո�����һ��ֻ��һ��
		if (pSrc[i] == ' ')
		{
			if (nCount > 0 && pDst[nCount - 1] != ' ')
			{
				pDst[nCount] = pSrc[i];
				++ nCount;
			}
			continue;
		}

		// ����ո��table����һ���ո�
		if (pSrc[i] == '\t')
		{
			if (nCount > 0 && pDst[nCount - 1] != ' ')
			{
				pDst[nCount] = ' ';
				++nCount;
			}
			continue;
		}

		pDst[nCount] = pSrc[i];
		++nCount;
	}
	pDst[nCount] = '\0';
	if (nCount <= 0)
	{
		return;
	}
	// ������һ���ǿո�,ȥ��
	if (pDst[nCount - 1] == ' ')
	{
		pDst[nCount - 1] = '\0';
	}
}

// �Ƿ��Ǻ�������
bool CParseHeader::checkFunc(const char* pLine, int nLineLength)
{
	if (NULL == pLine)
	{
		return false;
	}

	// �����ԣ���Ϊ��־
	if (strchr(pLine, '(') != NULL)
	{
		// �����������У���ʾ��������
		if (strchr(pLine, ';') != NULL)
		{
			return true;
		}

		char acBuffer[MAX_PATH] = { 0 };
		int nCurLine = mCurLineIndex;
		int nInFunction = 0;
		for (; nCurLine < (int)mFileContent.size(); ++nCurLine)
		{
			// �������������ֺ���
			// void Func(int i, float f,
			// int j);
			if (0 == nInFunction && strchr(mFileContent[nCurLine], ';') != NULL)
			{
				mCurLineIndex = nCurLine;
				return true;
			}

			// �������������ֺ���
			// void Func()
			// {
			// }
			if (strchr(mFileContent[nCurLine], '{') != NULL)
			{
				++ nInFunction;
			}

			if (strchr(mFileContent[nCurLine], '}') != NULL)
			{
				-- nInFunction;
				if (nInFunction == 0)
				{
					mCurLineIndex = nCurLine;
					return true;
				}
			}
		}
	}

	return false;
}

/// �Ƿ���ע�Ͳ���
bool CParseHeader::checkComment(const char* pLine, int nLineLength)
{
	int nStart = 0;
	
	// ����ǰ���ո�
	bool bDelete = deleteHeadSpace(pLine, nStart, nLineLength);
	
	// ��һ���Ƿ�����//��ͷ
	if (pLine[nStart] == '/' && (nStart + 1 < nLineLength) && pLine[nStart + 1] == '/')
	{
		return true;
	}

	// ��һ���Ƿ�����/*��ͷ,���ҵ���Ӧ��*/���ڵ���
	if (pLine[nStart] == '/' && (nStart + 1 < nLineLength) && pLine[nStart + 1] == '*')
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

	return false;
}

// ����true��ʾ��һ���Ѿ�����,false��ʾ���л�û����
bool CParseHeader::deleteHeadSpace(const char* pLine, int& rStart, int nLineLength)
{
	for (int i = rStart; i < nLineLength; ++i)
	{
		// ����������ֹ������ֹ��
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

// ��������ά��
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
	int nPos = 0;
	// ȥ��ǰ���ո�
	for (; i < nLength; ++i)
	{
		if (pSrc[i] == ' ' || pSrc[i] == '\t')
		{
			continue;
		}

		nPos = i;
		break;
	}

	// �ҵ������ַ�
	for (; i < nLength; ++i)
	{
		if (pSrc[i] == ' ' || pSrc[i] == ']')
		{
			break;
		}
	}

	strncpy(pMaxArrayLen, pSrc + nPos, i - nPos);
	pMaxArrayLen[i] = '\0';
}