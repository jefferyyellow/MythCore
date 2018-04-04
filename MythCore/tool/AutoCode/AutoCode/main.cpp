#include "parseheader.h"
#include "tinyxml2.h"
#include <time.h>
using namespace tinyxml2;
#define MAX_FILE_BUFF 100 * 1024		// 1M
class CFilePair
{
public:
	char	mHeadFileName[MAX_PATH];
	char	mSrcFileName[MAX_PATH];
};


/// get the pos of the last '/' or '\'
const char* getLastSeparator(const char* pFilePath)
{
	const char* pFind = strrchr(pFilePath, '/');
	if (NULL == pFind)
	{
		pFind = strrchr(pFilePath, '\\');
	}
	return pFind;

}

/// get the file name from file path
char* getFileName(const char* pFilePath, char* pFileName, int nSize)
{
	const char* pFind = getLastSeparator(pFilePath);
	if (NULL == pFind)
	{
		strncpy(pFileName, pFilePath, nSize - 1);
	}
	else
	{
		strncpy(pFileName, pFind + 1, nSize - 1);
	}
	return pFileName;
}

typedef vector<CFilePair>	FILE_LIST;
FILE_LIST gFileList;
bool loadFileList(const char* pXmlFile)
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


	XMLElement* pFileElement = pRootElement->FirstChildElement("File");
	for (; NULL != pFileElement; pFileElement = pFileElement->NextSiblingElement("File"))
	{
		CFilePair tFilePair;
		const char* pHeadFile = pFileElement->Attribute("HeadFile");
		if (NULL != pHeadFile)
		{
			strncpy(tFilePair.mHeadFileName, pHeadFile, MAX_PATH - 1);
		}

		const char* pSrcFile = pFileElement->Attribute("SrcFile");
		if (NULL != pSrcFile)
		{
			strncpy(tFilePair.mSrcFileName, pSrcFile, MAX_PATH - 1);
		}

		gFileList.push_back(tFilePair);
	}
	return true;
}

bool copyFile(const char* pSrcFilePath, const char* pDestFilePath)
{
	FILE* pSrcFile = fopen(pSrcFilePath, "r");
	if (NULL == pSrcFile)
	{
		return false;
	}

	char acDestFilePath[MAX_PATH] = {0};
	_snprintf_s(acDestFilePath, sizeof(acDestFilePath)-1, "back/%s", pDestFilePath);

	FILE* pDestFile = fopen(acDestFilePath, "wt");
	if (NULL == pDestFile)
	{
		return false;
	}

	char acBuffer[MAX_FILE_BUFF];
	
	int nReadBytes = 0;
	while ((nReadBytes = fread(acBuffer, 1, sizeof(acBuffer)-1, pSrcFile)) > 0)
	{
		fwrite(acBuffer, nReadBytes, 1, pDestFile);
	}
	fclose(pSrcFile);
	fclose(pDestFile);
	return true;
}

int main()
{
	CParseHeader tParseHeader;

	bool bResult = true;
	bResult = loadFileList("FileList.xml");
	if (!bResult)
	{
		printf("load file list xml failure");
		return false;
	}
	bResult = tParseHeader.loadDefaultValueXml("DefaultValue.xml");
	if (!bResult)
	{
		printf("load default value xml failure");
		return false;
	}

	time_t tNow = time(NULL);
	char acFileName[MAX_PATH] = {0};
	for (int i = 0; i < (int)gFileList.size(); ++ i)
	{
		tParseHeader.clear();
		tParseHeader.parseHeaderFile(gFileList[i].mHeadFileName);
		getFileName(gFileList[i].mHeadFileName, acFileName, sizeof(acFileName));
		_snprintf_s(acFileName, sizeof(acFileName) - 1, "%s_%d", acFileName, tNow);
		bResult = copyFile(gFileList[i].mHeadFileName, acFileName);
		if (!bResult)
		{
			printf("%s back file failure", gFileList[i].mHeadFileName);
			continue;
		}
		tParseHeader.writeHeaderFile(gFileList[i].mHeadFileName);


		tParseHeader.parseSourceFile(gFileList[i].mSrcFileName);
		getFileName(gFileList[i].mSrcFileName, acFileName, sizeof(acFileName));
		_snprintf_s(acFileName, sizeof(acFileName) - 1, "%s_%d", acFileName, tNow);
		bResult = copyFile(gFileList[i].mSrcFileName, acFileName);
		if (!bResult)
		{
			printf("%s back file failure", gFileList[i].mSrcFileName);
			continue;
		}
		tParseHeader.writeSourceFile(gFileList[i].mSrcFileName);
	}

	return true;
}