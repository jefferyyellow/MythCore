#pragma once
// ����ģ����
// ���Ե��������������
#include "commondefine.h"
#include <vector>
using namespace std;

class CTaskOption;
class CTaskMainNode;
typedef vector<CTaskOption*> OPTION_LIST;
typedef vector<CTaskMainNode*> TASK_NODE_LIST;

class CTaskOption
{
public:

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

public:
	TASK_NODE_LIST	mTextNodeList;
	TASK_NODE_LIST	mCondNodeList;
	TASK_NODE_LIST	mDiagNodeList;
};