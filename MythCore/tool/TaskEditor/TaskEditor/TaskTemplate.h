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
typedef vector<string>				INTERNATION_LIST;
/// 模板，地图的树状图的节点
class COptionNameItem
{
public:
	/// 选项名
	wstring			mName;
	OPTION_NAME_LIST	mChildList;
};

class COptionNameHash
{
public:
	/// hash表名(模板，地图）
	wstring				mName;
	OPTION_NAME_HASH	mHashList;
};


/// 对应任务模板文件的选项节点
class CTaskOption
{
public:
	/// 选项描述
	wstring			mDes;
	/// 下面的节点列表
	TASK_NODE_LIST	mNodeList;
};

/// 对应任务模板文件的主节点
class CTaskMainNode
{
public:

public:
	/// 主节点名字（显示）
	wstring			mName;
	/// 描述名字（写配置文件)
	wstring			mDesName;
	/// 外部的配置文件（如模板，地图配置）
	wstring			mConfigName;
	/// 选项列表
	OPTION_LIST		mOptionList;
};

// 任务模板，所有的任务以此为模板
class CTaskTemplate
{
public:
	CTaskTemplate()
	{

	}
	~CTaskTemplate();

public:
	/// 加载任务模板文件
	void	LoadTaskTemplate(const char* pTemplateFile);
	/// 加载主节点
	void	LoadMainNode(CTaskMainNode* pMainNode, XMLElement* pTextNodeElem, INTERNATION_LIST* pInternationList);
	/// 加载选择节点
	void	LoadOptionNode(CTaskOption* pOptionNode, XMLElement* pOptionNodeElem);
	/// 清除选择节点的内存
	void	ClearOptionList(OPTION_LIST& rList);
	/// 清除主节点
	void	ClearMainNodeList(TASK_NODE_LIST& rList);
	/// 清除选项名的列表
	void	ClearOptionNameList(OPTION_NAME_LIST rList);

	/// 加载选项节点名字文件（包括模板的树节点名字文件，地图节点名字文件）
	void	LoadItemNameFile(const char* pTempNameFile);
	void	LoadItemName(COptionNameItem* pParentTempName, XMLElement* pParentNodeElem, OPTION_NAME_HASH& rHashList);
	/// 通过ID得到选项的名字
	wstring FindOptionName(wstring strConfigName, int nID);
	/// 通过名字得到任务的主节点
	CTaskMainNode* GetTaskMainNode(wstring strName);

public:
	/// 任务模板分成三部分（主节点,条件节点，对话节点）
	TASK_NODE_LIST		mMainNodeList;		/// 主节点
	TASK_NODE_LIST		mCondNodeList;		/// 条件节点
	TASK_NODE_LIST		mDiagNodeList;		/// 对话节点

	OPTION_NAME_LIST		mOptionNameList;		/// 模板的树节点，地图节点的树状结构图
	OPTION_NAME_HASH_LIST	mOptionNameHashList;	/// 模板的树节点，地图节点hash表，先用名字代表是模板还是地图，然后就是hash表，通过ID可以找到值

	// 主节点中需要转换语言的节点
	INTERNATION_LIST	mMainLangList;
	// 对话中需要转换语言的节点
	INTERNATION_LIST	mDiagLangList;
};

class CTaskEditorConfig
{
public:
	/// 等级条件
	int					mLevelCond;
	/// 等级参数
	wstring				mLevelParam;
	/// 等级段
	int					mLevelPhase;
	/// 最大的等级
	int					mMaxLevel;
public:
	/// 加载任务编辑器配置文件
	void	LoadTaskEditorConfig(const char* pConfigFile);

};