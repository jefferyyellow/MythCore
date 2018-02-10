#include "parseheader.h"

bool CParseHeader::writeGetSetMethod(FILE* pFile, const char* pLine, int nLineLength)
{
	const char* pPos = strstr(pLine, "autocode");
	if (NULL == pPos)
	{
		return false;
	}
	int nSpaceNum = 0;
	for (int i = 0; i < nLineLength; ++i)
	{
		if (pLine[i] == '/')
		{
			break;
		}
		if (pLine[i] == ' ')
		{
			nSpaceNum++;
		}
		if (pLine[i] == '\t')
		{
			nSpaceNum += 4;
		}
	}


	int nCurLine = mCurLineIndex + 1;
	for (; nCurLine < (int)mFileContent.size(); ++nCurLine)
	{
		if (NULL != strstr(mFileContent[nCurLine], "autocode"))
		{
			mCurLineIndex = nCurLine;
			break;
		}
	}

	int nCount = 0;
	char acWord[MAX_PATH] = { 0 };
	CPlusClass::VARIABLE_VECTOR& rVariableList = mCurClass->getVariableList();
	for (int i = 0; i < (int)rVariableList.size(); ++i)
	{
		if (0 == strncmp(rVariableList[i]->getDefaultValue(), "ignore", MAX_PATH))
		{
			continue;
		}


		bool bBuiltIn = false;
		const char* pDefaultValue = rVariableList[i]->getDefaultValue();
		if (pDefaultValue[0] != '\0')
		{
			bBuiltIn = true;
		}
		else
		{
			const char* pDefaultValue = getDefaultValue(rVariableList[i]->getType());
			// 内置类型且不是数组
			if (NULL != pDefaultValue)
			{
				bBuiltIn = true;
			}
		}


		switch (rVariableList[i]->getArrayDimension())
		{
			case 0:
			{
				writeBuiltInType(bBuiltIn, rVariableList[i], pFile, nSpaceNum, nCount);
				break;
			}
			case 1:
			{
				if (0 == strncmp("char", rVariableList[i]->getType(), TYPE_NAME_LENGTH))
				{
					writeCharArray(rVariableList[i], pFile, nSpaceNum, nCount);
				}
				else
				{
					writeBuiltInArray(bBuiltIn, rVariableList[i], pFile, nSpaceNum, nCount);
				}
				break;
			}
			case 2:
			{
				if (0 == strncmp("char", rVariableList[i]->getType(), TYPE_NAME_LENGTH))
				{
					writeCharArray2(rVariableList[i], pFile, nSpaceNum, nCount);
				}
				else
				{
					writeBuiltInArray2(bBuiltIn, rVariableList[i], pFile, nSpaceNum, nCount);
				}
				break;
			}
			default:
				break;
		}
	}
	return true;
}

// 写非数组的内置类型
void CParseHeader::writeBuiltInType(bool bBuiltIn, CVariable* pVariable, FILE* pFile, int nSpaceNum, int &rCount)
{
	if (NULL == pVariable || NULL == pFile)
	{
		return;
	}
	const char* pName = getFirstUpcase(pVariable->getName());
	if (NULL == pName)
	{
		return;
	}
	char acBuffer[MAX_PATH] = { 0 };
	if (rCount != 0)
	{
		// 空行
		writeSpaceLine(acBuffer, 1);
	}

	if (bBuiltIn)
	{
		// 写入前导空格
		writeSpace(acBuffer, nSpaceNum);
		_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s%s get%s(){ return %s;}\n", acBuffer, pVariable->getType(), pName, pVariable->getName());

		// 写入前导空格
		writeSpace(acBuffer, nSpaceNum);
		_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%svoid set%s(%s value){ %s = value;}\n", acBuffer, pName, pVariable->getType(), pVariable->getName());
	}
	else
	{
		// 写入前导空格
		writeSpace(acBuffer, nSpaceNum);
		_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s%s& get%s(){ return %s;}\n", acBuffer, pVariable->getType(), pName, pVariable->getName());
	}
	fwrite(acBuffer, strlen(acBuffer), 1, pFile);
	++rCount;
}

