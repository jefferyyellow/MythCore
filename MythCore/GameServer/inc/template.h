#ifndef __TEMPLATE_H__
#define __TEMPLATE_H__

#define STRING32					32
#define STRING256					256
#define MAX_TEMPLATE_NUM			100000	// ���ģ����
class CTemplate;
class CStaticData
{
public:
	static CTemplate* SearchTpl(unsigned int vTempID);

private:
	static CTemplate* spTemplate[MAX_TEMPLATE_NUM];						// ģ���
};


enum EmTemplateType
{
	emTemplateType_None				= 0,
	emTemplateType_FuncNPC			= 1,
	emTemplateType_Ogre				= 2,
	emTemplateTypeMax
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

	// FieldName: �ѵ�����
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
// virtual	= false						## �������ɵ����ݿ���
// name		= d�ȼ������				## ģ������
// path		= Config/d�ȼ������		## ·��
// parent	=							## ����ģ��
// ********************************************************************** //
class CTplPlayerLevelExpConfig : public CTemplate
{
public:
	// FieldName: ��%d��������һ������ֵ
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
	
};
#endif
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //