#pragma once
// 任务模板类
// 所以的任务从这里衍生
#include <vector>
using namespace std;

class CTaskOption;
class CTaskTempNode;
typedef vector<CTaskOption> OPTION_LIST;
typedef vector<CTaskTempNode> TASK_NODE_LIST;

class CTaskOption
{
public:

public:
	wstring			mDes;
	int				mValue;
	TASK_NODE_LIST	mNodeList;
};

class CTaskTempNode
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

public:
	TASK_NODE_LIST	mNodeList;
};