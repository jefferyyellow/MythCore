// ***************************************************************
//  ProtoXMLManager   version:  1.0   ? date: 10/29/2014
//  -------------------------------------------------------------
//  Protobuf 和 XML 映射关系的管理类，
//  这里应该只关心 Protobuf 和 XML 层次对应关系，不干其它事
//  -------------------------------------------------------------
//  Copyright (C) 2014 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************


#ifndef _PROTO_XML_MANAGER_H_
#define _PROTO_XML_MANAGER_H_

#include "servertool.h"
#include "base.h"

// 要用的 TinyXML 组件
#include "tinyxml.h"
#include "tinystr.h"

// 要用的 PB 模块组件
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

// 一个字节共多少位
#define BYTE_NUM 8
// CBitData 最多存储多少位，目前游戏里最大的是功能开启 MAX_MAIN_FUNCTION_UNLOCK_NUM
// 注意这个数必须是 8 的倍数，否则解析含有 byte 字段的 PB 时会解析失败
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
	// 解析PB
	int Message2Xml(const Message& rMessage, TiXmlElement* tpParent );
	// 解析字段
	int Field2Xml(const Message& rMessage, const Reflection* pReflection, const FieldDescriptor* pFieldDescriptor, int nIndex, TiXmlElement* tpParent );

	// 在 google::protobuf 中，byte 和 string 都以字符串方式存储，但存储方式不同
	// 若某个字段的 cpptype 为 FieldDescriptor::CPPTYPE_STRING，且 protobuf 对应类型为 FieldDescriptor::TYPE_BYTES
	// 将其转换以 byte 对应的字符串类型解析
	std::string ParseByte2Xml( const std::string &StrValue );
	std::string WriteXml2Byte( const std::string &StrValue );

public:
	// 组装 pMessage（PB） 的某个字段
	int Xml2Message( Message* pMessage, const Reflection* pReflection ,TiXmlElement* tpParent, int nDepth = 0 );
	// 把某个字段组装到它的父 PB 上
	int Xml2Field( Message* pMessage, const Reflection* pReflection, const FieldDescriptor* pFieldDescriptor, TiXmlElement* tpParent, int nDepth = 0 );

public:
	// 为调试方便，打印读取的每一个Xml结点
	void PrintXmlNodeBegin( TiXmlElement* tpXmlElement, int nDepth );
	void PrintXmlNodeEnd( TiXmlElement* tpXmlElement, int nDepth );
};

#endif // _PROTO_XML_MANAGER_H_