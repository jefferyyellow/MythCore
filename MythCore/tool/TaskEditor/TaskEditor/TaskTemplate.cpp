#include "stdafx.h"
#include "TaskTemplate.h"
#include "commondefine.h"

/// 清除选择节点的内存
void CTaskTemplate::ClearOptionList(OPTION_LIST& rList)
{
	for (unsigned int i = 0; i < rList.size(); ++i)
	{
		ClearMainNodeList(rList[i]->mNodeList);
		delete rList[i];
	}
}

/// 清除主节点
void CTaskTemplate::ClearMainNodeList(TASK_NODE_LIST& rList)
{
	for (unsigned int  i = 0; i < rList.size(); ++ i)
	{
		ClearOptionList(rList[i]->mOptionList);
		delete rList[i];
	}
}

/// 清除选项名的列表
void CTaskTemplate::ClearOptionNameList(OPTION_NAME_LIST rList)
{
	for (unsigned int i = 0; i < rList.size(); ++i)
	{
		ClearOptionNameList(rList[i]->mChildList);
		delete rList[i];
	}
}

CTaskTemplate::~CTaskTemplate()
{
	ClearMainNodeList(mMainNodeList);
	ClearMainNodeList(mCondNodeList);
	ClearMainNodeList(mDiagNodeList);
	ClearOptionNameList(mOptionNameList);
}

/// 加载任务模板文件
void CTaskTemplate::LoadTaskTemplate(const char* pTemplateFile)
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

	// 主节点
	XMLElement* pTextNodeElem = pRoot->FirstChildElement("TextNode");
	for (; NULL != pTextNodeElem; pTextNodeElem = pTextNodeElem->NextSiblingElement("TextNode"))
	{
		CTaskMainNode* pMainNode = new CTaskMainNode;
		 LoadMainNode(pMainNode, pTextNodeElem, &mMainLangList);
		 mMainNodeList.push_back(pMainNode);
	}

	// 条件节点
	XMLElement* pCondNodeElem = pRoot->FirstChildElement("CondNode");
	for (; NULL != pCondNodeElem; pCondNodeElem = pCondNodeElem->NextSiblingElement("CondNode"))
	{
		CTaskMainNode* pMainNode = new CTaskMainNode;
		LoadMainNode(pMainNode, pCondNodeElem, NULL);
		mCondNodeList.push_back(pMainNode);
	}

	// 对话节点
	XMLElement* pDiagNodeElem = pRoot->FirstChildElement("DiagNode");
	for (; NULL != pDiagNodeElem; pDiagNodeElem = pDiagNodeElem->NextSiblingElement("DiagNode"))
	{
		CTaskMainNode* pMainNode = new CTaskMainNode;
		LoadMainNode(pMainNode, pDiagNodeElem, &mDiagLangList);
		mDiagNodeList.push_back(pMainNode);
	}
}

/// 加载主节点
void CTaskTemplate::LoadMainNode(CTaskMainNode* pMainNode, XMLElement* pTextNodeElem, INTERNATION_LIST* pInternationList)
{
	wchar_t acBuffer[4096] = { 0 };
	// 编辑显示的名字
	Utf8ToUnicode(pTextNodeElem->Attribute("Name"), acBuffer, sizeof(acBuffer) / 2 - 1);
	pMainNode->mName = acBuffer;

	// 写入配置文件的名字
	Utf8ToUnicode(pTextNodeElem->Attribute("DesName"), acBuffer, sizeof(acBuffer) / 2 - 1);
	pMainNode->mDesName = acBuffer;

	// 外置的配置文件
	const char* pConfigName = pTextNodeElem->Attribute("ConfigName");
	if (NULL != pConfigName)
	{
		Utf8ToUnicode(pConfigName, acBuffer, sizeof(acBuffer) / 2 - 1);
		pMainNode->mConfigName = acBuffer;
	}

	// 国际化语言
	bool bInternation = pTextNodeElem->BoolAttribute("Internation");
	if (NULL != pInternationList && bInternation)
	{
		pInternationList->push_back(pTextNodeElem->Attribute("DesName"));
	}

	// 选项列表
	XMLElement* pOptionElem = pTextNodeElem->FirstChildElement("Option");
	for (; NULL != pOptionElem; pOptionElem = pOptionElem->NextSiblingElement("Option"))
	{
		CTaskOption* pOptionNode = new CTaskOption;
		LoadOptionNode(pOptionNode, pOptionElem);
		pMainNode->mOptionList.push_back(pOptionNode);
	}
}

