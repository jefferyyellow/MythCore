#pragma once
// ����ģ����
// ���Ե��������������
#include <vector>
using namespace std;



class CTaskOption
{
	wstring			mDes;
	int				mValue;
};

class CTaskTempNode
{
public:
	wstring			mName;
};

class CTaskTemplate
{
public:
	typedef vector<CTaskTempNode> TASK_NODE_LIST;

public:
	void	loadTaskTemplate(const char* pTemplateFile);

public:
	TASK_NODE_LIST	mNodeList;
};