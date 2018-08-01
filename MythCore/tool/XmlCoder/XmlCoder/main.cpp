#include "parsexml.h"
/// get the file name from file path
char* getFileName(const char* pFilePath, char* pFileName, int nSize)
{
	const char* pFind = strrchr(pFilePath, '.');
	if (NULL == pFind)
	{
		strncpy(pFileName, pFilePath, nSize - 1);
	}
	else
	{
		strncpy(pFileName, pFilePath, pFind - pFilePath);
	}

	return pFileName;
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("param 1 is target xml file name");
		return -1;
	}

	char acXmlName[MAX_PATH] = {0};
	strncpy(acXmlName, argv[1], sizeof(acXmlName) - 1);

	char acFileName[MAX_PATH] = {0};
	getFileName(acXmlName, acFileName, sizeof(acFileName));

	CParseXml tParseXml;
	tParseXml.parseXml(acXmlName);

	char acHeadFileName[MAX_PATH] = {0};
	_snprintf_s(acHeadFileName, sizeof(acHeadFileName)-1, "%s.h", acFileName); 
	tParseXml.writeHeadFile(acHeadFileName);

	char acSourceFileName[MAX_PATH] = { 0 };
	_snprintf_s(acSourceFileName, sizeof(acSourceFileName)-1, "%s.cpp", acFileName);
	tParseXml.writeSourceFile(acSourceFileName, acXmlName);
	return 0;
}

