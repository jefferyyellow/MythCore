// ***************************************************************
//  ProtoXMLManager   version:  1.0   ? date: 10/29/2014
//  -------------------------------------------------------------
//  Protobuf �� XML ӳ���ϵ�Ĺ����࣬
//  ����Ӧ��ֻ���� Protobuf �� XML ��ζ�Ӧ��ϵ������������
//  -------------------------------------------------------------
//  Copyright (C) 2014 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************


#ifndef _PROTO_XML_MANAGER_H_
#define _PROTO_XML_MANAGER_H_

#include "servertool.h"
#include "base.h"

// Ҫ�õ� TinyXML ���
#include "tinyxml.h"
#include "tinystr.h"

// Ҫ�õ� PB ģ�����
#include <google/protobuf/message.h>
#include "google/protobuf/descriptor.h"
#include "google/protobuf/descriptor.pb.h"

#include "google/protobuf/compiler/importer.h"
#include "google/protobuf/dynamic_message.h"

typedef ::google::protobuf::Message Message;
typedef ::google::protobuf::Descriptor Descriptor;
typedef ::google::protobuf::Reflection Reflection;
typedef ::google::protobuf::FieldDescriptor FieldDescriptor;
typedef ::google::protobuf::EnumValueDescriptor EnumValueDescriptor;

// һ���ֽڹ�����λ
#define BYTE_NUM 8
// CBitData ���洢����λ��Ŀǰ��Ϸ�������ǹ��ܿ��� MAX_MAIN_FUNCTION_UNLOCK_NUM
// ע������������� 8 �ı���������������� byte �ֶε� PB ʱ�����ʧ��
#define MAX_BIT_DATA_SIZE 200	

//! CProtoXMLManager
/*!
 Function : Parse Protobuf object to XML File, and Parse XML File to Protobuf object
*/
class CProtoXMLManager : public CSingleton<CProtoXMLManager>
{
public:
	CProtoXMLManager(void);
	~CProtoXMLManager(void);

public:
	// ����PB
	int Message2Xml(const Message& rMessage, TiXmlElement* tpParent );
	// �����ֶ�
	int Field2Xml(const Message& rMessage, const Reflection* pReflection, const FieldDescriptor* pFieldDescriptor, int nIndex, TiXmlElement* tpParent );

	// �� google::protobuf �У�byte �� string �����ַ�����ʽ�洢�����洢��ʽ��ͬ
	// ��ĳ���ֶε� cpptype Ϊ FieldDescriptor::CPPTYPE_STRING���� protobuf ��Ӧ����Ϊ FieldDescriptor::TYPE_BYTES
	// ����ת���� byte ��Ӧ���ַ������ͽ���
	std::string ParseByte2Xml( const std::string &StrValue );
	std::string WriteXml2Byte( const std::string &StrValue );

public:
	// ��װ pMessage��PB�� ��ĳ���ֶ�
	int Xml2Message( Message* pMessage, const Reflection* pReflection ,TiXmlElement* tpParent, int nDepth = 0 );
	// ��ĳ���ֶ���װ�����ĸ� PB ��
	int Xml2Field( Message* pMessage, const Reflection* pReflection, const FieldDescriptor* pFieldDescriptor, TiXmlElement* tpParent, int nDepth = 0 );

public:
	// Ϊ���Է��㣬��ӡ��ȡ��ÿһ��Xml���
	void PrintXmlNodeBegin( TiXmlElement* tpXmlElement, int nDepth );
	void PrintXmlNodeEnd( TiXmlElement* tpXmlElement, int nDepth );
};

#endif // _PROTO_XML_MANAGER_H_