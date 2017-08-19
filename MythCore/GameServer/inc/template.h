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
	emTemplateType_None			= 0,
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

	CTemplate()
	{
		mTempID = 0;
#ifndef TEMPEDIT
		mTemplateType = emTemplateType_None;
#endif
	}

	~CTemplate() {}
};

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
};

#define MAX_LEVEL		100
class CTplPlayerLevelUpExpConfig : public CTemplate
{
public:
	// FieldName: ��%d��������һ������ֵ
	// FieldType: INT4
	// Type:	  EDITNUMBER(1,10000000000000)
	int			mLevelUpExp[MAX_LEVEL];

public:
	static CTplPlayerLevelUpExpConfig* spConfig;
};

#define MAX_VIP_LEVEL 20
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
#endif
