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

#define MAX_INDEX_DIGIT 10 // ����ת�ַ����������ɵ����λ��

#define PB_NUM_MAX		100

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
	typedef sk::hash_map<std::string, int, PB_NUM_MAX> String2emPB_HashMap;
	typedef sk::hash_map<std::string, std::string, UMS_FORM_MAX> Form2Field_HashMap;// ����������Blob�ֶ������ϡ���ӳ��
public:

	// ��ʼ��
	void Initialize();
	// ��ʼ�����ݿ�����
	bool InitializeMysql(const char* pIP, int nPort, const char* pUser, const char* pPasswd, const char* pDBName);

	// ���ݲ�ѯ������� PB
	int ParseCore( const char* pFormName, const char* pFieldName, const char* pPrimaryKeyVal, bool bAll );
	// �������ý���
	int ParseByConfig();
	// ������
	void ClearNodes(TiXmlDocument* tpXmlDoc);
	// ��ĳһ�������л�������ݿ�
	bool SwitchOtherDataBase(const char* pDataBase);
	
	
	// ��xmlд�ص����ݿ�
	int SerializeToArray( const char* pName );
	// ���� xml ��������׸��ӽ�㣬�����ݿ��ֶ�����Ϣ��㣬�õ���Ӧ��PB���� pMessage
	void XmlToProbuff(TiXmlElement* pXmlNode, Message* pMessage);

	// ���� xml ������������Ӧ�� PB ����
	int CreatePBCore( const char* pFormName, const char* pFieldName, const char* pPrimaryKeyVal );
	// �������ý���
	int WriteByConfig();
	// ��xmlд�ص����ݿ�
	int SerializeToDataBase( Message* pMessage, const char* pFormName, const char* pFieldName, const char* pPrimaryName, const char* pPrimaryKeyVal );

	//void TestPBData( const PBPlayerBasicInfo& tPBData );
public:
	CParseProtobuf(void);
	~CParseProtobuf(void);

private:
	/* �����ⲿ�֣��Ժ�ת�Ƶ� DBMsgFactory.h �� */

	// �����ݿ���������������Blob�ֶ�������ӳ�䣨������ȡ���ݿ⣩
	Form2Field_HashMap mForm2FieldHashMap;

	// �Ժ����Ƿ�� CreateForm2FieldMapping()��CreateField2PBMapping() ������Ϣ���ϵ������ļ�
	// ���߰���Ķ���̫����......
	// ��������������Blob�ֶ������ϡ���ӳ��
	void CreateForm2FieldMapping();


private:
	// ���ݿ������Ƿ�ɹ�
	bool mbMySql;
	// ���ݿ����Ӷ���
	CDatabaseMysql mMysql;


	// �����ݿ����ֶΡ���������emPB ö�١���ӳ�䣨��ȡ���ݿ⣩
	String2emPB_HashMap mField2emPBMap;
	// ��PB ����������Ӧ�� emPB ö�١���ӳ�䣨д�����ݿ⣩
	String2emPB_HashMap mPBName2PBMap;
};

#endif
