#pragma once
/********************************************************************
	created:	12/10/2011   11:29
	filename: 	ParseHeadFile.h
	author:		jeffery

	purpose:	����template.h��������ԭʼģ���ļ�
	comment:	ģ��ͷ�ļ���ö�ٵ��÷�̫����,
				1.��ȫ��ö��,
				2.�����Ƕ��ö��
				3.һ����������ø�ģ���ö��
				4.һ���໹������������û�й�ϵ��ģ���ö��
				Ϊ�˲������﷨,���������������ö������,������ĳ���������BUG
				�������Լ���Ƕ��ö��,���û����,�ݹ��Ҹ�ģ���ö��
				�����û����,����ȫ�ֵ�ö��,�����û���žͻᰤ��ȥ����,
				�����һ��ö����ȫ�ֺ�ĳһ��ģ���������,��Ŀ��ģ�岻������ģ�����ģ��ͻ������,
				����ʱû���������,�������������������ö�ٵ�������Ϣ
*********************************************************************/
extern string	GetFirstWord(string& strSource);

// ö������Ϣ
class CEnumItemInfo
{
public:
	string	m_strKey;
	string	m_strValue;
};

// ö����Ϣ
class CEnumInfo
{
public:
	CEnumInfo(){}
	~CEnumInfo(){}

public:
	//hash_map<string, string>		m_EnumInfo;// ö�ٵļ�ֵ��
	vector<CEnumItemInfo>			m_vecEnumInfo;
};

// �ֶ���Ϣ
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
	string	m_strFieldName;		// �ֶ�����
	string	m_strFieldType;		// �ֶ�����
	string	m_strType;			// ����

};

//Ƕ��ģ�����ͣ�֧�ֶ༶Ƕ��


// ԭʼģ����Ϣ
class CTemplateInfo;
typedef hash_map<string, CTemplateInfo*> nestTempMap;

class CTemplateInfo
{
public:
	CTemplateInfo(){}
	~CTemplateInfo(){}

public:
	BOOL	m_bVirtual;				// �������ɵ����ݿ���
	string	m_strName;				// ģ������
	string	m_strVersion;			// �汾��
	string	m_strPath;				// ·��
	string	m_strParent;			// ����ģ��

	vector<CFieldInfo>	m_vecFieldInfo;	// �ֶμ�
	CFieldInfo m_groupfield;		//Ƕ������Ϣ
	// Ƕ�׵�ö�ٱ�
	hash_map<string, CEnumInfo>		m_hashEnum;
	nestTempMap m_nestTemplates;	//��ģ�弯
};

// ����template.h��
class CParseHeadFile
{
public:
	enum
	{
		// ÿ���ļ���ȡһ���õ�BUFF��С
		emLine_Max_Char		= 1024,
	};

public:
	CParseHeadFile(void);
	~CParseHeadFile(void);
	

public:

	// ��ʼ����ģ��ͷ�ļ�
	BOOL	BeginParseFile(String strFilePath);
	// ������
	BOOL	ParseMacro(string strLine);
	// ����ö��,���pinfoΪNULL��ʾȫ��ö��,�����ΪNULL,��ʾĳһ��ģ���Ƕ��ö��
	BOOL	ParseEnum(CTemplateInfo* pinfo, ifstream& InFile, string strEnumLine);
	// ����ģ��ͷ
	BOOL	ParseTemplateHead(CTemplateInfo& info, ifstream& InFile);
	// ����ģ��
	BOOL	ParseTemplate(ifstream& InFile);
	// ����ģ����
	BOOL	ParseTemplateBody(CTemplateInfo& info, ifstream& InFile, int nRepatedNum1 = -1, int nRepeatedNum2 = -1);
	// ���ַ����ĵ�һ���ǿո���TAB��ʼ�Ĵ�ת��������
	int		GetFirstDigit(string& strSource);
	// �����ظ��ֶ�,"%d"���
	BOOL	ParseRepeateField(CTemplateInfo& info, string strLine);
	//����Ƕ������ģ�壬�ݹ�ִ��չ��
	BOOL	HandleNestTemplate(CTemplateInfo *info, ifstream& InFile);
	//չ�����������ֶ�
	BOOL	UnfoldNestTemplate(CTemplateInfo*info, string strline);
	// �ӵ�ǰ��ģ���༰�丸�������ö����Ϣ
	BOOL	GetEnumInfo(CTemplateInfo& rInfo, string strEnum, hash_map<string, CEnumInfo>::iterator& itResult);
	// ��ȫ��ö����Ϣ�����,����Ҳ��ž������е�ģ���Ƕ��ö����Ϣ
	BOOL	GetEnumInfo(string strEnum, hash_map<string, CEnumInfo>::iterator& itResult);
	//�ͷŵ�ǰģ��������Ƕ��ģ��
	void	ReleaseTp(CTemplateInfo &info);

	// ���
	hash_map<string, int>			m_hashMacro;
	// ö�ٱ�
	hash_map<string, CEnumInfo>		m_hashEnum;
	// ģ���
	vector<CTemplateInfo>			m_vecTemplate;
	// "#define"�ַ����ĳ���
	const int						m_nDefineLen;
	// "enum"�ַ����ĳ���
	const int						m_nEnumLen;
	// �ظ��ֶ�ģ��,���ڱ���"%d"����µ��ֶ�,�����������յ��ֶμ�
	vector<CFieldInfo>				m_RepeatvecFieldInfo;	// �ֶμ�
};
