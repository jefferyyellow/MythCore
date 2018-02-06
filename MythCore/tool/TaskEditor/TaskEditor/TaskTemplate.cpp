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

void CTaskTemplate::ClearOptionNameList(OPTION_NAME_LIST rList)
{
	for (int i = 0; i < rList.size(); ++i)
	{
		ClearOptionNameList(rList[i]->mChildList);
		delete rList[i];
	}
}

CTaskTemplate::~CTaskTemplate()
{
	ClearMainNodeList(mTextNodeList);
	ClearMainNodeList(mCondNodeList);
	ClearMainNodeList(mDiagNodeList);
	ClearOptionNameList(mOptionNameList);
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
		 loadTextNode(pMainNode, pTextNodeElem, &mMainLangList);
		 mTextNodeList.push_back(pMainNode);
	}

	XMLElement* pCondNodeElem = pRoot->FirstChildElement("CondNode");
	for (; NULL != pCondNodeElem; pCondNodeElem = pCondNodeElem->NextSiblingElement("CondNode"))
	{
		CTaskMainNode* pMainNode = new CTaskMainNode;
		loadTextNode(pMainNode, pCondNodeElem, NULL);
		mCondNodeList.push_back(pMainNode);
	}

	XMLElement* pDiagNodeElem = pRoot->FirstChildElement("DiagNode");
	for (; NULL != pDiagNodeElem; pDiagNodeElem = pDiagNodeElem->NextSiblingElement("DiagNode"))
	{
		CTaskMainNode* pMainNode = new CTaskMainNode;
		loadTextNode(pMainNode, pDiagNodeElem, &mDiagLangList);
		mDiagNodeList.push_back(pMainNode);
	}
}


void CTaskTemplate::loadTextNode(CTaskMainNode* pMainNode, XMLElement* pTextNodeElem, INTERNATION_LIST* pInternationList)
{
	wchar_t acBuffer[4096] = { 0 };
	Utf8ToUnicode(pTextNodeElem->Attribute("Name"), acBuffer, sizeof(acBuffer) / 2 - 1);
	pMainNode->mName = acBuffer;

	Utf8ToUnicode(pTextNodeElem->Attribute("DesName"), acBuffer, sizeof(acBuffer) / 2 - 1);
	pMainNode->mDesName = acBuffer;

	const char* pConfigName = pTextNodeElem->Attribute("ConfigName");
	if (NULL != pConfigName)
	{
		Utf8ToUnicode(pConfigName, acBuffer, sizeof(acBuffer) / 2 - 1);
		pMainNode->mConfigName = acBuffer;
	}

	bool bInternation = pTextNodeElem->BoolAttribute("Internation");
	if (NULL != pInternationList && bInternation)
	{
		pInternationList->push_back(pTextNodeElem->Attribute("DesName"));
	}

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
		loadTextNode(pMainNode, pTextNodeElem, NULL);
		pOptionNode->mNodeList.push_back(pMainNode);
	}
}

void CTaskTemplate::LoadItemNameFile(const char* pTempNameFile)
{
	if (NULL == pTempNameFile)
	{
		return;
	}
	tinyxml2::XMLDocument tDocument;
	if (XML_SUCCESS != tDocument.LoadFile(pTempNameFile))
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
	COptionNameItem* pTempName = new COptionNameItem;
	if (NULL == pTempName)
	{
		return;
	}
	COptionNameHash tOptionHastItem;

	LoadItemName(pTempName, pRoot, tOptionHastItem.mHashList);
	mOptionNameList.push_back(pTempName);

	tOptionHastItem.mName = pTempName->mName;
	mOptionNameHashList.push_back(tOptionHastItem);
}

void CTaskTemplate::LoadItemName(COptionNameItem* pParentTempName, XMLElement* pParentNodeElem, OPTION_NAME_HASH& rHashList)
{
	if (NULL == pParentNodeElem)
	{
		return;
	}

	wchar_t acBuffer[4096] = { 0 };
	Utf8ToUnicode(pParentNodeElem->Attribute("Name"), acBuffer, sizeof(acBuffer) / 2 - 1);
	pParentTempName->mName = acBuffer;
	CString strName = acBuffer;
	int nPos = strName.Find(',');
	if (nPos > 0)
	{
		CString strNum = strName.Left(nPos);
		int nNum = _ttoi(strNum);
		if (nNum > 0)
		{
			rHashList[nNum]= strName;
		}
	}

	XMLElement* pElement = pParentNodeElem->FirstChildElement();
	for (; NULL != pElement; pElement = pElement->NextSiblingElement())
	{
		COptionNameItem* pTempName = new COptionNameItem;
		LoadItemName(pTempName, pElement, rHashList);
		pParentTempName->mChildList.push_back(pTempName);
	}
}

wstring CTaskTemplate::FindOptionName(wstring strConfigName, int nID)
{
	for (int i = 0; i < mOptionNameHashList.size(); ++ i)
	{
		if (strConfigName == mOptionNameHashList[i].mName)
		{
			OPTION_NAME_HASH::iterator it = mOptionNameHashList[i].mHashList.find(nID);
			if (it == mOptionNameHashList[i].mHashList.end())
			{
				return _T("");
			}
			else
			{
				return it->second;
			}
		}
	}

	return _T("");
}

CTaskMainNode* CTaskTemplate::GetTaskMainNode(wstring strName)
{
	for (int i = 0; i < mTextNodeList.size(); ++ i)
	{
		if (mTextNodeList[i]->mDesName == strName)
		{
			return mTextNodeList[i];
		}
	}

	for (int i = 0; i < mCondNodeList.size(); ++ i)
	{
		if (mCondNodeList[i]->mDesName == strName)
		{
			return mCondNodeList[i];
		}
	}

	for (int i = 0; i < mDiagNodeList.size(); ++ i)
	{
		if (mDiagNodeList[i]->mDesName == strName)
		{
			return mDiagNodeList[i];
		}
	}

	return NULL;
}

void CTaskEditorConfig::LoadTaskEditorConfig(const char* pConfigFile)
{
	if (NULL == pConfigFile)
	{
		return;
	}
	tinyxml2::XMLDocument tDocument;
	if (XML_SUCCESS != tDocument.LoadFile(pConfigFile))
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
	wchar_t acBuffer[4096] = { 0 };

	XMLElement* pTaskLevelCondElem = pRoot->FirstChildElement("TaskLevelCond");
	if (NULL != pTaskLevelCondElem)
	{
		mLevelCond = pTaskLevelCondElem->IntAttribute("Value");
		Utf8ToUnicode(pTaskLevelCondElem->Attribute("LevelParam"), acBuffer, sizeof(acBuffer) / 2 - 1);
		mLevelParam = acBuffer;
		mLevelPhase = pTaskLevelCondElem->IntAttribute("LevelPhase");
		mMaxLevel = pTaskLevelCondElem->IntAttribute("MaxLevel");
	}
}