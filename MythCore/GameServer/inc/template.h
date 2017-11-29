#ifndef __TEMPLATE_H__
#define __TEMPLATE_H__

#ifdef TEMPEDIT
#pragma pack(push)
#pragma pack(4)
#endif

#define STRING32					32
#define STRING256					256
#define MAX_TEMPLATE_NUM			100000				// 最大模版数
#define TEMPLATE_CACHE_SIZE			10 * 1024 * 1024	// 大Cache
class CTemplate;
class PBTplTemplate;
class PBTplItem;
class PBTplLevelExpConfig;
class PBTplVIPConfig;
class PBTplNPC;
class PBTplFuncNPC;
class PBTplOgre;

#define TEMPLATE_SET_FROM_PB(ClassName, PBParent, PBName) \
	{ \
			for (int i = 0; i < (int)PBParent->PBName##_size(); ++ i) \
			{ \
				void* pBuff = malloc(sizeof(ClassName)); \
				if (NULL == pBuff) \
				{ \
					break; \
				} \
				memset(pBuff, 0, sizeof(ClassName)); \
				ClassName* pTemplate = new(pBuff) ClassName; \
				pTemplate->setFromPB(PBParent->mutable_##PBName(i)); \
				if (spTemplate[pTemplate->mTempID] != NULL) \
				{ \
					free(spTemplate[pTemplate->mTempID]); \
					spTemplate[pTemplate->mTempID] = NULL; \
				} \
				spTemplate[pTemplate->mTempID] = pTemplate; \
			} \
	}


class CStaticData
{

public:
	/// 从文件中加载
	static bool	loadFromFile(const char* pFilePath);
	/// 从PB中创建
	static void	createFromPB(PBTplTemplate* pTplTemplate);
public:
	static CTemplate* searchTpl(unsigned int vTempID);

private:
	static CTemplate* spTemplate[MAX_TEMPLATE_NUM];						// 模版表

public:
	static int	mVersion;			// 版本号
};


enum EmTemplateType
{
	emTemplateType_None				= 0,
	emTemplateType_FuncNPC			= 1,
	emTemplateType_Ogre				= 2,
	emTemplateTypeMax
};


enum EmPropertyType
{
	emPropertyType_MaxHP			= 0,	// 最大血值
	emPropertyType_MaxMP			= 1,	// 最大魔值
	emPropertyType_Attack			= 2,	// 攻击力
	emPropertyType_Defence			= 3,	// 防御力
	emPropertyTypeMax
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

	CTemplate()
	{
		mTempID = 0;
#ifndef TEMPEDIT
		mTemplateType = emTemplateType_None;
#endif
	}

	~CTemplate() {}
};


enum EmItemType
{
	emItemType_None		= 0,		// 无类型
	emItemType_Common	= 1,		// 通用道具
	emItemType_Equip	= 2,		// 装备道具
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

	// FieldName: 道具类型
	// FieldType: INT4
	// Type:	  COMBOENUM(EmItemType)
	int		mItemType;

	// FieldName: 堆叠上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(1,10000)
	int		mPileLimit;

public:
	CTplItem(){}
	~CTplItem(){}
	void	setFromPB(PBTplItem* pbItem);
	void	createToPB(PBTplItem* pbItem);
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
class CTplLevelExpConfig : public CTemplate
{
public:
	// FieldName: 第%d级升到下一级经验值
	// FieldType: INT4
	// Type:	  EDITNUMBER(1,10000000000000)
	int			mLevelUpExp[MAX_LEVEL];

public:
	static CTplLevelExpConfig* spConfig;

public:
	CTplLevelExpConfig(){}
	~CTplLevelExpConfig(){}

	void	setFromPB(PBTplLevelExpConfig* pbConfig);
	void	createToPB(PBTplLevelExpConfig* pbConfig);
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

public:
	CTplVIPConfig(){}
	~CTplVIPConfig(){}

	void	setFromPB(PBTplVIPConfig* pbConfig);
	void	createToPB(PBTplVIPConfig* pbConfig);
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

public:
	CTplNPC(){}
	~CTplNPC(){}

	void	setFromPB(PBTplNPC* pbNpc);
	void	createToPB(PBTplNPC* pbNpc);
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
public:
	CTplFuncNPC()
	{
#ifndef TEMPEDIT
		mTemplateType = emTemplateType_FuncNPC;
#endif
	}
	~CTplFuncNPC(){}
	void	setFromPB(PBTplFuncNPC* pbFuncNpc);
	void	createToPB(PBTplFuncNPC* pbFuncNpc);
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
	CTplOgre()
	{
#ifndef TEMPEDIT
		mTemplateType = emTemplateType_Ogre;
#endif
	}
	~CTplOgre(){}

	void	setFromPB(PBTplOgre* pbOgre);
	void	createToPB(PBTplOgre* pbOgre);
};

#ifdef TEMPEDIT
#pragma pack(pop)
#endif
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //
#endif