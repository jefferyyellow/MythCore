#include "stdafx.h"
#include "TaskTemplate.h"
#include "commondefine.h"

/// ���ѡ��ڵ���ڴ�
void CTaskTemplate::ClearOptionList(OPTION_LIST& rList)
{
	for (unsigned int i = 0; i < rList.size(); ++i)
	{
		ClearMainNodeList(rList[i]->mNodeList);
		delete rList[i];
	}
}

/// ������ڵ�
void CTaskTemplate::ClearMainNodeList(TASK_NODE_LIST& rList)
{
	for (unsigned int  i = 0; i < rList.size(); ++ i)
	{
		ClearOptionList(rList[i]->mOptionList);
		delete rList[i];
	}
}

/// ���ѡ�������б�
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

/// ��������ģ���ļ�
void CTaskTemplate::LoadTaskTemplate(const char* pTemplateFile)
{
	if (NULL == pTemplateFile)
	{
		return;
	}
	tinyxml2::XMLDocument tDocument;
	if (XML_SUCCESS != tDocument.LoadFile(pTemplateFile))
	{
		// �����޷�����xml�ļ�
		return;
	}

	XMLElement* pRoot = tDocument.RootElement();
	if (NULL == pRoot)
	{
		// ������Root�ڵ㶼û��
		return;
	}

	// ���ڵ�
	XMLElement* pTextNodeElem = pRoot->FirstChildElement("TextNode");
	for (; NULL != pTextNodeElem; pTextNodeElem = pTextNodeElem->NextSiblingElement("TextNode"))
	{
		CTaskMainNode* pMainNode = new CTaskMainNode;
		 LoadMainNode(pMainNode, pTextNodeElem, &mMainLangList);
		 mMainNodeList.push_back(pMainNode);
	}

	// �����ڵ�
	XMLElement* pCondNodeElem = pRoot->FirstChildElement("CondNode");
	for (; NULL != pCondNodeElem; pCondNodeElem = pCondNodeElem->NextSiblingElement("CondNode"))
	{
		CTaskMainNode* pMainNode = new CTaskMainNode;
		LoadMainNode(pMainNode, pCondNodeElem, NULL);
		mCondNodeList.push_back(pMainNode);
	}

	// �Ի��ڵ�
	XMLElement* pDiagNodeElem = pRoot->FirstChildElement("DiagNode");
	for (; NULL != pDiagNodeElem; pDiagNodeElem = pDiagNodeElem->NextSiblingElement("DiagNode"))
	{
		CTaskMainNode* pMainNode = new CTaskMainNode;
		LoadMainNode(pMainNode, pDiagNodeElem, &mDiagLangList);
		mDiagNodeList.push_back(pMainNode);
	}
}

/// �������ڵ�
void CTaskTemplate::LoadMainNode(CTaskMainNode* pMainNode, XMLElement* pTextNodeElem, INTERNATION_LIST* pInternationList)
{
	wchar_t acBuffer[4096] = { 0 };
	// �༭��ʾ������
	Utf8ToUnicode(pTextNodeElem->Attribute("Name"), acBuffer, sizeof(acBuffer) / 2 - 1);
	pMainNode->mName = acBuffer;

	// д�������ļ�������
	Utf8ToUnicode(pTextNodeElem->Attribute("DesName"), acBuffer, sizeof(acBuffer) / 2 - 1);
	pMainNode->mDesName = acBuffer;

	// ���õ������ļ�
	const char* pConfigName = pTextNodeElem->Attribute("ConfigName");
	if (NULL != pConfigName)
	{
		Utf8ToUnicode(pConfigName, acBuffer, sizeof(acBuffer) / 2 - 1);
		pMainNode->mConfigName = acBuffer;
	}

	// ���ʻ�����
	bool bInternation = pTextNodeElem->BoolAttribute("Internation");
	if (NULL != pInternationList && bInternation)
	{
		pInternationList->push_back(pTextNodeElem->Attribute("DesName"));
	}

	// ѡ���б�
	XMLElement* pOptionElem = pTextNodeElem->FirstChildElement("Option");
	for (; NULL != pOptionElem; pOptionElem = pOptionElem->NextSiblingElement("Option"))
	{
		CTaskOption* pOptionNode = new CTaskOption;
		LoadOptionNode(pOptionNode, pOptionElem);
		pMainNode->mOptionList.push_back(pOptionNode);
	}
}

/// ����ѡ��ڵ�
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

/// ����ѡ��ڵ������ļ�������ģ������ڵ������ļ�����ͼ�ڵ������ļ���
void CTaskTemplate::LoadItemNameFile(const char* pTempNameFile)
{
	if (NULL == pTempNameFile)
	{
		return;
	}
	tinyxml2::XMLDocument tDocument;
	if (XML_SUCCESS != tDocument.LoadFile(pTempNameFile))
	{
		// �����޷�����xml�ļ�
		return;
	}

	XMLElement* pRoot = tDocument.RootElement();
	if (NULL == pRoot)
	{
		// ������Root�ڵ㶼û��
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

/// ͨ��ID�õ�ѡ�������
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

/// ͨ�����ֵõ���������ڵ�
CTaskMainNode* CTaskTemplate::GetTaskMainNode(wstring strName)
{
	// �������ڵ�
	for (unsigned int i = 0; i < mMainNodeList.size(); ++ i)
	{
		if (mMainNodeList[i]->mDesName == strName)
		{
			return mMainNodeList[i];
		}
	}

	// ���������ڵ�
	for (unsigned int i = 0; i < mCondNodeList.size(); ++ i)
	{
		if (mCondNodeList[i]->mDesName == strName)
		{
			return mCondNodeList[i];
		}
	}

	// �����Ի��ڵ�
	for (unsigned int i = 0; i < mDiagNodeList.size(); ++ i)
	{
		if (mDiagNodeList[i]->mDesName == strName)
		{
			return mDiagNodeList[i];
		}
	}

	return NULL;
}

/// ��������༭�������ļ�
void CTaskEditorConfig::LoadTaskEditorConfig(const char* pConfigFile)
{
	if (NULL == pConfigFile)
	{
		return;
	}
	tinyxml2::XMLDocument tDocument;
	if (XML_SUCCESS != tDocument.LoadFile(pConfigFile))
	{
		// �����޷�����xml�ļ�
		return;
	}

	XMLElement* pRoot = tDocument.RootElement();
	if (NULL == pRoot)
	{
		// ������Root�ڵ㶼û��
		return;
	}
	wchar_t acBuffer[4096] = { 0 };

	XMLElement* pTaskLevelCondElem = pRoot->FirstChildElement("TaskLevelCond");
	if (NULL != pTaskLevelCondElem)
	{
		// �ȼ�
		mLevelCond = pTaskLevelCondElem->IntAttribute("Value");
		// �ȼ��ڵ��������
		Utf8ToUnicode(pTaskLevelCondElem->Attribute("LevelParam"), acBuffer, sizeof(acBuffer) / 2 - 1);
		mLevelParam = acBuffer;
		// ����һ���ȼ��׶�
		mLevelPhase = pTaskLevelCondElem->IntAttribute("LevelPhase");
		// ���ȼ�
		mMaxLevel = pTaskLevelCondElem->IntAttribute("MaxLevel");
	}
}