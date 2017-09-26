#ifndef _PARSE_PROTOBUF_H_
#define _PARSE_PROTOBUF_H_

/*#ifndef TIXML_USE_STL
#define TIXML_USE_STL
#endif*/
#include "ProtoXMLManager.h"
#include "DBMsgFactory.h"

#include <string.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <sk_vector.h>

#include "DatabaseMysql.h"
#include "QueryResultMysql.h"

#include "tinyxml.h"
#include "tinystr.h"
#include "sk_hashmap.h"
#include "servertool.h"

#include "db_msg_pb.hxx.pb.h"

#include "property_pb.hxx.pb.h"
#include "servermessage_pb.hxx.pb.h"
#include "arenamessage_pb.hxx.pb.h"
#include "activitymessag_pb.hxx.pb.h"
#include "factionmessage_pb.hxx.pb.h"

#include "google/protobuf/descriptor.h"
#include "google/protobuf/descriptor.pb.h"

Message* GetMessage(Message*& pRoot, const char* pTableName, const char* pFieldName);
Message* GetSubMessage(Message*& pRoot, const char* pFieldName);

typedef ::google::protobuf::Message Message;
typedef ::google::protobuf::Descriptor Descriptor;
typedef ::google::protobuf::Reflection Reflection;
typedef ::google::protobuf::FieldDescriptor FieldDescriptor;
typedef ::google::protobuf::EnumValueDescriptor EnumValueDescriptor;

#define MAX_INDEX_DIGIT 10 // 数字转字符串所能容纳的最大位数

#define PB_NUM_MAX		100

// 必须 使用 "mHashMapMacro" 作为参数传递
// 目前 mHashMapMacro 只能是 mField2emPBMap、mPBName2PBMap
#define InsertField2PB_HashMap( mHashMapMacro, STRING, emPB )	\
	tPairib = mHashMapMacro.insert( String2emPB_HashMap::value_type( STRING , emPB) )	\
	;if ( tPairib.second == false )\
{\
	printf(" " #emPB" insert Failed !\n");\
	return;\
	}\

// 参数要求
// PBClass：PB 类型名
// FieldName：字段名，必须先转换为小写
// isRepeated：只能是 true 或者 false
#define FillPBData( PBClass, FieldName, tLowerFieldName, FieldValue, isRepeated )\
{\
	PBClass tPBData;\
	if( isRepeated )\
	{\
	tPBData.add_##tLowerPBName( FieldValue );\
	}\
	else\
	{\
	FieldName* pbData = tPBData.set_##tLowerPBName( FieldValue );\
	}\
}\


#define UMS_FORM_MAX 10
#define UMS_FIELD_NUM_MAX 30					// Blob字段的最大数量
#define UMS_GLOBAL_DATA_FIELDS_NUM 11

class CParseProtobuf
{
public:
	typedef sk::hash_map<std::string, int, PB_NUM_MAX> String2emPB_HashMap;
	typedef sk::hash_map<std::string, std::string, UMS_FORM_MAX> Form2Field_HashMap;// “表名——Blob字段名集合”的映射
public:

	// 初始化
	void Initialize();
	// 初始化数据库连接
	bool InitializeMysql(const char* pIP, int nPort, const char* pUser, const char* pPasswd, const char* pDBName);

	// 根据查询结果解析 PB
	int ParseCore( const char* pFormName, const char* pFieldName, const char* pPrimaryKeyVal, bool bAll );
	// 根据配置解析
	int ParseByConfig();
	// 清除结点
	void ClearNodes(TiXmlDocument* tpXmlDoc);
	// 在某一连接下切换别的数据库
	bool SwitchOtherDataBase(const char* pDataBase);
	
	
	// 把xml写回到数据库
	int SerializeToArray( const char* pName );
	// 根据 xml 根结点下首个子结点，即数据库字段主信息结点，得到对应的PB对象 pMessage
	void XmlToProbuff(TiXmlElement* pXmlNode, Message* pMessage);

	// 根据 xml 结点对象生成相应的 PB 对象
	int CreatePBCore( const char* pFormName, const char* pFieldName, const char* pPrimaryKeyVal );
	// 根据配置解析
	int WriteByConfig();
	// 把xml写回到数据库
	int SerializeToDataBase( Message* pMessage, const char* pFormName, const char* pFieldName, const char* pPrimaryName, const char* pPrimaryKeyVal );

	//void TestPBData( const PBPlayerBasicInfo& tPBData );
public:
	CParseProtobuf(void);
	~CParseProtobuf(void);

private:
	/* 下面这部分，以后转移到 DBMsgFactory.h 里 */

	// 【数据库表名】→【表包含Blob字段名】的映射（批量读取数据库）
	Form2Field_HashMap mForm2FieldHashMap;

	// 以后考虑是否把 CreateForm2FieldMapping()、CreateField2PBMapping() 所需信息整合到配置文件
	// 乱七八糟的东西太多了......
	// 建立“表名——Blob字段名集合”的映射
	void CreateForm2FieldMapping();


private:
	// 数据库连接是否成功
	bool mbMySql;
	// 数据库连接对象
	CDatabaseMysql mMysql;


	// 【数据库中字段】→【它的emPB 枚举】的映射（读取数据库）
	String2emPB_HashMap mField2emPBMap;
	// 【PB 名】→【对应的 emPB 枚举】的映射（写回数据库）
	String2emPB_HashMap mPBName2PBMap;
};

#endif
