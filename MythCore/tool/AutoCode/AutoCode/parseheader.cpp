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

}


void CParseHeader::parseHeaderFile(const char* pFilePath)
{
	if (NULL == pFilePath)
	{
		return;
	}

	mFileContent.clear();
	FILE* pFile = fopen(pFilePath, "r");
	char acBuffer[MAX_LINE_CHAR_NUM] = {0};

	// 读取文件的内容，保存起来
	while (fgets(acBuffer, MAX_PATH, pFile) != NULL)
	{
		char *pNewLine = new char[MAX_LINE_CHAR_NUM];
		strncpy(pNewLine, acBuffer, sizeof(acBuffer) - 1);
		pNewLine[strlen(acBuffer)] = '\0';
		mFileContent.push_back(pNewLine);
	}
	
	for (mCurLineIndex = 0; mCurLineIndex < mFileContent.size(); ++mCurLineIndex)
	{
		parseLine(mFileContent[mCurLineIndex], strlen(mFileContent[mCurLineIndex]));
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
	char acBuffer[MAX_LINE_CHAR_NUM] = { 0 };

	// 读取文件的内容，保存起来
	while (fgets(acBuffer, MAX_PATH, pFile) != NULL)
	{
		char *pNewLine = new char[MAX_LINE_CHAR_NUM];
		strncpy(pNewLine, acBuffer, sizeof(acBuffer) - 1);
		pNewLine[strlen(acBuffer)] = '\0';
		mFileContent.push_back(pNewLine);
	}
	writeSourceFile();
}

void CParseHeader::parseLine(const char* pLine, int nLineLength)
{
	if (NULL == pLine)
	{
		return;
	}
	char acWord[MAX_PATH] = {0};
	int nStart = 0;

	getFirstWord(pLine, nStart, nLineLength, acWord);
	// 这行是函数的
	if (checkFunc(pLine, nLineLength))
	{
		return;
	}

	if (nStart >= nLineLength)
	{
		return;
	}

	// 一个类开始了，开始一个新的类
	if (strncmp(acWord, "class", MAX_PATH - 1) == 0
		|| strncmp(acWord, "struct", MAX_PATH - 1) == 0)
	{
		// 分配一个新的类
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
		// 将当前的类设置为外部类
		pNewClass->setOutClass(mCurClass);
		//将新的类设置为当前类
		mCurClass = pNewClass;

		getFirstWord(pLine, nStart, nLineLength, acWord);
		mCurClass->setName(acWord);
		return;
	}

	if (strncmp(acWord, "{", MAX_PATH - 1) == 0)
	{
		return;
	}

	// 一个类结束了，将当前的类置空
	if (strncmp(acWord, "}", MAX_PATH - 1) == 0)
	{
		// 回到外层类
		mCurClass = mCurClass->getOutClass();
		return;
	}
	// 没有当前类，可能在各个类之间的空隙的代码，不处理，之间返回
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

	// 静态变量不能在init里初始化,所以不管是静态函数还是静态变量，直接跳过
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
		getFirstWord(pLine, nStart, nLineLength, acWord);
		if (strncmp(acWord, "", MAX_PATH - 1) == 0 )
		{
			break;
		}

		CVariable* pNewVariable = new CVariable;
		pNewVariable->setType(strType);
		pNewVariable->setName(acWord);
		rVariableList.push_back(pNewVariable);

		if (nStart >= nLineLength)
		{
			break;
		}
	}

}


// 返回false表示是
bool CParseHeader::deleteHeadSpace(const char* pLine, int& rStart, int nLineLength)
{
	for (int i = rStart; i < nLineLength; ++i)
	{
		if (pLine[i] != ' '&& pLine[i] != '\t' && pLine[i] != '\n')
		{
			rStart = i;
			return false;
		}
	}
	rStart = nLineLength;
	return true;
}

