#pragma once
// 任务模板类
// 所以的任务从这里衍生
#include <vector>
using namespace std;
#include "tinyxml2.h"
using namespace tinyxml2;

class CTaskOption;
class CTaskMainNode;
typedef vector<CTaskOption*> OPTION_LIST;
typedef vector<CTaskMainNode*> TASK_NODE_LIST;

enum TmNodeType
{
	emNodeType_Main
};

class CTaskOption
{
public:

public:
	wstring			mDes;
	int				mValue;
	TASK_NODE_LIST	mNodeList;
};

class CTaskMainNode
{
public:

public:
	wstring			mName;
	OPTION_LIST		mOptionList;
};

class CTaskTemplate
{
public:
	

public:
	void	loadTaskTemplate(const char* pTemplateFile);
	void	loadTextNode(CTaskMainNode* pMainNode, XMLElement* pTextNodeElem);
	void	loadOptionNode(CTaskOption* pOptionNode, XMLElement* pOptionNodeElem);

public:
	TASK_NODE_LIST	mTextNodeList;
	TASK_NODE_LIST	mCondNodeList;
	TASK_NODE_LIST	mDiagNodeList;
};