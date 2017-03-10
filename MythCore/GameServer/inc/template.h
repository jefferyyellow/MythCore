#ifndef __TEMPLATE_H__
#define __TEMPLATE_H__

#define STRING32					32
#define STRING256					256
#define MAX_TEMPLATE_NUM			100000	// 最大模版数
class CTemplate;
class CDataStatic
{
public:
	static CTemplate* SearchTpl(unsigned int vTempID);

private:
	static CTemplate* spTemplate[MAX_TEMPLATE_NUM];						// 模版表
};


enum EmTemplateType
{
	emTemplateType_None			= 0,
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
};
#endif