void CParseHeader::getFirstWord(const char* pLine, int& rStart, int nLineLength, char* pWord)
{
	// 删除前导空格
	bool bDelete = deleteHeadSpace(pLine, rStart, nLineLength);
	if (bDelete)
	{
		pWord[0] = '\0';
		rStart = nLineLength;
		return;
	}

	int nIndex = 0;
	bool bEnd = false;
	for (int i = rStart; i < nLineLength; ++ i)
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
		// //注释
		if (pLine[i] == '/' && i + 1 < nLineLength && pLine[i + 1]== '/')
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

bool CParseHeader::checkFunc(const char* pLine, int nLineLength)
{
	if (strchr(pLine, '(') != NULL)
	{
		// 函数声明
		if (strchr(pLine, ';') != NULL)
		{
			return true;
		}

		char acBuffer[MAX_PATH] = { 0 };
		int nCurLine = mCurLineIndex;
		bool bInFunction = false;
		for (; nCurLine < mFileContent.size(); ++nCurLine)
		{
			strncpy(acBuffer, mFileContent[nCurLine], sizeof(acBuffer) - 1);
			acBuffer[strlen(mFileContent[nCurLine])] = '\0';
			if (!bInFunction && strchr(acBuffer, ';') != NULL)
			{
				mCurLineIndex = nCurLine;
				return true;
			}

			if (strchr(acBuffer, '{') != NULL)
			{
				bInFunction  = true;
			}


			if (strchr(acBuffer, '}') != NULL)
			{
				mCurLineIndex  = nCurLine;
				return true;
			}
		}
	}

	return false;
}

void CParseHeader::writeHeaderFile()
{
	mCurClass = NULL;
	mCurLineIndex = 0;

	FILE* pFile = fopen("test_back.h", "wt");

	char acBuffer[MAX_PATH] = { 0 };
	for (mCurLineIndex = 0; mCurLineIndex < mFileContent.size(); ++mCurLineIndex)
	{
		writeHeaderLine(pFile, mFileContent[mCurLineIndex], strlen(mFileContent[mCurLineIndex]));
	}

	fclose(pFile);
}

void CParseHeader::writeHeaderLine(FILE* pFile, const char* pLine, int nLineLength)
{
	char acWord[MAX_PATH] = { 0 };
	int nStart = 0;
	int nCurLineIndex = mCurLineIndex;

	getFirstWord(pLine, nStart, nLineLength, acWord);
	// 这行是函数的
	if (checkFunc(pLine, nLineLength))
	{
		if (NULL != strstr(pLine, "init") || NULL != strstr(pLine, "Init"))
		{
			fwrite(pLine, nLineLength, 1, pFile);
			if (strchr(pLine, ';') == NULL)
			{
				++ nCurLineIndex;
				int nSpaceNum = 4;
				char acBuffer[MAX_PATH] = { 0 };
				for (; nCurLineIndex < mCurLineIndex; ++nCurLineIndex)
				{
					fwrite(mFileContent[nCurLineIndex], strlen(mFileContent[nCurLineIndex]), 1, pFile);
					const char* pBrace = strchr(acBuffer, '{');
					if (NULL != pBrace)
					{
						int nLineLength = strlen(mFileContent[nCurLineIndex]);
						for (int i = 0; i < nLineLength; ++ i)
						{
							if (acBuffer[i] == '{')
							{
								break;
							}
							if (acBuffer[i] == ' ')
							{
								nSpaceNum ++;
							}
							if (acBuffer[i] == '\t')
							{
								nSpaceNum += 4;
							}
						}
						break;
					}
				}

				writeVariableInit(pFile, nSpaceNum);
			}
		}
		else
		{
			writeContent(pFile, nCurLineIndex, mCurLineIndex);
		}
		return;
	}

	fwrite(pLine, nLineLength, 1, pFile);

	// 一个类开始了，开始一个新的类
	if (strncmp(acWord, "class", MAX_PATH - 1) == 0
		|| strncmp(acWord, "struct", MAX_PATH - 1) == 0)
	{
		if (NULL == mCurClass)
		{
			mCurClass = mClassList[mCurClassIndex];
			++ mCurClassIndex;
		}
		else
		{
			CPlusClass::SUB_CLASS_VECTOR& rSubClassList = mCurClass->getSubClassList();
			mCurClass = rSubClassList[mCurClass->getCurClassIndex()];
			mCurClass->setCurClassIndex(mCurClass->getCurClassIndex() + 1);
		}
		return;
	}


	// 一个类结束了，将当前的类置空
	if (strncmp(acWord, "}", MAX_PATH - 1) == 0)
	{
		// 回到外层类
		mCurClass = mCurClass->getOutClass();
		return;
	}

}

void CParseHeader::writeVariableInit(FILE* pFile, int nSpaceNum)
{
	char acWord[MAX_PATH] = { 0 };
	char acSpace[] = " ";
	CPlusClass::VARIABLE_VECTOR& rVariableList = mCurClass->getVariableList();
	for (int i = 0; i < rVariableList.size(); ++ i)
	{
		const char* pDefaultValue = getDefaultValue(rVariableList[i]->getType());
		if (NULL == pDefaultValue)
		{
			continue;
		}

		for (int j = 0; j < nSpaceNum; ++ j)
		{
			fwrite(acSpace, strlen(acSpace), 1, pFile);
		}
		_snprintf_s(acWord, sizeof(acWord) - 1, "%s = %s;\n", rVariableList[i]->getName(), pDefaultValue);
		fwrite(acWord, strlen(acWord), 1, pFile);

	}
	for (int j = 0; j < nSpaceNum - 4; ++j)
	{
		fwrite(acSpace, strlen(acSpace), 1, pFile);
	}
	fwrite("}\n", strlen("}\n"), 1, pFile);

}

void CParseHeader::writeContent(FILE* pFile, int nStartIndex, int nEndIndex)
{
	char acBuffer[MAX_PATH] = { 0 };

	for (; nStartIndex <= nEndIndex; ++nStartIndex)
	{
		fwrite(mFileContent[nStartIndex], strlen(mFileContent[nStartIndex]), 1, pFile);
	}
}

void CParseHeader::writeSourceFile()
{
	mCurClass = NULL;
	mCurLineIndex = 0;

	FILE* pFile = fopen("test_back.cpp", "wt");

	char acBuffer[MAX_PATH] = { 0 };
	for (mCurLineIndex = 0; mCurLineIndex < mFileContent.size(); ++mCurLineIndex)
	{
		writeSourceLine(pFile, mFileContent[mCurLineIndex], strlen(mFileContent[mCurLineIndex]));
	}

	fclose(pFile);
}

void CParseHeader::writeSourceLine(FILE* pFile, const char* pLine, int nLineLength)
{
	char acWord[MAX_PATH] = { 0 };
	int nStart = 0;
	int nCurLineIndex = mCurLineIndex;

	getFirstWord(pLine, nStart, nLineLength, acWord);
	// 这行是函数的
	if (checkFunc(pLine, nLineLength))
	{
		if (NULL != strstr(pLine, "::init") || NULL != strstr(pLine, "::Init"))
		{
			fwrite(pLine, nLineLength, 1, pFile);
			mCurClass = getClass(pLine, nLineLength);
			if (NULL != mCurClass)
			{
				++nCurLineIndex;
				int nSpaceNum = 4;
				char acBuffer[MAX_PATH] = { 0 };
				for (; nCurLineIndex < mCurLineIndex; ++nCurLineIndex)
				{
					fwrite(mFileContent[nCurLineIndex], strlen(mFileContent[nCurLineIndex]), 1, pFile);
					const char* pBrace = strchr(acBuffer, '{');
					if (NULL != pBrace)
					{
						int nLineLength = strlen(mFileContent[nCurLineIndex]);
						for (int i = 0; i < nLineLength; ++i)
						{
							if (acBuffer[i] == '{')
							{
								break;
							}
							if (acBuffer[i] == ' ')
							{
								nSpaceNum++;
							}
							if (acBuffer[i] == '\t')
							{
								nSpaceNum += 4;
							}
						}
						break;
					}
				}

				writeVariableInit(pFile, nSpaceNum);
			}
			else
			{
				writeContent(pFile, nCurLineIndex, mCurLineIndex);
			}
		}
		else
		{
			writeContent(pFile, nCurLineIndex, mCurLineIndex);
		}
		return;
	}

	fwrite(pLine, nLineLength, 1, pFile);
}

CPlusClass* CParseHeader::getClass(const char* pLine, int nLineLength)
{
	const char* pFunc = strstr(pLine, "::init");
	if (NULL == pFunc)
	{
		pFunc = strstr(pLine, "::Init");
	}
	if (NULL == pFunc)
	{
		return NULL;
	}

	if (pFunc == pLine)
	{
		return NULL;
	}
	const char* pBegin = pFunc - 1;
	for (; pBegin >= pLine; -- pBegin)
	{
		if (*pBegin == ' ')
		{
			break;
		}
	}

	char acClassName[MAX_PATH] = {0};
	++ pBegin;
	CPlusClass* pClass = NULL;
	for (const char* pTemp = pBegin; pTemp <= pFunc; ++ pTemp)
	{
		if (*pTemp == ':')
		{
			int nLength = pTemp - pBegin;
			strncpy(acClassName, pBegin, nLength);
			acClassName[nLength] = '\0';
			// A::init,跳过后面的:
			pClass = getClassByName(acClassName, pClass);
			++ pTemp;
			pBegin = pTemp + 1;
		}
	}
	return pClass;
}

CPlusClass* CParseHeader::getClassByName(char* pClassName, CPlusClass* pOutClass)
{
	if (NULL == pClassName)
	{
		return NULL;
	}
	if (NULL == pOutClass)
	{
		for (int i = 0; i < mClassList.size(); ++ i)
		{
			if (0 == strncmp(pClassName, mClassList[i]->getName(), MAX_PATH))
			{
				return mClassList[i];
			}
		}
	}
	else
	{
		CPlusClass::SUB_CLASS_VECTOR& rSubClassList = mCurClass->getSubClassList();
		for (int i = 0; i < rSubClassList.size(); ++i)
		{
			if (0 == strncmp(pClassName, rSubClassList[i]->getName(), MAX_PATH))
			{
				return rSubClassList[i];
			}
		}
	}

	return NULL;
}

const char* CParseHeader::getDefaultValue(const char* pVariableType)
{
	if (NULL == pVariableType)
	{
		return NULL;
	}
	if (NULL != strchr(pVariableType, '*'))
	{
		return "NULL";
	}

	for (int i = 0; i < mVariableDefaultList.size(); ++ i)
	{
		if (0 == strncmp(pVariableType, mVariableDefaultList[i].getType(), MAX_PATH))
		{
			return mVariableDefaultList[i].getValue();
		}
	}

	return NULL;
}