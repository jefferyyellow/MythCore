#include <windows.h>
#include "parsexml.h"
#include "tinyxml2.h"
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

void loadQuestionXml()
{
	tinyxml2::XMLDocument tDocument;
	if (XML_SUCCESS != tDocument.LoadFile("QuestionClientConfig.xml"))
	{
		return;
	}

	XMLElement* pRoot = tDocument.RootElement();
	if (NULL == pRoot)
	{
		return;
	}


	tinyxml2::XMLDocument tNewDocument;
	XMLElement* pNewRootElem = tNewDocument.NewElement("AllQuestion");
	tNewDocument.LinkEndChild(pNewRootElem);

	XMLElement* pQuestionElem = pRoot->FirstChildElement("Question");
	for (; NULL != pQuestionElem; pQuestionElem = pQuestionElem->NextSiblingElement("Question"))
	{
		const char* pQuestID = pQuestionElem->Attribute("ID");
		while (*pQuestID != '\0')
		{
			if (*pQuestID != ' ')
			{
				break;
			}
			++ pQuestID;
		}
		int nQuestID = atoi(pQuestID);

		XMLElement* pTextElem = pQuestionElem->FirstChildElement("QuestionInfo");
		if (NULL != pTextElem)
		{
			const char*pText = pTextElem->Attribute("Text");
			if (NULL != pText)
			{
				int nCharNum = MultiByteToWideChar(CP_UTF8, 0, pText, -1, NULL, 0) - 1;
				if (nCharNum > 28)
				{
					printf("%d\n", nQuestID);
					XMLElement* pNewQuesElem = tNewDocument.NewElement("Question");
					pNewQuesElem->SetAttribute("ID", nQuestID);
					pNewRootElem->LinkEndChild(pNewQuesElem);
				}
				
			}
		}
	}

	tNewDocument.SaveFile("ClientQuestLeng.xml");
}

int main(int argc, char** argv)
{
	loadQuestionXml();
	return 0;
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

