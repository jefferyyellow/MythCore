#ifndef __TEMPLATE_H__
#define __TEMPLATE_H__

#ifdef TEMPEDIT
#pragma pack(push)
#pragma pack(4)
#endif

#define STRING32					32
#define STRING256					256
#define MAX_TEMPLATE_NUM			65536				// 最大模版数
#define TEMPLATE_CACHE_SIZE			10 * 1024 * 1024	// 大Cache
class CTemplate;
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

class PBTplTemplate;
class CStaticData
{

public:
	/// 从文件中加载
	static bool	loadFromFile(const char* pFilePath);
	/// 从PB中创建
	static void	createFromPB(PBTplTemplate* pTplTemplate);
	static void clearTemplate();
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

enum EmProType
{
	emProType_None				= 0,		// 无类型
	emProType_MaxHp				= 1,		// 最大血值固定值(0x0001)
	emProType_MaxMp				= 2,		// 最大魔值固定值(0x0002)
	emProType_Attack			= 3,		// 最大攻击力固定值(0x0003)
	emProType_Defence			= 4,		// 最大防御力固定值(0x0004)
	emProTypeMax				   ,		
	emProType_MaxHpPer			= 257,		// 最大血值百分比(0x0101)
	emProType_MaxMpPer			= 258,		// 最大魔值百分比(0x0102)
	emProType_AttackPer			= 259,		// 最大攻击力百分比(0x0103)
	emProType_DefencePer		= 260,		// 最大防御力百分比(0x0104)
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
// virtual	= false						## 代表不生成到数据库中
// name		= x新玩家信息				## 模版名称
// path		= Config/x新玩家信息		## 路径
// parent	=							## 父亲模版
// ********************************************************************** //
class PBTplNewPlayerConfig;
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
	// Type:	  EDITNUMBER(0,10000000)
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
class PBItem;
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
// path		= z装备						## 路径
// parent	= 道具基本信息				## 父亲模版
// ********************************************************************** //
class PBTplEquip;
class PBTplProperty;
#define EQUIP_PROPERTY_NUM	6					// 装备最大的属性数目
enum EmTplEquipPart
{
	emEquipPart_Hat				= 0,			// 帽子
	emEquipPart_Clothes			= 1,			// 衣服
	emEquipPart_Shoes			= 2,			// 鞋子
	emEquipPart_Weapon			= 3,			// 武器
	emEquipPart_Ring			= 4,			// 戒指
	emEquipPart_Accessory		= 5,			// 饰品
	emEquipPartMax
};
class CTplEquip : public CTplItem
{
public:
	class CProperty
	{
	public:
		// FieldName: 类型
		// FieldType: INT4
		// Type:	  COMBOENUM(EmProType)
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
	// Type:	  COMBOENUM(EmTplEquipPart)
	int		mEquipPart;

	// FieldName: 等级限制
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mLevelRequire;

	// FieldName: 第%d项属性
	// FieldType: CLASS
	// Type:	  CLASS
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


// ********************************************************************** //
// BEGINMAKE
// virtual	= false						##
// name		= d等级经验表				## 模版名称
// path		= Config/d等级经验表		## 路径
// parent	=							## 父亲模版
// ********************************************************************** //
#define MAX_LEVEL		100
class PBTplLevelExpConfig;
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
class PBTplVIPConfig;
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
class PBTplNPC;
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
class PBTplFuncNPC;
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
class PBTplOgre;
#define MAX_OGRE_DROP	4
class CTplOgre : public CTplNPC
{
public:
	// FieldName: 杀怪经验
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)
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
// path		= j技能						## 路径
// parent	=							## 父亲模版
// ********************************************************************** //
class PBTplSkill;
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

	// FieldName: 技能CD(毫秒)
	// FieldType: INT4
	// Type:	  EDITNUMBER(1,10000)
	int		mSkillCD;

	// FieldName: 附加伤害
	// FieldType: INT4
	// Type:	  EDITNUMBER(1,10000)
	int		mAddDamage;

	// FieldName: 准备时间(毫秒)
	// FieldType: INT4
	// Type:	  EDITNUMBER(1,10000)
	int		mPrepareTime;

	// FieldName: 释放时间
	// FieldType: INT4
	// Type:	  EDITNUMBER(1,10000)
	int		mActionTime;

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
class PBTplDropItem;
class PBTplDropTable;
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