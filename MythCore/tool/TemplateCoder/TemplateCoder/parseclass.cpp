#include "parseclass.h"

/// ��ʼ
void CParseClass::init()
{
	CBuildInTypeToPB tValue;

	tValue.setBuildInType("int");
	tValue.setPBType("uint32");
	mBuildTypeToPBList.push_back(tValue);

	tValue.setBuildInType("char");
	tValue.setPBType("string");
	mBuildTypeToPBList.push_back(tValue);

	tValue.setBuildInType("float");
	tValue.setPBType("float");
	mBuildTypeToPBList.push_back(tValue);

	tValue.setBuildInType("unsigned long long");
	tValue.setPBType("uint64");
	mBuildTypeToPBList.push_back(tValue);
}

void CParseClass::clearContent()
{
	for (int i = 0; i < mFileContent.size(); ++i)
	{
		delete[]mFileContent[i];
	}

	mFileContent.clear();
}


void CParseClass::parseClass(const char* pFilePath, const char* pClassName)
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
	char* pPos = NULL;
	int nClassCount = 0;
	bool bInClass = false;
	bool bInEnum = false;
	// ��ȡ�ļ������ݣ���������
	while (fgets(acBuffer, MAX_PATH, pFile) != NULL)
	{
		pPos = strstr(acBuffer, pClassName);
		if (!bInClass && NULL == pPos)
		{
			continue;
		}
		else
		{
			bInClass = true;
		}

		if (bInEnum)
		{
			pPos = strstr(acBuffer, "};");
			// ����Ľ�����־
			if (NULL != pPos)
			{
				bInEnum = false;
			}
			continue;
		}

		pPos = strstr(acBuffer, "enum");
		if (NULL != pPos)
		{
			bInEnum  = true;
			continue;
		}

		pPos = strstr(acBuffer, "class");
		if (NULL != pPos)
		{
			++ nClassCount;
		}
		else
		{
			pPos = strstr(acBuffer, "};");
			// ����Ľ�����־
			if (NULL != pPos)
			{
				--nClassCount;
			}
		}


		char *pNewLine = new char[MAX_LINE_CHAR_NUM];
		strncpy(pNewLine, acBuffer, sizeof(acBuffer) - 1);
		pNewLine[strlen(acBuffer)] = '\0';
		mFileContent.push_back(pNewLine);

		if (0 == nClassCount)
		{
			break;
		}
	}
	
	for (mCurLineIndex = 0; mCurLineIndex < (int)mFileContent.size(); ++mCurLineIndex)
	{
		parseLine(mFileContent[mCurLineIndex], strlen(mFileContent[mCurLineIndex]));
	}

}

void CParseClass::parseLine(const char* pLine, int nLineLength)
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
		CClass* pNewClass = new CClass;
		if (NULL == mCurClass)
		{
			mMainClass = pNewClass;
		}
		else
		{
			CClass::SUB_CLASS_VECTOR& rSubClassList = mCurClass->getSubClassList();
			rSubClassList.push_back(pNewClass);
		}
		// ����ǰ��������Ϊ�ⲿ��
		pNewClass->setOutClass(mCurClass);
		//���µ�������Ϊ��ǰ��
		mCurClass = pNewClass;

		getFirstWord(pLine, nStart, nLineLength, acWord);
		mCurClass->setName(acWord);

		const char* pParentPos = strstr(pLine, "public");
		if (NULL != pParentPos)
		{
			nStart = pParentPos - pLine;
			nStart += strlen("public");
			getFirstWord(pLine, nStart, nLineLength, acWord);
			pNewClass->setParentName(acWord);
		}

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
	
	CClass::VARIABLE_VECTOR& rVariableList = mCurClass->getVariableList();
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
		rVariableList.push_back(pNewVariable);

		if (nStart >= nLineLength)
		{
			break;
		}
	}

}

void CParseClass::getFirstWord(const char* pLine, int& rStart, int nLineLength, char* pWord)
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

