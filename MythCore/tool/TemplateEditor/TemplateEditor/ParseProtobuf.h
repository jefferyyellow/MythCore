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

#define MAX_INDEX_DIGIT 10 // ����ת�ַ����������ɵ����λ��

#define  XML_ASSERT(x)          { if( NULL == (x) ) return -1; }
#define PB_NUM_MAX		100

// ParseCore(...) ����ר���꣬���������޸�
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

// ���� ʹ�� "mHashMapMacro" ��Ϊ��������
// Ŀǰ mHashMapMacro ֻ���� mField2emPBMap��mPBName2PBMap
#define InsertField2PB_HashMap( mHashMapMacro, STRING, emPB )	\
	tPairib = mHashMapMacro.insert( String2emPB_HashMap::value_type( STRING , emPB) )	\
	;if ( tPairib.second == false )\
{\
	printf(" " #emPB" insert Failed !\n");\
	return;\
	}\

// ����Ҫ��
// PBClass��PB ������
// FieldName���ֶ�����������ת��ΪСд
// isRepeated��ֻ���� true ���� false
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
#define UMS_FIELD_NUM_MAX 30					// Blob�ֶε��������
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
	// ��ʼ��
	void Initialize();
public:
	CParseProtobuf(void);
	~CParseProtobuf(void);

public:	// ����PB
	int Message2Xml(const Message& rMessage, XMLElement* tpParent, int* pTempID);
	// �����ֶ�
	int Field2Xml(const Message& rMessage, const Reflection* pReflection, const FieldDescriptor* pFieldDescriptor, int nIndex, XMLElement* tpParent, int* pTempID);
private:
	tinyxml2::XMLDocument* mXmldoc;
};

#endif
