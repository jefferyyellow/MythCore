#include "stdafx.h"
#include "TaskTemplate.h"
#include "commondefine.h"

void CTaskTemplate::ClearOptionList(OPTION_LIST& rList)
{
	for (int i = 0; i < rList.size(); ++i)
	{
		ClearMainNodeList(rList[i]->mNodeList);
		delete rList[i];
	}
}

void CTaskTemplate::ClearMainNodeList(TASK_NODE_LIST& rList)
{
	for (int  i = 0; i < rList.size(); ++ i)
	{
		ClearOptionList(rList[i]->mOptionList);
		delete rList[i];
	}
}

CTaskTemplate::~CTaskTemplate()
{
	ClearMainNodeList(mTextNodeList);
	ClearMainNodeList(mCondNodeList);
	ClearMainNodeList(mDiagNodeList);
}
void CTaskTemplate::loadTaskTemplate(const char* pTemplateFile)
{
	if (NULL == pTemplateFile)
	{
		return;
	}
	tinyxml2::XMLDocument tDocument;
	if (XML_SUCCESS != tDocument.LoadFile(pTemplateFile))
	{
		// 出错，无法加载xml文件
		return;
	}

	XMLElement* pRoot = tDocument.RootElement();
	if (NULL == pRoot)
	{
		// 出错，连Root节点都没有
		return;
	}

	XMLElement* pTextNodeElem = pRoot->FirstChildElement("TextNode");
	for (; NULL != pTextNodeElem; pTextNodeElem = pTextNodeElem->NextSiblingElement("TextNode"))
	{
		CTaskMainNode* pMainNode = new CTaskMainNode;
		 loadTextNode(pMainNode, pTextNodeElem);
		 mTextNodeList.push_back(pMainNode);
	}

	XMLElement* pCondNodeElem = pRoot->FirstChildElement("CondNode");
	for (; NULL != pCondNodeElem; pCondNodeElem = pCondNodeElem->NextSiblingElement("CondNode"))
	{
		CTaskMainNode* pMainNode = new CTaskMainNode;
		loadTextNode(pMainNode, pCondNodeElem);
		mCondNodeList.push_back(pMainNode);
	}

	XMLElement* pDiagNodeElem = pRoot->FirstChildElement("DiagNode");
	for (; NULL != pDiagNodeElem; pDiagNodeElem = pDiagNodeElem->NextSiblingElement("DiagNode"))
	{
		CTaskMainNode* pMainNode = new CTaskMainNode;
		loadTextNode(pMainNode, pDiagNodeElem);
		mDiagNodeList.push_back(pMainNode);
	}
}


void CTaskTemplate::loadTextNode(CTaskMainNode* pMainNode, XMLElement* pTextNodeElem)
{
	wchar_t acBuffer[4096] = { 0 };
	Utf8ToUnicode(pTextNodeElem->Attribute("Name"), acBuffer, sizeof(acBuffer) / 2 - 1);
	pMainNode->mName = acBuffer;

	Utf8ToUnicode(pTextNodeElem->Attribute("DesName"), acBuffer, sizeof(acBuffer) / 2 - 1);
	pMainNode->mDesName = acBuffer;

	XMLElement* pOptionElem = pTextNodeElem->FirstChildElement("Option");
	for (; NULL != pOptionElem; pOptionElem = pOptionElem->NextSiblingElement("Option"))
	{
		CTaskOption* pOptionNode = new CTaskOption;
		loadOptionNode(pOptionNode, pOptionElem);
		pMainNode->mOptionList.push_back(pOptionNode);
	}
}

void CTaskTemplate::loadOptionNode(CTaskOption* pOptionNode, XMLElement* pOptionNodeElem)
{
	wchar_t acBuffer[4096] = { 0 };
	Utf8ToUnicode(pOptionNodeElem->Attribute("Name"), acBuffer, sizeof(acBuffer) / 2 - 1);
	pOptionNode->mDes = acBuffer;

	XMLElement* pTextNodeElem = pOptionNodeElem->FirstChildElement("TextNode");
	for (; NULL != pTextNodeElem; pTextNodeElem = pTextNodeElem->NextSiblingElement("TextNode"))
	{
		CTaskMainNode* pMainNode = new CTaskMainNode;
		loadTextNode(pMainNode, pTextNodeElem);
		pOptionNode->mNodeList.push_back(pMainNode);
	}
}