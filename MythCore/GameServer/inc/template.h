#ifndef __TEMPLATE_H__
#define __TEMPLATE_H__

#ifdef TEMPEDIT
#pragma pack(push)
#pragma pack(4)
#endif

#define STRING32					32
#define STRING256					256
#define MAX_TEMPLATE_NUM			100000				// ���ģ����
#define TEMPLATE_CACHE_SIZE			10 * 1024 * 1024	// ��Cache
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
	/// ���ļ��м���
	static bool	loadFromFile(const char* pFilePath);
	/// ��PB�д���
	static void	createFromPB(PBTplTemplate* pTplTemplate);
public:
	static CTemplate* searchTpl(unsigned int vTempID);

private:
	static CTemplate* spTemplate[MAX_TEMPLATE_NUM];						// ģ���

public:
	static int	mVersion;			// �汾��
};


enum EmTemplateType
{
	emTemplateType_None				= 0,
	emTemplateType_FuncNpc			= 1,		// ����NPC
	emTemplateType_Ogre				= 2,		// ����
	emTemplateType_Item				= 3,		// ����
	emTemplateType_DropTable		= 4,		// �����
	emTemplateTypeMax
};


enum EmPropertyType
{
	emPropertyType_None				= 0,	// ��
	emPropertyType_MaxHp			= 1,	// ���Ѫֵ
	emPropertyType_MaxMp			= 2,	// ���ħֵ
	emPropertyType_Attack			= 3,	// ������
	emPropertyType_Defence			= 4,	// ������
	emPropertyTypeMax
};

enum EmBuffEffect
{
	emBuffEffect_None				= 0,	// ��
	emBuffEffect_Hp					= 1,	// ��/��Ѫ
	emBuffEffect_Mp					= 2,	// ��/��ħ
	emBuffEffectMax
};

/// ��Ҫ�������Ե�ģ�嶼��CTpl��ͷ

// ����ģ��Ļ���
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
// virtual	= true						## �������ɵ����ݿ���
// name		= �������Ϣ				## ģ������
// path		=							## ·��
// parent	=							## ����ģ��
// ********************************************************************** //
#define MAX_NEW_PLAYER_ITEM		8
class CTplNewPlayerConfig : public CTemplate
{
public:
	// FieldName: ��%d������ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	int		mItemID[MAX_NEW_PLAYER_ITEM];

	// FieldName: ��%d����������
	// FieldType: INT4
	// Type:	  EDITNUMBER(1,10000000)
	int		mItemNum[MAX_NEW_PLAYER_ITEM];

	// FieldName: ��ʼ�ȼ�
	// FieldType: INT4
	// Type:	  EDITNUMBER(1,10000000)
	int		mLevel;

	// FieldName: ��ʼVip�ȼ�
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
	emItemType_None		= 0,		// ������
	emItemType_Equip	= 2,		// װ������
};
// ********************************************************************** //
// BEGINMAKE
// virtual	= true						## �������ɵ����ݿ���
// name		= ���߻�����Ϣ				## ģ������
// path		=							## ·��
// parent	=							## ����ģ��
// ********************************************************************** //
class CTplItem : public CTemplate
{
public:
	// FieldName: $������ʾ����
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mName[STRING32];

	// FieldName: $��Ʒ��ϸ˵��
	// FieldType: STRING256
	// Type:	  EDITSTRING
	char	mDescribe[STRING256];

	// FieldName: ��������
	// FieldType: INT4
	// Type:	  COMBOENUM(EmItemType)
	int		mItemType;

	// FieldName: �ѵ�����
	// FieldType: INT4
	// Type:	  EDITNUMBER(1,10000)
	int		mPileLimit;

	// FieldName: �����۸�
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
// name		= zװ��						## ģ������
// path		=							## ·��
// parent	=							## ����ģ��
// ********************************************************************** //
#define EQUIP_PROPERTY_NUM	6					// װ������������Ŀ
class CTplEquip : public CTplItem
{
public:
	class CProperty
	{
	public:
		// FieldName: ����
		// FieldType: INT4
		// Type:	  COMBOENUM(EmPropertyType)
		int		mType;

		// FieldName: ֵ
		// FieldType: INT4
		// Type:	  EDITNUMBER(0,1000)
		int		mValue;

	public:
		void setFromPB(PBTplProperty* pbData);
		void createToPB(PBTplProperty* pbData);
	};

public:
	// FieldName: װ��ģ������
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mEquipModal[STRING32];

	// FieldName: װ����λ
	// FieldType: INT4
	// Type:	  COMBOENUM(TmTplEquipPart)
	int		mEquipPart;

	// FieldName: �ȼ�����
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mLevelRequire;

	// FieldName: ��%d������
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
// name		= d�ȼ������				## ģ������
// path		= Config/d�ȼ������		## ·��
// parent	=							## ����ģ��
// ********************************************************************** //
class CTplLevelExpConfig : public CTemplate
{
public:
	// FieldName: ��%d��������һ������ֵ
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
// name		= VIP�����					## ģ������
// path		= Config/VIP�����			## ·��
// parent	=							## ����ģ��
// ********************************************************************** //
class CTplVIPConfig : public CTemplate
{
public:
	// FieldName: ��%d��VIP������һ������ֵ
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
// virtual	= true						## �������ɵ����ݿ���
// name		= NPC������Ϣ				## ģ������
// path		=							## ·��
// parent	=							## ����ģ��
// ********************************************************************** //
class CTplNPC : public CTemplate
{
	// FieldName: $NPC����
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
// name		= g����NPC					## ģ������
// path		= NPC/g����NPC				## ·��
// parent	= NPC������Ϣ				## ����ģ��
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
// name		= g����						## ģ������
// path		= NPC/g����					## ·��
// parent	= NPC������Ϣ				## ����ģ��
// ********************************************************************** //
#define MAX_OGRE_DROP	4
class CTplOgre : public CTplNPC
{
public:
	// FieldName: ɱ�־���
	// FieldType: INT4
	// Type:	  EDITNUMBER(1,100000000)
	int			mExp;

	// FieldName: �������
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
// name		= j����						## ģ������
// path		= j����/j����				## ·��
// parent	=							## ����ģ��
// ********************************************************************** //
class CTplSkill : public CTemplate
{
public:
	// FieldName: $��������
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mName[STRING32];

	// FieldName: $��������
	// FieldType: STRING256
	// Type:	  EDITSTRING
	char	mDescription[STRING256];

	// FieldName: ����CD(��)
	// FieldType: INT4
	// Type:	  EDITNUMBER(1,10000)
	int		mSkillCD;

	// FieldName: �����˺�
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
// name		= d�����					## ģ������
// path		= d�����					## ·��
// parent	=							## ����ģ��
// ********************************************************************** //
#define MAX_DROP_ITEM		16	// ���ĵ������
class CTplDropTable : public CTemplate
{
public:
	class CDropItem
	{
	public:
		// FieldName: ����ID
		// FieldType: INT4
		// Type:	  TEMPPATH
		int		mItemID;

		// FieldName: ������Ŀ
		// FieldType: INT4
		// Type:	  EDITNUMBER(1,10000)
		int		mItemNum;

		// FieldName: ����
		// FieldType: INT4
		// Type:	  EDITNUMBER(1,10000)
		int		mProbability;

		void setFromPB(PBTplDropItem* pbData);
		void createToPB(PBTplDropItem* pbData);

	};

	// FieldName: ��%d������
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