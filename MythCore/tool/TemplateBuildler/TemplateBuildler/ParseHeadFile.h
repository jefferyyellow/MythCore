#pragma once
/********************************************************************
	created:	12/10/2011   11:29
	filename: 	ParseHeadFile.h
	author:		jeffery

	purpose:	分配template.h用来生成原始模板文件
	comment:	模板头文件中枚举的用法太纠结,
				1.有全局枚举,
				2.有类的嵌套枚举
				3.一个类可以引用父模板的枚举
				4.一个类还可以引用其它没有关系的模板的枚举
				为了不分析语法,所以我做了如果的枚举搜索,这种在某种情况下有BUG
				先搜索自己的嵌套枚举,如果没找着,递归找父模板的枚举
				如果还没找着,再找全局的枚举,如果还没找着就会挨着去找了,
				但如果一个枚举在全局和某一个模板就声明了,但目标模板不是声明模板的子模板就会出问题,
				但暂时没有这种情况,如果有这种情况再另外加枚举的搜索信息
*********************************************************************/
extern string	GetFirstWord(string& strSource);

// 枚举项信息
class CEnumItemInfo
{
public:
	string	m_strKey;
	string	m_strValue;
};

// 枚举信息
class CEnumInfo
{
public:
	CEnumInfo(){}
	~CEnumInfo(){}

public:
	//hash_map<string, string>		m_EnumInfo;// 枚举的键值对
	vector<CEnumItemInfo>			m_vecEnumInfo;
};

// 字段信息
class CFieldInfo
{
public:
	CFieldInfo(){}
	~CFieldInfo(){}
	CFieldInfo& operator = (const CFieldInfo& field)
	{
		if(this == &field)
		{
			return *this;
		}
		m_strFieldName = field.m_strFieldName;
		m_strFieldType = field.m_strFieldType;
		m_strType = field.m_strType;

		return *this;
	}

public:
	string	m_strFieldName;		// 字段名称
	string	m_strFieldType;		// 字段类型
	string	m_strType;			// 类型

};

//嵌套模板类型，支持多级嵌套


// 原始模板信息
class CTemplateInfo;
typedef hash_map<string, CTemplateInfo*> nestTempMap;

class CTemplateInfo
{
public:
	CTemplateInfo(){}
	~CTemplateInfo(){}

public:
	BOOL	m_bVirtual;				// 代表生成到数据库中
	string	m_strName;				// 模版名称
	string	m_strVersion;			// 版本号
	string	m_strPath;				// 路径
	string	m_strParent;			// 父亲模板

	vector<CFieldInfo>	m_vecFieldInfo;	// 字段集
	CFieldInfo m_groupfield;		//嵌套类信息
	// 嵌套的枚举表
	hash_map<string, CEnumInfo>		m_hashEnum;
	nestTempMap m_nestTemplates;	//子模板集
};

// 分配template.h类
class CParseHeadFile
{
public:
	enum
	{
		// 每从文件中取一行用的BUFF大小
		emLine_Max_Char		= 1024,
	};

public:
	CParseHeadFile(void);
	~CParseHeadFile(void);
	

public:

	// 开始分析模板头文件
	BOOL	BeginParseFile(String strFilePath);
	// 分析宏
	BOOL	ParseMacro(string strLine);
	// 分析枚举,如果pinfo为NULL表示全局枚举,如果不为NULL,表示某一个模板的嵌套枚举
	BOOL	ParseEnum(CTemplateInfo* pinfo, ifstream& InFile, string strEnumLine);
	// 分析模板头
	BOOL	ParseTemplateHead(CTemplateInfo& info, ifstream& InFile);
	// 分析模板
	BOOL	ParseTemplate(ifstream& InFile);
	// 分析模板体
	BOOL	ParseTemplateBody(CTemplateInfo& info, ifstream& InFile, int nRepatedNum1 = -1, int nRepeatedNum2 = -1);
	// 将字符串的第一个非空格与TAB开始的串转换成数字
	int		GetFirstDigit(string& strSource);
	// 分析重复字段,"%d"情况
	BOOL	ParseRepeateField(CTemplateInfo& info, string strLine);
	//处理嵌套类型模板，递归执行展开
	BOOL	HandleNestTemplate(CTemplateInfo *info, ifstream& InFile);
	//展开数组类型字段
	BOOL	UnfoldNestTemplate(CTemplateInfo*info, string strline);
	// 从当前的模板类及其父类里查找枚举信息
	BOOL	GetEnumInfo(CTemplateInfo& rInfo, string strEnum, hash_map<string, CEnumInfo>::iterator& itResult);
	// 在全局枚举信息里查找,如果找不着就能所有的模板的嵌套枚举信息
	BOOL	GetEnumInfo(string strEnum, hash_map<string, CEnumInfo>::iterator& itResult);
	//释放当前模板下所有嵌套模板
	void	ReleaseTp(CTemplateInfo &info);

	// 宏表
	hash_map<string, int>			m_hashMacro;
	// 枚举表
	hash_map<string, CEnumInfo>		m_hashEnum;
	// 模板表
	vector<CTemplateInfo>			m_vecTemplate;
	// "#define"字符串的长度
	const int						m_nDefineLen;
	// "enum"字符串的长度
	const int						m_nEnumLen;
	// 重复字段模板,用于保存"%d"情况下的字段,用于生成最终的字段集
	vector<CFieldInfo>				m_RepeatvecFieldInfo;	// 字段集
};