/// 加载选择节点
void CTaskTemplate::LoadOptionNode(CTaskOption* pOptionNode, XMLElement* pOptionNodeElem)
{
	wchar_t acBuffer[4096] = { 0 };
	Utf8ToUnicode(pOptionNodeElem->Attribute("Name"), acBuffer, sizeof(acBuffer) / 2 - 1);
	pOptionNode->mDes = acBuffer;

	XMLElement* pTextNodeElem = pOptionNodeElem->FirstChildElement("TextNode");
	for (; NULL != pTextNodeElem; pTextNodeElem = pTextNodeElem->NextSiblingElement("TextNode"))
	{
		CTaskMainNode* pMainNode = new CTaskMainNode;
		LoadMainNode(pMainNode, pTextNodeElem, NULL);
		pOptionNode->mNodeList.push_back(pMainNode);
	}
}

/// 加载选项节点名字文件（包括模板的树节点名字文件，地图节点名字文件）
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

	Utf8ToUnicode(pParentNodeElem->Attribute("name"), acBuffer, sizeof(acBuffer) / 2 - 1);
	pParentTempName->mName = acBuffer;

	int nID = pParentNodeElem->IntAttribute("id");
	if (nID > 0)
	{
		wchar_t acNameBuffer[4096] = { 0 };
		wsprintf(acNameBuffer, _T("%d,%s"), nID, acBuffer);
		pParentTempName->mName = acNameBuffer;
		rHashList[nID] = acNameBuffer;
	}
	else
	{
		pParentTempName->mName = acBuffer;
	}

	XMLElement* pElement = pParentNodeElem->FirstChildElement();
	for (; NULL != pElement; pElement = pElement->NextSiblingElement())
	{
		COptionNameItem* pTempName = new COptionNameItem;
		LoadItemName(pTempName, pElement, rHashList);
		pParentTempName->mChildList.push_back(pTempName);
	}
}

/// 通过ID得到选项的名字
wstring CTaskTemplate::FindOptionName(wstring strConfigName, int nID)
{
	for (unsigned int i = 0; i < mOptionNameHashList.size(); ++ i)
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

/// 通过名字得到任务的主节点
CTaskMainNode* CTaskTemplate::GetTaskMainNode(wstring strName)
{
	// 遍历主节点
	for (unsigned int i = 0; i < mMainNodeList.size(); ++ i)
	{
		if (mMainNodeList[i]->mDesName == strName)
		{
			return mMainNodeList[i];
		}
	}

	// 遍历条件节点
	for (unsigned int i = 0; i < mCondNodeList.size(); ++ i)
	{
		if (mCondNodeList[i]->mDesName == strName)
		{
			return mCondNodeList[i];
		}
	}

	// 遍历对话节点
	for (unsigned int i = 0; i < mDiagNodeList.size(); ++ i)
	{
		if (mDiagNodeList[i]->mDesName == strName)
		{
			return mDiagNodeList[i];
		}
	}

	return NULL;
}

/// 加载任务编辑器配置文件
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
		// 等级
		mLevelCond = pTaskLevelCondElem->IntAttribute("Value");
		// 等级节点的属性名
		Utf8ToUnicode(pTaskLevelCondElem->Attribute("LevelParam"), acBuffer, sizeof(acBuffer) / 2 - 1);
		mLevelParam = acBuffer;
		// 多少一个等级阶段
		mLevelPhase = pTaskLevelCondElem->IntAttribute("LevelPhase");
		// 最大等级
		mMaxLevel = pTaskLevelCondElem->IntAttribute("MaxLevel");
	}
}