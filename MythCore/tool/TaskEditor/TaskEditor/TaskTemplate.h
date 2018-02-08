#pragma once
// ����ģ����
// ���Ե��������������
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
typedef vector<string>				INTERNATION_LIST;
/// ģ�壬��ͼ����״ͼ�Ľڵ�
class COptionNameItem
{
public:
	/// ѡ����
	wstring			mName;
	OPTION_NAME_LIST	mChildList;
};

class COptionNameHash
{
public:
	/// hash����(ģ�壬��ͼ��
	wstring				mName;
	OPTION_NAME_HASH	mHashList;
};


/// ��Ӧ����ģ���ļ���ѡ��ڵ�
class CTaskOption
{
public:
	/// ѡ������
	wstring			mDes;
	/// ����Ľڵ��б�
	TASK_NODE_LIST	mNodeList;
};

/// ��Ӧ����ģ���ļ������ڵ�
class CTaskMainNode
{
public:

public:
	/// ���ڵ����֣���ʾ��
	wstring			mName;
	/// �������֣�д�����ļ�)
	wstring			mDesName;
	/// �ⲿ�������ļ�����ģ�壬��ͼ���ã�
	wstring			mConfigName;
	/// ѡ���б�
	OPTION_LIST		mOptionList;
};

// ����ģ�壬���е������Դ�Ϊģ��
class CTaskTemplate
{
public:
	CTaskTemplate()
	{

	}
	~CTaskTemplate();

public:
	/// ��������ģ���ļ�
	void	LoadTaskTemplate(const char* pTemplateFile);
	/// �������ڵ�
	void	LoadMainNode(CTaskMainNode* pMainNode, XMLElement* pTextNodeElem, INTERNATION_LIST* pInternationList);
	/// ����ѡ��ڵ�
	void	LoadOptionNode(CTaskOption* pOptionNode, XMLElement* pOptionNodeElem);
	/// ���ѡ��ڵ���ڴ�
	void	ClearOptionList(OPTION_LIST& rList);
	/// ������ڵ�
	void	ClearMainNodeList(TASK_NODE_LIST& rList);
	/// ���ѡ�������б�
	void	ClearOptionNameList(OPTION_NAME_LIST rList);

	/// ����ѡ��ڵ������ļ�������ģ������ڵ������ļ�����ͼ�ڵ������ļ���
	void	LoadItemNameFile(const char* pTempNameFile);
	void	LoadItemName(COptionNameItem* pParentTempName, XMLElement* pParentNodeElem, OPTION_NAME_HASH& rHashList);
	/// ͨ��ID�õ�ѡ�������
	wstring FindOptionName(wstring strConfigName, int nID);
	/// ͨ�����ֵõ���������ڵ�
	CTaskMainNode* GetTaskMainNode(wstring strName);

public:
	/// ����ģ��ֳ������֣����ڵ�,�����ڵ㣬�Ի��ڵ㣩
	TASK_NODE_LIST		mMainNodeList;		/// ���ڵ�
	TASK_NODE_LIST		mCondNodeList;		/// �����ڵ�
	TASK_NODE_LIST		mDiagNodeList;		/// �Ի��ڵ�

	OPTION_NAME_LIST		mOptionNameList;		/// ģ������ڵ㣬��ͼ�ڵ����״�ṹͼ
	OPTION_NAME_HASH_LIST	mOptionNameHashList;	/// ģ������ڵ㣬��ͼ�ڵ�hash���������ִ�����ģ�廹�ǵ�ͼ��Ȼ�����hash��ͨ��ID�����ҵ�ֵ

	// ���ڵ�����Ҫת�����ԵĽڵ�
	INTERNATION_LIST	mMainLangList;
	// �Ի�����Ҫת�����ԵĽڵ�
	INTERNATION_LIST	mDiagLangList;
};

class CTaskEditorConfig
{
public:
	/// �ȼ�����
	int					mLevelCond;
	/// �ȼ�����
	wstring				mLevelParam;
	/// �ȼ���
	int					mLevelPhase;
	/// ���ĵȼ�
	int					mMaxLevel;
public:
	/// ��������༭�������ļ�
	void	LoadTaskEditorConfig(const char* pConfigFile);

};