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
	emTemplateType_FuncNPC			= 1,
	emTemplateType_Ogre				= 2,
	emTemplateTypeMax
};


enum EmPropertyType
{
	emPropertyType_MaxHP			= 0,	// ���Ѫֵ
	emPropertyType_MaxMP			= 1,	// ���ħֵ
	emPropertyType_Attack			= 2,	// ������
	emPropertyType_Defence			= 3,	// ������
	emPropertyTypeMax
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


enum EmItemType
{
	emItemType_None		= 0,		// ������
	emItemType_Common	= 1,		// ͨ�õ���
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
// virtual	= false						## �������ɵ����ݿ���
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
// virtual	= false						## �������ɵ����ݿ���
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
// virtual	= false						## �������ɵ����ݿ���
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
// virtual	= false						## �������ɵ����ݿ���
// name		= g����						## ģ������
// path		= NPC/g����					## ·��
// parent	= NPC������Ϣ				## ����ģ��
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