// 写内置数组
void CParseHeader::writeBuiltInArray(bool bBuiltIn, CVariable* pVariable, FILE* pFile, int nSpaceNum, int &rCount)
{
	if (NULL == pVariable || NULL == pFile)
	{
		return;
	}
	const char* pName = getFirstUpcase(pVariable->getName());
	if (NULL == pName)
	{
		return;
	}

	char acWord[MAX_LINE_CHAR_NUM] = { 0 };

	if (rCount != 0)
	{
		// 空行
		writeSpaceLine(acWord, 1);
	}

	// **** Type getName(int nIndex) ****
	// 写入前导空格
	writeSpace(acWord, nSpaceNum);
	if (bBuiltIn)
	{
		_snprintf_s(acWord, sizeof(acWord)-1, "%s%s get%s(int nIndex)\n", acWord, pVariable->getType(), pName);
	}
	else
	{
		_snprintf_s(acWord, sizeof(acWord)-1, "%s%s* get%s(int nIndex)\n", acWord, pVariable->getType(), pName);
	}

	writeSpace(acWord, nSpaceNum);
	strcat(acWord, "{\n");

	nSpaceNum += 4;
	writeSpace(acWord, nSpaceNum);
	_snprintf_s(acWord, sizeof(acWord) - 1, "%sif(nIndex < 0 || nIndex >= %s)\n", acWord, pVariable->getArrayMaxLen(0));

	writeSpace(acWord, nSpaceNum);
	strcat(acWord, "{\n");
	
	nSpaceNum += 4;
	writeSpace(acWord, nSpaceNum);
	if (bBuiltIn)
	{
		if (pVariable->checkDefaultValue())
		{
			_snprintf_s(acWord, sizeof(acWord)-1, "%sreturn %s;\n", acWord, pVariable->getDefaultValue());
		}
		else
		{
			_snprintf_s(acWord, sizeof(acWord)-1, "%sreturn %s;\n", acWord, getDefaultValue(pVariable->getType()));
		}
	}
	else
	{
		_snprintf_s(acWord, sizeof(acWord)-1, "%sreturn NULL;\n", acWord);
	}




	nSpaceNum -= 4;
	writeSpace(acWord, nSpaceNum);
	strcat(acWord, "}\n");

	writeSpace(acWord, nSpaceNum);
	if (bBuiltIn)
	{
		_snprintf_s(acWord, sizeof(acWord)-1, "%sreturn %s[nIndex];\n", acWord, pVariable->getName());
	}
	else
	{
		_snprintf_s(acWord, sizeof(acWord)-1, "%sreturn &%s[nIndex];\n", acWord, pVariable->getName());
	}

	nSpaceNum -= 4;
	writeSpace(acWord, nSpaceNum);
	strcat(acWord, "}\n");
	// **** end Type getName(int nIndex) ****


	// **** Type setName(int nIndex) ****
	writeSpace(acWord, nSpaceNum);
	if (bBuiltIn)
	{
		_snprintf_s(acWord, sizeof(acWord)-1, "%svoid set%s(int nIndex, %s value)\n", acWord, pName, pVariable->getType());
	}
	else
	{
		_snprintf_s(acWord, sizeof(acWord)-1, "%svoid set%s(int nIndex, %s& value)\n", acWord, pName, pVariable->getType());
	}

	writeSpace(acWord, nSpaceNum);
	strcat(acWord, "{\n");

	nSpaceNum += 4;
	writeSpace(acWord, nSpaceNum);
	_snprintf_s(acWord, sizeof(acWord) - 1, "%sif(nIndex < 0 || nIndex >= %s)\n", acWord, pVariable->getArrayMaxLen(0));

	writeSpace(acWord, nSpaceNum);
	strcat(acWord, "{\n");

	nSpaceNum += 4;
	writeSpace(acWord, nSpaceNum);
	_snprintf_s(acWord, sizeof(acWord) - 1, "%sreturn;\n", acWord);


	nSpaceNum -= 4;
	writeSpace(acWord, nSpaceNum);
	strcat(acWord, "}\n");

	writeSpace(acWord, nSpaceNum);
	_snprintf_s(acWord, sizeof(acWord) - 1, "%s%s[nIndex] = value;\n", acWord, pVariable->getName());

	nSpaceNum -= 4;
	writeSpace(acWord, nSpaceNum);
	strcat(acWord, "}\n");

	// **** end Type setName(int nIndex) ****

	fwrite(acWord, strlen(acWord), 1, pFile);


	++rCount;
}

