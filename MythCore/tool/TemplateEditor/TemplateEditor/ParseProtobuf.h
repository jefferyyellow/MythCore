#ifndef _PARSE_PROTOBUF_H_
#define _PARSE_PROTOBUF_H_

/*#ifndef TIXML_USE_STL
#define TIXML_USE_STL
#endif*/

#include <string.h>
#include <stdio.h>
#include <string>
#include <vector>
#include "tinyxml2.h"

#include "google/protobuf/descriptor.h"
#include "google/protobuf/descriptor.pb.h"
using namespace tinyxml2;


typedef ::google::protobuf::Message Message;
typedef ::google::protobuf::Descriptor Descriptor;
typedef ::google::protobuf::Reflection Reflection;
typedef ::google::protobuf::FieldDescriptor FieldDescriptor;
typedef ::google::protobuf::EnumValueDescriptor EnumValueDescriptor;

#define MAX_INDEX_DIGIT 10 // 数字转字符串所能容纳的最大位数

#define  XML_ASSERT(x)          { if( NULL == (x) ) return -1; }
#define PB_NUM_MAX		100

// ParseCore(...) 函数专属宏，不得随意修改
#define CASE_PARSE_PB(PBClass)	\
{\
	PBClass tPBData;\
	if( !tPBData.ParseFromArray( pField[iCol].GetString(), pField[iCol].GetValueLen() ) )	\
{\
	printf("PB ParseFromArray failed\n");\
	return -1;\
	}\
	const Descriptor* tpDescriptor = tPBData.GetDescriptor();\
	if( tpDescriptor != NULL )\
{\
	tpPB = new XMLElement( tpDescriptor->name().c_str() );\
	}\
	tpRoot->LinkEndChild( tpPB );\
	Message2Xml( tPBData, tpPB );\
	\
	break;\
}\

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
	static const char* ToLower( char pSrc[], int nLen )
	{
		for (int i = 0; i < nLen ; ++i)
		{
			pSrc[i] = tolower( pSrc[i] );
		}
		return pSrc;
	}
	// 初始化
	void Initialize();
public:
	CParseProtobuf(void);
	~CParseProtobuf(void);

public:	// 解析PB
	int Message2Xml(const Message& rMessage, XMLElement* tpParent, int* pTempID);
	// 解析字段
	int Field2Xml(const Message& rMessage, const Reflection* pReflection, const FieldDescriptor* pFieldDescriptor, int nIndex, XMLElement* tpParent, int* pTempID);
private:
	tinyxml2::XMLDocument* mXmldoc;
};

#endif
