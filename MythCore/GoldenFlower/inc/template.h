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
class PBItem;
class PBTplLevelExpConfig;
class PBTplVIPConfig;
class PBTplNPC;
class PBTplFuncNPC;
class PBTplOgre;
class PBTplEquip;
class PBTplProperty;
class PBTplDropItem;
class PBTplDropTable;
class PBTplSkill;
class PBTplNewPlayerConfig;
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
	emTemplateType_FuncNpc			= 1,		// 功能NPC
	emTemplateType_Ogre				= 2,		// 怪物
	emTemplateType_Item				= 3,		// 道具
	emTemplateType_DropTable		= 4,		// 掉落表
	emTemplateTypeMax
};


enum EmPropertyType
{
	emPropertyType_None				= 0,	// 无
	emPropertyType_MaxHp			= 1,	// 最大血值
	emPropertyType_MaxMp			= 2,	// 最大魔值
	emPropertyType_Attack			= 3,	// 攻击力
	emPropertyType_Defence			= 4,	// 防御力
	emPropertyTypeMax
};

enum EmBuffEffect
{
	emBuffEffect_None				= 0,	// 无
	emBuffEffect_Hp					= 1,	// 加/减血
	emBuffEffect_Mp					= 2,	// 加/减魔
	emBuffEffectMax
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


// ********************************************************************** //
// BEGINMAKE
// virtual	= true						## 代表不生成到数据库中
// name		= 新玩家信息				## 模版名称
// path		=							## 路径
// parent	=							## 父亲模版
// ********************************************************************** //
#define MAX_NEW_PLAYER_ITEM		8
class CTplNewPlayerConfig : public CTemplate
{
public:
	// FieldName: 第%d个道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	int		mItemID[MAX_NEW_PLAYER_ITEM];

	// FieldName: 第%d个道具数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(1,10000000)
	int		mItemNum[MAX_NEW_PLAYER_ITEM];

	// FieldName: 初始等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(1,10000000)
	int		mLevel;

	// FieldName: 初始Vip等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(1,10000000)
	int		mVipLevel;

public:
	static CTplNewPlayerConfig* spConfig;

public:
	CTplNewPlayerConfig()
	{
		spConfig = this;
	};
	~CTplNewPlayerConfig(){};
	void setFromPB(PBTplNewPlayerConfig* pbData);
	void createToPB(PBTplNewPlayerConfig* pbData);
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

enum EmItemType
{
	emItemType_None		= 0,		// 无类型
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

	// FieldName: 卖出价格
	// FieldType: INT4
	// Type:	  EDITNUMBER(1,10000)
	int		mSellPrice;

public:
	CTplItem()
	{
#ifndef TEMPEDIT
		mTemplateType = emTemplateType_Item;
#endif
	}
	~CTplItem(){}
	void	setFromPB(PBItem* pbItem);
	void	createToPB(PBItem* pbItem);
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false						## 
// name		= z装备						## 模版名称
// path		=							## 路径
// parent	=							## 父亲模版
// ********************************************************************** //
#define EQUIP_PROPERTY_NUM	6					// 装备最大的属性数目
class CTplEquip : public CTplItem
{
public:
	class CProperty
	{
	public:
		// FieldName: 类型
		// FieldType: INT4
		// Type:	  COMBOENUM(EmPropertyType)
		int		mType;

		// FieldName: 值
		// FieldType: INT4
		// Type:	  EDITNUMBER(0,1000)
		int		mValue;

	public:
		void setFromPB(PBTplProperty* pbData);
		void createToPB(PBTplProperty* pbData);
	};

public:
	// FieldName: 装备模型名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mEquipModal[STRING32];

	// FieldName: 装备部位
	// FieldType: INT4
	// Type:	  COMBOENUM(TmTplEquipPart)
	int		mEquipPart;

	// FieldName: 等级限制
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mLevelRequire;

	// FieldName: 第%d项属性
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	CProperty	mProperty[EQUIP_PROPERTY_NUM];

public:
	CTplEquip(){};
	~CTplEquip(){};
	void setFromPB(PBTplEquip* pbData);
	void createToPB(PBTplEquip* pbData);

};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

#define MAX_LEVEL		100
// ********************************************************************** //
// BEGINMAKE
// virtual	= false						##
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
	CTplLevelExpConfig()
	{
		spConfig = this;
	}
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
// virtual	= false						##
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
	CTplVIPConfig()
	{
		spConfig = this;
	}
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
// virtual	= false						##
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
		mTemplateType = emTemplateType_FuncNpc;
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
// virtual	= false						##
// name		= g怪物						## 模版名称
// path		= NPC/g怪物					## 路径
// parent	= NPC基本信息				## 父亲模版
// ********************************************************************** //
#define MAX_OGRE_DROP	4
class CTplOgre : public CTplNPC
{
public:
	// FieldName: 杀怪经验
	// FieldType: INT4
	// Type:	  EDITNUMBER(1,100000000)
	int			mExp;

	// FieldName: 怪物掉落
	// FieldType: INT4
	// Type:	  TEMPPATH
	int			mDropTable[MAX_OGRE_DROP];

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
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false						## 
// name		= j技能						## 模版名称
// path		= j技能/j技能				## 路径
// parent	=							## 父亲模版
// ********************************************************************** //
class CTplSkill : public CTemplate
{
public:
	// FieldName: $技能名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mName[STRING32];

	// FieldName: $技能描述
	// FieldType: STRING256
	// Type:	  EDITSTRING
	char	mDescription[STRING256];

	// FieldName: 技能CD(秒)
	// FieldType: INT4
	// Type:	  EDITNUMBER(1,10000)
	int		mSkillCD;

	// FieldName: 附加伤害
	// FieldType: INT4
	// Type:	  EDITNUMBER(1,10000)
	int		mAddDamage;

public:
	CTplSkill(){};
	~CTplSkill(){};
	void setFromPB(PBTplSkill* pbData);
	void createToPB(PBTplSkill* pbData);
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false						## 
// name		= d掉落表					## 模版名称
// path		= d掉落表					## 路径
// parent	=							## 父亲模版
// ********************************************************************** //
#define MAX_DROP_ITEM		16	// 最大的掉落道具
class CTplDropTable : public CTemplate
{
public:
	class CDropItem
	{
	public:
		// FieldName: 道具ID
		// FieldType: INT4
		// Type:	  TEMPPATH
		int		mItemID;

		// FieldName: 道具数目
		// FieldType: INT4
		// Type:	  EDITNUMBER(1,10000)
		int		mItemNum;

		// FieldName: 几率
		// FieldType: INT4
		// Type:	  EDITNUMBER(1,10000)
		int		mProbability;

		void setFromPB(PBTplDropItem* pbData);
		void createToPB(PBTplDropItem* pbData);

	};

	// FieldName: 第%d个掉落
	// FieldType: CLASS
	// Type:	  CLASS
	CDropItem	mDropItem[MAX_DROP_ITEM];

public:
	CTplDropTable()
	{
#ifndef TEMPEDIT
		mTemplateType = emTemplateType_DropTable;
#endif
	};
	~CTplDropTable(){};
	void setFromPB(PBTplDropTable* pbData);
	void createToPB(PBTplDropTable* pbData);
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //
#ifdef TEMPEDIT
#pragma pack(pop)
#endif
#endif