/// 写二维内置类型数组
void CParseHeader::writeBuiltInArray2(bool bBuiltIn, CVariable* pVariable, FILE* pFile, int nSpaceNum, int &rCount)
{
	if (NULL == pVariable || NULL == pFile)
	{
		return;
	}
	const char* pName = getFirstUpcase(pVariable->getName());
	if (NULL == pName)
	{
		return;
	}

	char acWord[MAX_LINE_CHAR_NUM] = { 0 };

	if (rCount != 0)
	{
		// 空行
		writeSpaceLine(acWord, 1);
	}

	// **** Type getName(int nIndex) ****
	// 写入前导空格
	writeSpace(acWord, nSpaceNum);
	if (bBuiltIn)
	{
		_snprintf_s(acWord, sizeof(acWord)-1, "%s%s get%s(int nIndex, int nIndex2)\n", acWord, pVariable->getType(), pName);
	}
	else
	{
		_snprintf_s(acWord, sizeof(acWord)-1, "%s%s* get%s(int nIndex, int nIndex2)\n", acWord, pVariable->getType(), pName);
	}

	writeSpace(acWord, nSpaceNum);
	strcat(acWord, "{\n");

	nSpaceNum += 4;
	writeSpace(acWord, nSpaceNum);
	_snprintf_s(acWord, sizeof(acWord)-1, "%sif(nIndex < 0 || nIndex >= %s)\n", acWord, pVariable->getArrayMaxLen(0));

	writeSpace(acWord, nSpaceNum);
	strcat(acWord, "{\n");

	nSpaceNum += 4;
	writeSpace(acWord, nSpaceNum);
	if (bBuiltIn)
	{
		if (pVariable->checkDefaultValue())
		{
			_snprintf_s(acWord, sizeof(acWord)-1, "%sreturn %s;\n", acWord, pVariable->getDefaultValue());
		}
		else
		{
			_snprintf_s(acWord, sizeof(acWord)-1, "%sreturn %s;\n", acWord, getDefaultValue(pVariable->getType()));
		}
	}
	else
	{
		_snprintf_s(acWord, sizeof(acWord)-1, "%sreturn NULL;\n", acWord, getDefaultValue(pVariable->getType()));
	}




	nSpaceNum -= 4;
	writeSpace(acWord, nSpaceNum);
	strcat(acWord, "}\n");



	writeSpace(acWord, nSpaceNum);
	_snprintf_s(acWord, sizeof(acWord)-1, "%sif(nIndex2 < 0 || nIndex2 >= %s)\n", acWord, pVariable->getArrayMaxLen(1));

	writeSpace(acWord, nSpaceNum);
	strcat(acWord, "{\n");

	nSpaceNum += 4;
	writeSpace(acWord, nSpaceNum);
	if (bBuiltIn)
	{
		if (pVariable->checkDefaultValue())
		{
			_snprintf_s(acWord, sizeof(acWord)-1, "%sreturn %s;\n", acWord, pVariable->getDefaultValue());
		}
		else
		{
			_snprintf_s(acWord, sizeof(acWord)-1, "%sreturn %s;\n", acWord, getDefaultValue(pVariable->getType()));
		}
	}
	else
	{
		_snprintf_s(acWord, sizeof(acWord)-1, "%sreturn NULL;\n", acWord, getDefaultValue(pVariable->getType()));
	}


	nSpaceNum -= 4;
	writeSpace(acWord, nSpaceNum);
	strcat(acWord, "}\n");

	writeSpace(acWord, nSpaceNum);
	if (bBuiltIn)
	{
		_snprintf_s(acWord, sizeof(acWord)-1, "%sreturn %s[nIndex][nIndex2];\n", acWord, pVariable->getName());
	}
	else
	{
		_snprintf_s(acWord, sizeof(acWord)-1, "%sreturn &%s[nIndex][nIndex2];\n", acWord, pVariable->getName());
	}

	nSpaceNum -= 4;
	writeSpace(acWord, nSpaceNum);
	strcat(acWord, "}\n");
	// **** end Type getName(int nIndex) ****


	// **** Type setName(int nIndex) ****
	writeSpace(acWord, nSpaceNum);
	if (bBuiltIn)
	{
		_snprintf_s(acWord, sizeof(acWord)-1, "%svoid set%s(int nIndex, int nIndex2, %s value)\n", acWord, pName, pVariable->getType());
	}
	else
	{
		_snprintf_s(acWord, sizeof(acWord)-1, "%svoid set%s(int nIndex, int nIndex2, %s& value)\n", acWord, pName, pVariable->getType());
	}

	writeSpace(acWord, nSpaceNum);
	strcat(acWord, "{\n");

	nSpaceNum += 4;
	writeSpace(acWord, nSpaceNum);
	_snprintf_s(acWord, sizeof(acWord)-1, "%sif(nIndex < 0 || nIndex >= %s)\n", acWord, pVariable->getArrayMaxLen(0));

	writeSpace(acWord, nSpaceNum);
	strcat(acWord, "{\n");

	nSpaceNum += 4;
	writeSpace(acWord, nSpaceNum);
	_snprintf_s(acWord, sizeof(acWord)-1, "%sreturn;\n", acWord);


	nSpaceNum -= 4;
	writeSpace(acWord, nSpaceNum);
	strcat(acWord, "}\n");


	writeSpace(acWord, nSpaceNum);
	_snprintf_s(acWord, sizeof(acWord)-1, "%sif(nIndex2 < 0 || nIndex2 >= %s)\n", acWord, pVariable->getArrayMaxLen(1));

	writeSpace(acWord, nSpaceNum);
	strcat(acWord, "{\n");

	nSpaceNum += 4;
	writeSpace(acWord, nSpaceNum);
	_snprintf_s(acWord, sizeof(acWord)-1, "%sreturn;\n", acWord);


	nSpaceNum -= 4;
	writeSpace(acWord, nSpaceNum);
	strcat(acWord, "}\n");

	writeSpace(acWord, nSpaceNum);
	_snprintf_s(acWord, sizeof(acWord)-1, "%s%s[nIndex][nIndex2] = value;\n", acWord, pVariable->getName());

	nSpaceNum -= 4;
	writeSpace(acWord, nSpaceNum);
	strcat(acWord, "}\n");

	// **** end Type setName(int nIndex) ****

	fwrite(acWord, strlen(acWord), 1, pFile);


	++rCount;
}

