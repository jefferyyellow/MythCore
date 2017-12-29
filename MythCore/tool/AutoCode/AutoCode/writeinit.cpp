#include "parseheader.h"

void CParseHeader::writeHeaderFile(const char* pHeadFile)
{
	if (NULL == pHeadFile)
	{
		return;
	}
	mCurClass = NULL;
	mCurLineIndex = 0;

	FILE* pFile = fopen(pHeadFile, "wt");

	char acBuffer[MAX_PATH] = { 0 };
	for (mCurLineIndex = 0; mCurLineIndex < (int)mFileContent.size(); ++mCurLineIndex)
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
	if (checkComment(pLine, nLineLength))
	{
		writeContent(pFile, nCurLineIndex, mCurLineIndex);
		if (writeGetSetMethod(pFile, pLine, nLineLength))
		{
			writeContent(pFile, mCurLineIndex, mCurLineIndex);
		}
		return;
	}

	getFirstWord(pLine, nStart, nLineLength, acWord);


	// 这行是函数的
	if (checkFunc(pLine, nLineLength))
	{
		if (NULL != strstr(pLine, "init") || NULL != strstr(pLine, "Init"))
		{
			fwrite(pLine, nLineLength, 1, pFile);
			if (strchr(pLine, ';') == NULL)
			{
				++nCurLineIndex;
				int nSpaceNum = 4;
				char acBuffer[MAX_PATH] = { 0 };
				for (; nCurLineIndex < mCurLineIndex; ++nCurLineIndex)
				{
					int nLength = strlen(mFileContent[nCurLineIndex]);
					strncpy(acBuffer, mFileContent[nCurLineIndex], nLength);
					acBuffer[nLength] = '\0';

					fwrite(mFileContent[nCurLineIndex], strlen(mFileContent[nCurLineIndex]), 1, pFile);
					const char* pBrace = strchr(acBuffer, '{');
					if (NULL != pBrace)
					{
						for (int i = 0; i < nLength; ++i)
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
				fwrite(mFileContent[mCurLineIndex], strlen(mFileContent[mCurLineIndex]), 1, pFile);
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
		// 类声明
		if (strchr(pLine, ';') != NULL)
		{
			return;
		}

		if (NULL == mCurClass)
		{
			mCurClass = mClassList[mCurClassIndex];
			++mCurClassIndex;
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


void CParseHeader::writeSourceFile(const char* pSrcFile)
{
	if (NULL == pSrcFile)
	{
		return;
	}

	mCurClass = NULL;
	mCurLineIndex = 0;

	FILE* pFile = fopen(pSrcFile, "wt");

	char acBuffer[MAX_PATH] = { 0 };
	for (mCurLineIndex = 0; mCurLineIndex < (int)mFileContent.size(); ++mCurLineIndex)
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
				fwrite(mFileContent[mCurLineIndex], strlen(mFileContent[mCurLineIndex]), 1, pFile);
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
	for (; pBegin >= pLine; --pBegin)
	{
		if (*pBegin == ' ')
		{
			break;
		}
	}

	char acClassName[MAX_PATH] = { 0 };
	++pBegin;
	CPlusClass* pClass = NULL;
	for (const char* pTemp = pBegin; pTemp <= pFunc; ++pTemp)
	{
		if (*pTemp == ':')
		{
			int nLength = pTemp - pBegin;
			strncpy(acClassName, pBegin, nLength);
			acClassName[nLength] = '\0';
			// A::init,跳过后面的:
			pClass = getClassByName(acClassName, pClass);
			++pTemp;
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
		for (int i = 0; i < (int)mClassList.size(); ++i)
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
		for (int i = 0; i < (int)rSubClassList.size(); ++i)
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

	for (int i = 0; i < (int)mVariableDefaultList.size(); ++i)
	{
		if (0 == strncmp(pVariableType, mVariableDefaultList[i].getType(), MAX_PATH))
		{
			return mVariableDefaultList[i].getValue();
		}
	}

	return NULL;
}
void CParseHeader::writeVariableInit(FILE* pFile, int nSpaceNum)
{
	char acWord[MAX_PATH] = { 0 };
	char acSpace[] = " ";
	CPlusClass::VARIABLE_VECTOR& rVariableList = mCurClass->getVariableList();
	for (int i = 0; i < (int)rVariableList.size(); ++i)
	{
		const char* pDefaultValue = getDefaultValue(rVariableList[i]->getType());
		if (NULL == pDefaultValue)
		{
			continue;
		}

		if (1 == rVariableList[i]->getArrayDimension() && 0 == strcmp(rVariableList[i]->getType(), "char"))
		{
			_snprintf_s(acWord, sizeof(acWord) - 1, "%s[0] = '\\0';\n", rVariableList[i]->getName());
		}
		else
		{
			if (rVariableList[i]->checkDefaultValue())
			{
				if (0 == strncmp(rVariableList[i]->getDefaultValue(), "ignore", MAX_PATH))
				{
					continue;
				}

				if (rVariableList[i]->getArrayDimension() > 0)
				{
					_snprintf_s(acWord, sizeof(acWord) - 1, "memset(%s, %s, sizeof(%s));\n", rVariableList[i]->getName(),
						rVariableList[i]->getDefaultValue(), rVariableList[i]->getName());
				}
				else
				{
					_snprintf_s(acWord, sizeof(acWord) - 1, "%s = %s;\n", rVariableList[i]->getName(), rVariableList[i]->getDefaultValue());
				}
			}
			else
			{
				if (rVariableList[i]->getArrayDimension() > 0)
				{
					_snprintf_s(acWord, sizeof(acWord) - 1, "memset(%s, %s, sizeof(%s));\n", rVariableList[i]->getName(),
						pDefaultValue, rVariableList[i]->getName());
				}
				else
				{
					_snprintf_s(acWord, sizeof(acWord) - 1, "%s = %s;\n", rVariableList[i]->getName(), pDefaultValue);
				}
			}
		}

		for (int j = 0; j < nSpaceNum; ++j)
		{
			fwrite(acSpace, strlen(acSpace), 1, pFile);
		}
		fwrite(acWord, strlen(acWord), 1, pFile);

	}
}

void CParseHeader::writeContent(FILE* pFile, int nStartIndex, int nEndIndex)
{
	for (; nStartIndex <= nEndIndex; ++nStartIndex)
	{
		fwrite(mFileContent[nStartIndex], strlen(mFileContent[nStartIndex]), 1, pFile);
	}
}

