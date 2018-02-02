#pragma once
// 任务模板类
// 所以的任务从这里衍生
#include "commondefine.h"
#include <vector>
#include <hash_map>
using namespace std;

class CTaskOption;
class CTaskMainNode;
class COptionNameItem;
class COptionNameHash;

typedef vector<CTaskOption*> OPTION_LIST;
typedef vector<CTaskMainNode*> TASK_NODE_LIST;

typedef vector<COptionNameItem*>	OPTION_NAME_LIST;
typedef hash_map<int, wstring>		OPTION_NAME_HASH;
typedef vector<COptionNameHash>		OPTION_NAME_HASH_LIST;
class COptionNameItem
{
public:
	wstring			mName;
	OPTION_NAME_LIST	mChildList;
};

class COptionNameHash
{
public:
	wstring				mName;
	OPTION_NAME_HASH	mHashList;
};


class CTaskOption
{
public:
	wstring			mDes;
	TASK_NODE_LIST	mNodeList;
};

class CTaskMainNode
{
public:

public:
	wstring			mName;
	wstring			mDesName;
	wstring			mConfigName;
	OPTION_LIST		mOptionList;
};

class CTaskTemplate
{
public:
	CTaskTemplate()
	{

	}
	~CTaskTemplate();

public:
	void	loadTaskTemplate(const char* pTemplateFile);
	void	loadTextNode(CTaskMainNode* pMainNode, XMLElement* pTextNodeElem);
	void	loadOptionNode(CTaskOption* pOptionNode, XMLElement* pOptionNodeElem);
	void	ClearOptionList(OPTION_LIST& rList);
	void	ClearMainNodeList(TASK_NODE_LIST& rList);
	void	ClearOptionNameList(OPTION_NAME_LIST rList);

	void	LoadTempName(const char* pTempNameFile);
	void	LoadItemName(COptionNameItem* pParentTempName, XMLElement* pParentNodeElem, OPTION_NAME_HASH& rHashList);
	wstring FindOptionName(wstring strConfigName, int nID);
public:
	TASK_NODE_LIST		mTextNodeList;
	TASK_NODE_LIST		mCondNodeList;
	TASK_NODE_LIST		mDiagNodeList;
	OPTION_NAME_LIST	mOptionNameList;
	OPTION_NAME_HASH_LIST mOptionNameHashList;
};