// 写字符数组
void CParseHeader::writeCharArray(CVariable* pVariable, FILE* pFile, int nSpaceNum, int &rCount)
{
	if (NULL == pVariable || NULL == pFile)
	{
		return;
	}
	const char* pName = getFirstUpcase(pVariable->getName());
	if (NULL == pName)
	{
		return;
	}
	char acBuffer[MAX_PATH] = { 0 };
	if (rCount != 0)
	{
		// 空行
		writeSpaceLine(acBuffer, 1);
	}

	// 写入前导空格
	writeSpace(acBuffer, nSpaceNum);
	_snprintf_s(acBuffer, sizeof(acBuffer) - 1, "%schar* get%s(){ return %s;}\n", acBuffer, pName, pVariable->getName());

	// 写入前导空格
	writeSpace(acBuffer, nSpaceNum);
	_snprintf_s(acBuffer, sizeof(acBuffer) - 1, "%svoid set%s(const char* value)\n", acBuffer, pName);

	writeSpace(acBuffer, nSpaceNum);
	strcat(acBuffer, "{\n");

	nSpaceNum += 4;
	writeSpace(acBuffer, nSpaceNum);
	_snprintf_s(acBuffer, sizeof(acBuffer) - 1, "%sif (NULL == value)\n", acBuffer);

	writeSpace(acBuffer, nSpaceNum);
	strcat(acBuffer, "{\n");

	nSpaceNum += 4;
	writeSpace(acBuffer, nSpaceNum);
	_snprintf_s(acBuffer, sizeof(acBuffer) - 1, "%sreturn;\n", acBuffer);
	
	nSpaceNum -= 4;
	writeSpace(acBuffer, nSpaceNum);
	strcat(acBuffer, "}\n");

	writeSpace(acBuffer, nSpaceNum);
	_snprintf_s(acBuffer, sizeof(acBuffer) - 1, "%sstrncpy(%s, value, sizeof(%s) - 1);\n", acBuffer, pVariable->getName(), pVariable->getName());
	
	nSpaceNum -= 4;
	writeSpace(acBuffer, nSpaceNum);
	strcat(acBuffer, "}\n");

	fwrite(acBuffer, strlen(acBuffer), 1, pFile);
	++rCount;
}

