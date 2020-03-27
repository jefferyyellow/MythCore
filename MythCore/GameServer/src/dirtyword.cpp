#include "dirtyword.h"
#include "commontype.h"
#include <stdio.h>
void CDirtyWord::clear()
{
	mDirtyWordSet.clear();
}

bool CDirtyWord::loadDirtyWordConfig(const char* pFilePath)
{
	if (NULL == pFilePath)
	{
		return false;
	}

	FILE* pFile = fopen(pFilePath, "r");
	if (NULL == pFile)
	{
		return false;
	}

	char tLineBuf[STR_LENGTH_1024] = { 0 };
	fread(tLineBuf, 1, 3, pFile);
	// UTF8的BOM文件头，如果检测不是UTF8的BOM文件头，从头开始读
	if (tLineBuf[0] != (char)0xEF || tLineBuf[1] != (char)0xBB || tLineBuf[2] != (char)0xBF)
	{
		fseek(pFile, 0, SEEK_SET);
	}

	for (;;)
	{
		fgets((char *)tLineBuf, sizeof(tLineBuf) - 1, pFile);
		if (tLineBuf[strlen(tLineBuf) - 1] == '\n')
		{
			tLineBuf[strlen(tLineBuf) - 1] = '\0';
		}
		mDirtyWordSet.insert(tLineBuf);
		if (feof(pFile))
		{
			break;
		}
	}
	fclose(pFile);
	return true;
}

bool CDirtyWord::checkDirtyWord(const char* pWord)
{
	if (NULL == pWord)
	{
		return false;
	}
	std::set<std::string>::iterator it = mDirtyWordSet.find(pWord);
	return it != mDirtyWordSet.end();
}

void CDirtyWord::addDirtyWord(const char* pWord)
{
	if (NULL == pWord)
	{
		return;
	}

	mDirtyWordSet.insert(pWord);
}