void CParseClass::getFirstVariable(const char* pLine, int& rStart, int nLineLength, char* pWord)
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

/// �õ���һ���������Ŀ�ʼ��
bool CParseClass::getVariableTypeName(const char* pLine, int& rStart, int nLineLength, char* pVariableName)
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

void CParseClass::processVariableType(char* pSrc, char* pDst, int nLength)
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

bool CParseClass::checkFunc(const char* pLine, int nLineLength)
{
	if (NULL == pLine)
	{
		return false;
	}

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

bool CParseClass::checkComment(const char* pLine, int nLineLength)
{

	int nStart = 0;
	bool bDelete = deleteHeadSpace(pLine, nStart, nLineLength);

	if (pLine[nStart] == '/' && (nStart + 1 < nLineLength) && pLine[nStart + 1] == '/')
	{
		return true;
	}
	// /*
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

// ����false��ʾ��
bool CParseClass::deleteHeadSpace(const char* pLine, int& rStart, int nLineLength)
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


int CParseClass::calcArrayDimension(CVariable* pVariable, const char* pLine, int nLineLength)
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
void CParseClass::getMaxArrayLen(const char* pSrc, int nLength, char* pMaxArrayLen)
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

/// �õ������ʵ���֣�ȥ��ǰ׺CTpl,C,CTemplate��
void CParseClass::getRealClassName(const char* pClassName, char* pRealName, int nSize)
{
	if (NULL == pClassName || NULL == pRealName)
	{
		return;
	}

	/// if(str)
	if (0 == strncmp(pClassName, "CTpl", strlen("CTpl")))
	{
		strncpy(pRealName, pClassName + strlen("CTpl"), nSize - 1);
	}
	else if (0 == strncmp(pClassName, "CTemplate", strlen("CTemplate")))
	{
		strncpy(pRealName, pClassName + strlen("CTemplate"), nSize - 1);
	}
	else if (0 == strncmp(pClassName, "C", strlen("C")))
	{
		strncpy(pRealName, pClassName + strlen("C"), nSize - 1);
	}
	else
	{
		strncpy(pRealName, pClassName, nSize - 1);
	}
}

/// �õ���һ����д��ĸ��λ��
const char* CParseClass::getFirstUpcase(const char* pSrc)
{
	if (NULL == pSrc)
	{
		return NULL;
	}
	int nLength = strlen(pSrc);
	for (int i = 0; i < nLength; ++i)
	{
		if (pSrc[i] >= 'A' && pSrc[i] <= 'Z')
		{
			return (pSrc + i);
		}
	}
	return NULL;
}


const char* CParseClass::getPBTypeValue(const char* pBuildInType)
{
	if (NULL == pBuildInType)
	{
		return NULL;
	}

	for (int i = 0; i < (int)mBuildTypeToPBList.size(); ++i)
	{
		if (0 == strncmp(pBuildInType, mBuildTypeToPBList[i].getBuildInType(), MAX_NAME_LENGTH))
		{
			return mBuildTypeToPBList[i].getPBType();
		}
	}

	return NULL;
}

/// �õ�ȫ����
void CParseClass::getClassDomainName(CClass* pClass,char* pName, int nSize)
{
	if (NULL == pClass || NULL == pName)
	{
		return;
	}

	char acBuffer[CLASS_NAME_LENGTH] = { 0 };
	vector<CClass*> tClassList;
	for (; NULL != pClass; pClass = pClass->getOutClass())
	{
		tClassList.push_back(pClass);
	}

	for (int i = tClassList.size() - 1; i >= 0; -- i)
	{
		_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s%s::", acBuffer, tClassList[i]->getName());
	}
	strncpy(pName, acBuffer, nSize - 1);
}

/// дģ��ͷ�ļ�
void CParseClass::writeTempHeadFile()
{
	if (NULL == mMainClass)
	{
		return;
	}
	FILE* pFile = fopen("Template_back.h", "wt");
	if (NULL == pFile)
	{
		return;
	}
	char acClassName[MAX_NAME_LENGTH] = { 0 };
	getRealClassName(mMainClass->getName(), acClassName, sizeof(acClassName));

	char acBuffer[MAX_WRITE_BUFFER] = { 0 };
	_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t%s(){};\n", acBuffer, mMainClass->getName());
	_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t~%s(){};\n", acBuffer, mMainClass->getName());
	fwrite(acBuffer, strlen(acBuffer), 1, pFile);

	writeClassHeadFile(mMainClass, pFile);
	fclose(pFile);
}

/// д���ͷ�ļ�
void CParseClass::writeClassHeadFile(CClass* pClass, FILE* pFile)
{
	if (NULL == pClass || NULL == pFile)
	{
		return;
	}

	char acClassName[MAX_NAME_LENGTH] = { 0 };
	getRealClassName(pClass->getName(), acClassName, sizeof(acClassName));

	char acBuffer[MAX_WRITE_BUFFER] = { 0 };
	_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\tvoid SetFromPB(PB%s* pbData);\n", acBuffer, acClassName);
	_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\tvoid CreateToPB(PB%s* pbData);\n", acBuffer, acClassName);
	_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\n", acBuffer);
	fwrite(acBuffer, strlen(acBuffer), 1, pFile);

	for (int i = 0; i < pClass->getSubClassList().size(); ++i)
	{
		writeClassHeadFile(pClass->getSubClassList()[i], pFile);
	}
}

/// дģ��Դ�ļ�
void CParseClass::writeTempSourceFile()
{
	if (NULL == mMainClass)
	{
		return;
	}
	FILE* pFile = fopen("Template_back.cpp", "wt");
	if (NULL == pFile)
	{
		return;
	}
	writeClassSourceFile(mMainClass, pFile);
}

/// д���Դ�ļ�
void CParseClass::writeClassSourceFile(CClass* pClass, FILE* pFile)
{
	if (NULL == pClass || NULL == pFile)
	{
		return;
	}


	writeSetFromPB(pClass, pFile);
	writeCreatePB(pClass, pFile);

	for (int i = 0; i < pClass->getSubClassList().size(); ++i)
	{
		writeClassSourceFile(pClass->getSubClassList()[i], pFile);
	}

}

void CParseClass::writeSetFromPB(CClass* pClass, FILE* pFile)
{
	char acClassName[MAX_NAME_LENGTH] = { 0 };
	getRealClassName(pClass->getName(), acClassName, sizeof(acClassName));


	char acClassDomainName[CLASS_NAME_LENGTH] = { 0 };
	getClassDomainName(pClass, acClassDomainName, sizeof(acClassDomainName)-1);

	char acBuffer[MAX_WRITE_BUFFER] = { 0 };
	_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%svoid %sSetFromPB(PB%s* pbData)\n", acBuffer, acClassDomainName, acClassName);
	_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s{\n", acBuffer);

	// ����и���
	if (pClass->getParentName()[0] != '\0')
	{
		if (0 == strncmp(pClass->getParentName(), "CTemplate", CLASS_NAME_LENGTH))
		{
			// ��������
			_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\tmTempID = pbData->tempid();\t\n", acBuffer);

		}
		else
		{
			char acClassName[MAX_NAME_LENGTH] = { 0 };
			getRealClassName(pClass->getParentName(), acClassName, sizeof(acClassName));

			// �Զ�������
			_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t%s::SetFromPB(pbData->mutable_super());\n", acBuffer, pClass->getParentName());
		}
	}


	for (int i = 0; i < pClass->getVariableList().size(); ++i)
	{
		CVariable* pVariable = (pClass->getVariableList())[i];
		if (NULL == pVariable)
		{
			continue;
		}
		const char* pRealVariableName = getFirstUpcase(pVariable->getName());
		char acRealVariableName[MAX_NAME_LENGTH] = { 0 };
		strncpy(acRealVariableName, pRealVariableName, sizeof(acRealVariableName)-1);

		if (pVariable->getArrayDimension() > 0)
		{
			if (0 == strncmp("char", pVariable->getType(), MAX_NAME_LENGTH))
			{
				if (1 == pVariable->getArrayDimension())
				{
					_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\tstrncpy(%s, pbData->%s().c_str(), sizeof(%s) - 1);\n", acBuffer, pVariable->getName(), _strlwr(acRealVariableName), pVariable->getName());
				}
				else if (2 == pVariable->getArrayDimension())
				{
					_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\tfor(int i = 0; i < %s && i < pbData->%s_size(); ++ i)\n", acBuffer, pVariable->getArrayMaxLen(0), _strlwr(acRealVariableName));
					_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t{\n", acBuffer);
					_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t\tstrncpy(%s[i], pbData->%s(i).c_str(), sizeof(%s) - 1);\n", acBuffer, pVariable->getName(), _strlwr(acRealVariableName), pVariable->getName());
					_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t}\n", acBuffer);
				}
			}
			else
			{
				if (1 == pVariable->getArrayDimension())
				{
					_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\tfor(int i = 0; i < %s && i < pbData->%s_size(); ++ i)\n", acBuffer, pVariable->getArrayMaxLen(0), _strlwr(acRealVariableName));
					_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t{\n", acBuffer);
					const char* pPBType = getPBTypeValue(pVariable->getType());
					if (NULL == pPBType)
					{
						// �Զ�������
						_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t\t%s[i].SetFromPB(pbData->mutable_%s(i));\n", acBuffer, pVariable->getName(), _strlwr(acRealVariableName));
					}
					else
					{
						// ��������
						_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t\t%s[i] = pbData->%s(i);\n", acBuffer, pVariable->getName(), _strlwr(acRealVariableName));
					}
					_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t}\n", acBuffer);

				}
				else if (2 == pVariable->getArrayDimension())
				{
					_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\tint n%sCount = 0;\n", acBuffer, acRealVariableName);
					_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\tfor(int i = 0; i < %s; ++ i)\n", acBuffer, pVariable->getArrayMaxLen(0));
					_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t{\n", acBuffer);

					_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t\tfor(int j = 0; j < %s && n%sCount < %s_size(); ++ j, ++  n%sCount)\n",
						acBuffer, pVariable->getArrayMaxLen(1), acRealVariableName, _strlwr(acRealVariableName), acRealVariableName);
					_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t\t{\n", acBuffer);

					const char* pPBType = getPBTypeValue(pVariable->getType());
					if (NULL == pPBType)
					{
						// �Զ�������
						_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t\t\t%s[i][j].SetFromPB(pbData->mutable_%s(%s));\n", acBuffer, pVariable->getName(), _strlwr(acRealVariableName), acRealVariableName);
					}
					else
					{
						// ��������
						_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t\t\t%s[i][j] = pbData->%s(%s);\n", acBuffer, pVariable->getName(), _strlwr(acRealVariableName), acRealVariableName);
					}
					_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t\t}\n", acBuffer);
					_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t}\n", acBuffer);
				}

			}

			continue;
		}

		const char* pPBType = getPBTypeValue(pVariable->getType());
		if (NULL == pPBType)
		{
			// �Զ�������
			_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t%s.SetFromPB(pbData->mutable_%s());\n", acBuffer, pVariable->getName(), _strlwr(acRealVariableName));
		}
		else
		{
			// ��������
			_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t%s = pbData->%s();\n", acBuffer, pVariable->getName(), _strlwr(acRealVariableName));
		}
	}

	_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s}\n", acBuffer);
	fwrite(acBuffer, strlen(acBuffer), 1, pFile);
}

void CParseClass::writeCreatePB(CClass* pClass, FILE* pFile)
{
	char acClassName[MAX_NAME_LENGTH] = { 0 };
	getRealClassName(pClass->getName(), acClassName, sizeof(acClassName));


	char acClassDomainName[CLASS_NAME_LENGTH] = { 0 };
	getClassDomainName(pClass, acClassDomainName, sizeof(acClassDomainName)-1);

	char acBuffer[MAX_WRITE_BUFFER] = { 0 };

	_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%svoid %sCreateToPB(PB%s* pbData)\n", acBuffer, acClassDomainName, acClassName);
	_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s{\n", acBuffer);




	// ����и���
	if (pClass->getParentName()[0] != '\0')
	{
		if (0 == strncmp(pClass->getParentName(), "CTemplate", CLASS_NAME_LENGTH))
		{
			// ��������
			_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\tpbData->set_tempid(mTempID);\t\n", acBuffer);

		}
		else
		{
			char acClassName[MAX_NAME_LENGTH] = { 0 };
			getRealClassName(pClass->getParentName(), acClassName, sizeof(acClassName));

			// �Զ�������
			_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t%s::CreateToPB(pbData->mutable_super());\n", acBuffer, pClass->getParentName());
		}
	}


	for (int i = 0; i < pClass->getVariableList().size(); ++i)
	{
		CVariable* pVariable = (pClass->getVariableList())[i];
		if (NULL == pVariable)
		{
			continue;
		}
		const char* pRealVariableName = getFirstUpcase(pVariable->getName());
		char acRealVariableName[MAX_NAME_LENGTH] = { 0 };
		strncpy(acRealVariableName, pRealVariableName, sizeof(acRealVariableName)-1);

		if (pVariable->getArrayDimension() > 0)
		{
			if (0 == strncmp("char", pVariable->getType(), MAX_NAME_LENGTH))
			{
				if (1 == pVariable->getArrayDimension())
				{
					_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\tpbData->set_%s(%s);\n", acBuffer, _strlwr(acRealVariableName), pVariable->getName());
				}
				else if (2 == pVariable->getArrayDimension())
				{
					_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\tfor(int i = 0; i < %s; ++ i)\n", acBuffer, pVariable->getArrayMaxLen(0));
					_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t{\n", acBuffer);
					_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t\tpbData->add_%s(%s[i]);\n", acBuffer, _strlwr(acRealVariableName), pVariable->getName());
					_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t}\n", acBuffer);
				}
			}
			else
			{
				if (1 == pVariable->getArrayDimension())
				{
					_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\tfor(int i = 0; i < %s && i < pbData->%s_size(); ++ i)\n", acBuffer, pVariable->getArrayMaxLen(0), _strlwr(acRealVariableName));
					_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t{\n", acBuffer);
					const char* pPBType = getPBTypeValue(pVariable->getType());
					if (NULL == pPBType)
					{
						// �Զ�������
						_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t\t%s[i].CreateToPB(pbData->add_%s());\n", acBuffer, pVariable->getName(), _strlwr(acRealVariableName));
					}
					else
					{
						// ��������
						_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t\tpbData->add_%s(%s[i]);\n", acBuffer, _strlwr(acRealVariableName), pVariable->getName());
					}
					_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t}\n", acBuffer);

				}
				else if (2 == pVariable->getArrayDimension())
				{
					_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\tfor(int i = 0; i < %s; ++ i)\n", acBuffer, pVariable->getArrayMaxLen(0));
					_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t{\n", acBuffer);

					_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t\tfor(int j = 0; j < %s; ++ j)\n",acBuffer, pVariable->getArrayMaxLen(1));
					_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t\t{\n", acBuffer);

					const char* pPBType = getPBTypeValue(pVariable->getType());
					if (NULL == pPBType)
					{
						// �Զ�������
						_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t\t\t%s[i][j].CreateToPB(pbData->add_%s());\n", acBuffer, pVariable->getName(), _strlwr(acRealVariableName));
					}
					else
					{
						// ��������
						_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t\t\tpbData->add_%s(%s);\n", acBuffer, _strlwr(acRealVariableName), pVariable->getName());
					}
					_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t\t}\n", acBuffer);
					_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t}\n", acBuffer);
				}

			}

			continue;
		}

		const char* pPBType = getPBTypeValue(pVariable->getType());
		if (NULL == pPBType)
		{
			// �Զ�������
			_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t%s.CreateToPB(pbData->mutable_%s());\n", acBuffer, pVariable->getName(), _strlwr(acRealVariableName));
		}
		else
		{
			// ��������
			_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\tpbData->set_%s(%s);\n", acBuffer, _strlwr(acRealVariableName), pVariable->getName());
		}
	}


	_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s}\n", acBuffer);
	_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\n\n", acBuffer);

	fwrite(acBuffer, strlen(acBuffer), 1, pFile);
}


/// дģ��PB�ļ�
void CParseClass::writeTempPBFile()
{
	if (NULL == mMainClass)
	{
		return;
	}
	FILE* pFile = fopen("template_pb_back.hxx", "wt");
	if (NULL == pFile)
	{
		return;
	}

	writeClassPBFile(mMainClass, pFile);
	fclose(pFile);
}

/// д��PB�ļ�
void CParseClass::writeClassPBFile(CClass* pClass, FILE* pFile)
{
	if (NULL == pClass || NULL == pFile)
	{
		return;
	}

	
	for (int i = 0; i < pClass->getSubClassList().size(); ++ i)
	{
		writeClassPBFile(pClass->getSubClassList()[i], pFile);
	}

	char acClassName[MAX_NAME_LENGTH] = { 0 };
	getRealClassName(pClass->getName(), acClassName, sizeof(acClassName));

	char acBuffer[MAX_WRITE_BUFFER] = { 0 };
	_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%smessage PB%s\n", acBuffer, acClassName);
	_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s{\n", acBuffer);
	
	int nFieldCount = 1;
	// ����и���
	if (pClass->getParentName()[0] != '\0')
	{
		if (0 == strncmp(pClass->getParentName(), "CTemplate", CLASS_NAME_LENGTH))
		{
			// ��������
			_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t\t\t\t%s\t%s\t=%d;\n", acBuffer, "uint32", "TempID", nFieldCount);

		}
		else
		{
			char acClassName[MAX_NAME_LENGTH] = { 0 };
			getRealClassName(pClass->getParentName(), acClassName, sizeof(acClassName));

			// �Զ�������
			_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t\t\t\tPB%s\t%s\t=%d;\n", acBuffer, acClassName, "Super", nFieldCount);
		}
		++ nFieldCount;
	}

	for (int i = 0; i < pClass->getVariableList().size(); ++ i)
	{
		CVariable* pVariable = (pClass->getVariableList())[i];
		if (NULL == pVariable)
		{
			continue;
		}
		if (pVariable->getArrayDimension() > 0)
		{
			// һάchar����ֱ�Ӳ���repeated,ֻ�ж�άchar�������repeated
			if (pVariable->getArrayDimension() < 2 && 0 == strncmp("char", pVariable->getType(), MAX_NAME_LENGTH))
			{
				_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t\t\t", acBuffer);
			}
			else
			{
				_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\trepeated", acBuffer);
			}
		}
		else
		{
			_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t\t\t", acBuffer);
		}

		const char* pRealVariableName = getFirstUpcase(pVariable->getName());
		const char* pPBType = getPBTypeValue(pVariable->getType());
		if (NULL == pPBType)
		{
			char acClassName[MAX_NAME_LENGTH] = { 0 };
			getRealClassName(pVariable->getType(), acClassName, sizeof(acClassName));

			// �Զ�������
			_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\tPB%s\t%s\t=%d;\n", acBuffer, acClassName, pRealVariableName, nFieldCount);
		}
		else
		{
			// ��������
			_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t%s\t%s\t=%d;\n", acBuffer, pPBType, pRealVariableName, nFieldCount);
		}
		++ nFieldCount;
	}

	_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s}\n", acBuffer);
	fwrite(acBuffer, strlen(acBuffer), 1, pFile);
}