/// 写二维字符串数组 
void CParseHeader::writeCharArray2(CVariable* pVariable, FILE* pFile, int nSpaceNum, int &rCount)
{
	if (NULL == pVariable || NULL == pFile)
	{
		return;
	}
	const char* pName = getFirstUpcase(pVariable->getName());
	if (NULL == pName)
	{
		return;
	}

	char acWord[MAX_LINE_CHAR_NUM] = { 0 };

	if (rCount != 0)
	{
		// 空行
		writeSpaceLine(acWord, 1);
	}

	// **** Type getName(int nIndex) ****
	// 写入前导空格
	writeSpace(acWord, nSpaceNum);
	_snprintf_s(acWord, sizeof(acWord)-1, "%schar* get%s(int nIndex)\n", acWord, pName);

	writeSpace(acWord, nSpaceNum);
	strcat(acWord, "{\n");

	nSpaceNum += 4;
	writeSpace(acWord, nSpaceNum);
	_snprintf_s(acWord, sizeof(acWord)-1, "%sif(nIndex < 0 || nIndex >= %s)\n", acWord, pVariable->getArrayMaxLen(0));

	writeSpace(acWord, nSpaceNum);
	strcat(acWord, "{\n");

	nSpaceNum += 4;
	writeSpace(acWord, nSpaceNum);
	if (pVariable->checkDefaultValue())
	{
		_snprintf_s(acWord, sizeof(acWord)-1, "%sreturn %s;\n", acWord, pVariable->getDefaultValue());
	}
	else
	{
		_snprintf_s(acWord, sizeof(acWord)-1, "%sreturn %s;\n", acWord, getDefaultValue(pVariable->getType()));
	}

	nSpaceNum -= 4;
	writeSpace(acWord, nSpaceNum);
	strcat(acWord, "}\n");

	writeSpace(acWord, nSpaceNum);
	_snprintf_s(acWord, sizeof(acWord)-1, "%sreturn %s[nIndex];\n", acWord, pVariable->getName());

	nSpaceNum -= 4;
	writeSpace(acWord, nSpaceNum);
	strcat(acWord, "}\n");
	// **** end Type getName(int nIndex) ****


	// **** Type setName(int nIndex) ****
	writeSpace(acWord, nSpaceNum);
	_snprintf_s(acWord, sizeof(acWord)-1, "%svoid set%s(int nIndex, char* value)\n", acWord, pName);

	writeSpace(acWord, nSpaceNum);
	strcat(acWord, "{\n");

	nSpaceNum += 4;
	writeSpace(acWord, nSpaceNum);
	_snprintf_s(acWord, sizeof(acWord)-1, "%sif(nIndex < 0 || nIndex >= %s)\n", acWord, pVariable->getArrayMaxLen(0));

	writeSpace(acWord, nSpaceNum);
	strcat(acWord, "{\n");

	nSpaceNum += 4;
	writeSpace(acWord, nSpaceNum);
	_snprintf_s(acWord, sizeof(acWord)-1, "%sreturn;\n", acWord);


	nSpaceNum -= 4;
	writeSpace(acWord, nSpaceNum);
	strcat(acWord, "}\n");

	writeSpace(acWord, nSpaceNum);
	_snprintf_s(acWord, sizeof(acWord)-1, "%sstrncpy(%s[nIndex], value, %s - 1);\n", acWord, pVariable->getName(), pVariable->getArrayMaxLen(1));

	nSpaceNum -= 4;
	writeSpace(acWord, nSpaceNum);
	strcat(acWord, "}\n");

	// **** end Type setName(int nIndex) ****

	fwrite(acWord, strlen(acWord), 1, pFile);


	++rCount;
}

const char* CParseHeader::getFirstUpcase(const char* pLine)
{
	if (NULL == pLine)
	{
		return NULL;
	}
	int nLineLength = strlen(pLine);
	for (int i = 0; i < nLineLength; ++i)
	{
		if (pLine[i] >= 'A' && pLine[i] <= 'Z')
		{
			return (pLine + i);
		}
	}
	return NULL;
}

/// 写指定多个空格
void CParseHeader::writeSpace(char* pBuffer, int nSpaceNum)
{
	for (int i = 0; i < nSpaceNum; ++i)
	{
		strcat(pBuffer, " ");
	}
}

/// 写入指定空行
void CParseHeader::writeSpaceLine(char* pBuffer, int nSpaceLineNum)
{
	for (int i = 0; i < nSpaceLineNum; ++i)
	{
		strcat(pBuffer, "\n");
	}
}
