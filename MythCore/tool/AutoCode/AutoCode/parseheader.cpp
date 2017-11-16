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


/// 加载默认值配置文件
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

	// 读取文件的内容，保存起来
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

	// 这行是函数的
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
	// 静态变量和函数都跳过
	if (strncmp(acWord, "static", MAX_PATH - 1) == 0)
	{
		return;
	}

	// const int i;如果不是以下修饰变量的，那nStart已经指向int后面的位置了，后退到int前面
	if (strncmp(acWord, "auto", MAX_PATH - 1) != 0 &&
		strncmp(acWord, "const", MAX_PATH - 1) != 0 &&
		strncmp(acWord, "mutable", MAX_PATH - 1) != 0 &&
		strncmp(acWord, "register", MAX_PATH - 1) != 0 &&
		strncmp(acWord, "volatile", MAX_PATH - 1) != 0)
	{
		// 回到类型前
		nStart = nStart - strlen(acWord);
	}

	// 得到变量类型
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

	// 读取文件的内容，保存起来
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
		// //注释
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

// 得到第一个变量
void CParseHeader::getFirstVariable(const char* pLine, int& rStart, int nLineLength, char* pWord)
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
	for (int i = rStart; i < nLineLength; ++i)
	{
		if (pLine[i] == ' '		// 空字符
			|| pLine[i] == '\t'	// 间隔字符
			|| pLine[i] == '\n'	// 行结束
			|| pLine[i] == ','	// 变量结束
			|| pLine[i] == ';'	// 语句结束
			|| pLine[i] == '[') // 数组开始
		{
			nIndex = i;
			break;
		}
		// //注释
		if (pLine[i] == '/' && i + 1 < nLineLength && pLine[i + 1] == '/')
		{
			nIndex = i;
			bEnd = true;
			break;
		}
		// /*注释
		if (pLine[i] == '/' && i + 1 < nLineLength && pLine[i + 1] == '*')
		{
			nIndex = i;
			bEnd = true;
			break;
		}

	}

	strncpy(pWord, pLine + rStart, nIndex - rStart);
	pWord[nIndex - rStart] = '\0';

	// 如果结束符是数组
	if (pLine[nIndex] == '[')
	{
		// 找到数组的解释]
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

/// 得到第一个变量名的开始处
bool CParseHeader::getVariableTypeName(const char* pLine, int& rStart, int nLineLength, char* pVariableName)
{
	if (NULL == pLine)
	{
		return false;
	}
	
	int nPos = -1;
	for (int i = rStart; i < nLineLength; ++ i)
	{
		// ,是因为多个变量名之间
		if (',' == pLine[i] || ';' == pLine[i] || '[' == pLine[i])
		{
			nPos = i;
			break;
		}
	}
	// 找不到逗号和分号，一般不会如此
	if (-1 == nPos)
	{
		rStart = nLineLength;
		return false;
	}


	// 倒着找，找到第一个变量名，主要可能有空格
	bool bInName = false;
	for (int i = nPos - 1; i >= 0; -- i)
	{
		// 第一个不是空格和tab键
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
		// 结束
		if (pSrc[i] == '\0')
		{
			break;
		}

		// 如果是多个空格连在一起，只留一个
		if (pSrc[i] == ' ')
		{
			if (nCount > 0 && pDst[nCount - 1] != ' ')
			{
				pDst[nCount] = pSrc[i];
				++ nCount;
			}
			continue;
		}

		// 多个空格和table缩成一个空格
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
	// 如果最后一个是空格,去掉
	if (pDst[nCount - 1] == ' ')
	{
		pDst[nCount - 1] = '\0';
	}
}

// 是否是函数部分
bool CParseHeader::checkFunc(const char* pLine, int nLineLength)
{
	if (NULL == pLine)
	{
		return false;
	}

	// 函数以（）为标志
	if (strchr(pLine, '(') != NULL)
	{
		// 如果这行里就有；表示函数声明
		if (strchr(pLine, ';') != NULL)
		{
			return true;
		}

		char acBuffer[MAX_PATH] = { 0 };
		int nCurLine = mCurLineIndex;
		int nInFunction = 0;
		for (; nCurLine < (int)mFileContent.size(); ++nCurLine)
		{
			// 处理像如下这种函数
			// void Func(int i, float f,
			// int j);
			if (0 == nInFunction && strchr(mFileContent[nCurLine], ';') != NULL)
			{
				mCurLineIndex = nCurLine;
				return true;
			}

			// 处理像如下这种函数
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

/// 是否是注释部分
bool CParseHeader::checkComment(const char* pLine, int nLineLength)
{
	int nStart = 0;
	
	// 函数前导空格
	bool bDelete = deleteHeadSpace(pLine, nStart, nLineLength);
	
	// 这一行是否是以//开头
	if (pLine[nStart] == '/' && (nStart + 1 < nLineLength) && pLine[nStart + 1] == '/')
	{
		return true;
	}

	// 这一行是否是以/*开头,并找到对应的*/所在的行
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

// 返回true表示这一行已经结束,false表示该行还没结束
bool CParseHeader::deleteHeadSpace(const char* pLine, int& rStart, int nLineLength)
{
	for (int i = rStart; i < nLineLength; ++i)
	{
		// 碰到下列终止符就终止掉
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

// 计算数组维数
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

/// 得到数组维数长度
void CParseHeader::getMaxArrayLen(const char* pSrc, int nLength, char* pMaxArrayLen)
{
	int i = 0;
	int nPos = 0;
	// 去掉前导空格
	for (; i < nLength; ++i)
	{
		if (pSrc[i] == ' ' || pSrc[i] == '\t')
		{
			continue;
		}

		nPos = i;
		break;
	}

	// 找到结束字符
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