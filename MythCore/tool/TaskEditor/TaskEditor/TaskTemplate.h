#pragma once
// ����ģ����
// ���Ե��������������
#include <vector>
using namespace std;
class CTaskTempNode;
typedef vector<CTaskTempNode> TASK_NODE_LIST;

class CTaskTempNode
{
public:
	wstring			mName;
	wstring			mType;
	wstring			mEditType;
};

class CTaskTemplate
{
public:
	void	loadTaskTemplate(const char* pTemplateFile);

public:
	TASK_NODE_LIST	mNodeList;
};