#ifndef __TEMPLATE_H__
#define __TEMPLATE_H__

#define STRING32					32
#define STRING256					256
#define MAX_TEMPLATE_NUM			100000	// 最大模版数
class CTemplate;
class CStaticData
{
public:
	static CTemplate* SearchTpl(unsigned int vTempID);

private:
	static CTemplate* spTemplate[MAX_TEMPLATE_NUM];						// 模版表
};


enum EmTemplateType
{
	emTemplateType_None				= 0,
	emTemplateType_FuncNPC			= 1,
	emTemplateType_Ogre				= 2,
	emTemplateTypeMax
};

/// 主要规则，所以的模板都以CTpl开头

// 所有模板的基类
class CTemplate
{
public:
	int					mTempID;

#ifndef TEMPEDIT
	EmTemplateType		mTemplateType;
#endif
};

// ********************************************************************** //
// BEGINMAKE
// virtual	= true						## 代表不生成到数据库中
// name		= 道具基本信息				## 模版名称
// path		=							## 路径
// parent	=							## 父亲模版
// ********************************************************************** //
class CTplItem : public CTemplate
{
public:
	// FieldName: $道具显示名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mName[STRING32];

	// FieldName: $物品详细说明
	// FieldType: STRING256
	// Type:	  EDITSTRING
	char	mDescribe[STRING256];

	// FieldName: 堆叠上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(1,10000)
	int		mPileLimit;

public:
	void	setFromPB();
	void	createToPB();
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

#define MAX_LEVEL		100
// ********************************************************************** //
// BEGINMAKE
// virtual	= false						## 代表不生成到数据库中
// name		= d等级经验表				## 模版名称
// path		= Config/d等级经验表		## 路径
// parent	=							## 父亲模版
// ********************************************************************** //
class CTplPlayerLevelExpConfig : public CTemplate
{
public:
	// FieldName: 第%d级升到下一级经验值
	// FieldType: INT4
	// Type:	  EDITNUMBER(1,10000000000000)
	int			mLevelUpExp[MAX_LEVEL];

public:
	static CTplPlayerLevelExpConfig* spConfig;
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

#define MAX_VIP_LEVEL 20
// ********************************************************************** //
// BEGINMAKE
// virtual	= false						## 代表不生成到数据库中
// name		= VIP经验表					## 模版名称
// path		= Config/VIP经验表			## 路径
// parent	=							## 父亲模版
// ********************************************************************** //
class CTplVIPConfig : public CTemplate
{
public:
	// FieldName: 第%d级VIP升到下一级经验值
	// FieldType: INT4
	// Type:	  EDITNUMBER(1,10000000000000)
	int			mVIPExp[MAX_VIP_LEVEL];
	
public:
	static CTplVIPConfig* spConfig;
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= true						## 代表不生成到数据库中
// name		= NPC基本信息				## 模版名称
// path		=							## 路径
// parent	=							## 父亲模版
// ********************************************************************** //
class CTplNPC : public CTemplate
{
	// FieldName: $NPC名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mName[STRING32];
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false						## 代表不生成到数据库中
// name		= g功能NPC					## 模版名称
// path		= NPC/g功能NPC				## 路径
// parent	= NPC基本信息				## 父亲模版
// ********************************************************************** //
class CTplFuncNPC : public CTplNPC
{

};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false						## 代表不生成到数据库中
// name		= g怪物						## 模版名称
// path		= NPC/g怪物					## 路径
// parent	= NPC基本信息				## 父亲模版
// ********************************************************************** //
class CTplOgre : public CTplNPC
{
public:
	
};
#endif
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //