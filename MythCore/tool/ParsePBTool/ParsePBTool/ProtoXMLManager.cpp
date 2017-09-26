#include "ProtoXMLManager.h"
#include <bitset>
#include <iostream>

CProtoXMLManager::CProtoXMLManager(void)
{
}

CProtoXMLManager::~CProtoXMLManager(void)
{
}

int CProtoXMLManager::Field2Xml(const Message& rMessage, const Reflection* pReflection, const FieldDescriptor* pFieldDescriptor, int nIndex, TiXmlElement* tpParent )
{
	FieldDescriptor::CppType emCppType = pFieldDescriptor->cpp_type();
	switch( emCppType )
	{
	case FieldDescriptor::CPPTYPE_BOOL:
		{
			bool bRepeated = pFieldDescriptor->is_repeated();
			if ( bRepeated )
			{
				//char tNodeName[MAX_INDEX_DIGIT] = { 0 };
				//snprintf( tNodeName, sizeof(tNodeName), "%d", nIndex );
				TiXmlElement* tpXmlElement = new TiXmlElement("Index");
				tpXmlElement->SetAttribute("Id", nIndex);
				int nValue = pReflection->GetRepeatedBool(rMessage, pFieldDescriptor, nIndex);
				tpXmlElement->SetAttribute( "Type", "bool" );
				tpXmlElement->SetAttribute("Value", nValue );

				tpParent->LinkEndChild( tpXmlElement );
			}
			else
			{
				TiXmlElement* tpXmlElement = new TiXmlElement("Optional");
				tpXmlElement->SetAttribute( "Type", "bool" );
				tpXmlElement->SetAttribute( "Field", pFieldDescriptor->name().c_str() );
				int nValue = pReflection->GetBool(rMessage, pFieldDescriptor);
				tpXmlElement->SetAttribute( "Value", (int)nValue );

				tpParent->LinkEndChild( tpXmlElement );
			}
			break;
		}
	case FieldDescriptor::CPPTYPE_INT32:
		{
			bool bRepeated = pFieldDescriptor->is_repeated();
			if ( bRepeated )
			{
				//char tNodeName[MAX_INDEX_DIGIT] = { 0 };
				//snprintf( tNodeName, sizeof(tNodeName), "%d", nIndex );
				TiXmlElement* tpXmlElement = new TiXmlElement("Index");
				tpXmlElement->SetAttribute("Id", nIndex);
				int nValue = pReflection->GetRepeatedInt32(rMessage, pFieldDescriptor, nIndex);
				tpXmlElement->SetAttribute( "Type", "int32" );
				tpXmlElement->SetAttribute("Value", nValue );

				tpParent->LinkEndChild( tpXmlElement );
			}
			else
			{
				TiXmlElement* tpXmlElement = new TiXmlElement("Optional");
				tpXmlElement->SetAttribute( "Type", "int32" );
				tpXmlElement->SetAttribute( "Field", pFieldDescriptor->name().c_str() );
				int nValue = pReflection->GetInt32(rMessage, pFieldDescriptor);
				tpXmlElement->SetAttribute( "Value", (int)nValue );

				tpParent->LinkEndChild( tpXmlElement );
			}
			break;
		}
	case FieldDescriptor::CPPTYPE_INT64:
		{
			bool bRepeated = pFieldDescriptor->is_repeated();
			if ( bRepeated )
			{
				//char tNodeName[MAX_INDEX_DIGIT] = { 0 };
				//snprintf( tNodeName, sizeof(tNodeName), "%d", nIndex );
				TiXmlElement* tpXmlElement = new TiXmlElement("Index");
				tpXmlElement->SetAttribute("Id", nIndex);
				long long int lValue = pReflection->GetRepeatedInt64(rMessage, pFieldDescriptor, nIndex);
				char tLValueArr[20] = { 0 };
				snprintf( tLValueArr, sizeof( tLValueArr ), "%lld", lValue );
				tpXmlElement->SetAttribute( "Type", "int64" );
				tpXmlElement->SetAttribute( "Value", tLValueArr );

				tpParent->LinkEndChild( tpXmlElement );
			}
			else
			{
				TiXmlElement* tpXmlElement = new TiXmlElement("Optional");
				tpXmlElement->SetAttribute( "Type", "int64" );
				tpXmlElement->SetAttribute( "Field", pFieldDescriptor->name().c_str() );
				long long int lValue = pReflection->GetInt64(rMessage, pFieldDescriptor);
				char tLValueArr[20] = { 0 };
				snprintf( tLValueArr, sizeof(tLValueArr), "%lld", lValue );
				tpXmlElement->SetAttribute( "Value", tLValueArr );

				tpParent->LinkEndChild( tpXmlElement );
			}
			break;
		}
	case FieldDescriptor::CPPTYPE_UINT32:
		{
			bool bRepeated = pFieldDescriptor->is_repeated();
			if ( bRepeated )
			{
				//char tNodeName[MAX_INDEX_DIGIT] = { 0 };
				//snprintf( tNodeName, sizeof(tNodeName), "%d", nIndex );
				TiXmlElement* tpXmlElement = new TiXmlElement("Index");
				tpXmlElement->SetAttribute("Id", nIndex);
				tpXmlElement->SetAttribute( "Type", "uint32" );
				unsigned int nValue = pReflection->GetRepeatedUInt32(rMessage, pFieldDescriptor, nIndex);
				tpXmlElement->SetAttribute("Value", nValue );

				tpParent->LinkEndChild( tpXmlElement );
			}
			else
			{
				TiXmlElement* tpXmlElement = new TiXmlElement("Optional");
				tpXmlElement->SetAttribute( "Type", "uint32" );
				tpXmlElement->SetAttribute( "Field", pFieldDescriptor->name().c_str() );
				unsigned int nValue = pReflection->GetUInt32(rMessage, pFieldDescriptor);
				tpXmlElement->SetAttribute( "Value", (int)nValue );

				tpParent->LinkEndChild( tpXmlElement );
			}
			break;
		}
	case FieldDescriptor::CPPTYPE_UINT64:
		{
			bool bRepeated = pFieldDescriptor->is_repeated();
			if ( bRepeated )
			{
				//char tNodeName[MAX_INDEX_DIGIT] = { 0 };
				//snprintf( tNodeName, sizeof(tNodeName), "%d", nIndex );
				TiXmlElement* tpXmlElement = new TiXmlElement("Index");
				tpXmlElement->SetAttribute("Id", nIndex);
				unsigned long long lValue = pReflection->GetRepeatedUInt64(rMessage, pFieldDescriptor, nIndex);
				char tLValueArr[20] = { 0 };
				snprintf( tLValueArr, sizeof(tLValueArr), "%llu", lValue );// 注意！"%llu"只在32位机器上有效，在64位机器上会报警告！
				tpXmlElement->SetAttribute( "Type", "uint64" );
				tpXmlElement->SetAttribute( "Value", tLValueArr );

				tpParent->LinkEndChild( tpXmlElement );
			}
			else
			{
				TiXmlElement* tpXmlElement = new TiXmlElement("Optional");
				tpXmlElement->SetAttribute( "Type", "uint64" );
				tpXmlElement->SetAttribute( "Field", pFieldDescriptor->name().c_str() );
				unsigned long long lValue = pReflection->GetUInt64(rMessage, pFieldDescriptor);
				char tLValueArr[20] = { 0 };
				snprintf( tLValueArr, sizeof(tLValueArr), "%llu", lValue );// 注意！"%llu"只在32位机器上有效，在64位机器上会报警告！
				tpXmlElement->SetAttribute( "Value", tLValueArr );

				tpParent->LinkEndChild( tpXmlElement );
			}
			break;
		}
	case FieldDescriptor::CPPTYPE_STRING:// 注意：CPPTYPE_STRING 包含 TYPE_STRING, TYPE_BYTES 两种类型
		{
			bool bRepeated = pFieldDescriptor->is_repeated();
			if ( bRepeated )
			{
				//char tNodeName[MAX_INDEX_DIGIT] = { 0 };
				//snprintf( tNodeName, sizeof(tNodeName), "%d", nIndex );
				TiXmlElement* tpXmlElement = new TiXmlElement("Index");
				tpXmlElement->SetAttribute("Id", nIndex);
				std::string StrValue = pReflection->GetRepeatedString(rMessage, pFieldDescriptor, nIndex);

				switch( pFieldDescriptor->type() )
				{
				case FieldDescriptor::TYPE_BYTES:
					{
						tpXmlElement->SetAttribute( "Type", "bytes" );
						tpXmlElement->SetAttribute( "Field", pFieldDescriptor->name().c_str() );
						
						std::string tpByteStr = ParseByte2Xml(StrValue);
						if ( tpByteStr.empty() )
						{
							printf("Parse Byte2Xml Failed ! The Field Name : %s \n", pFieldDescriptor->name().c_str());
							return -1;
						}
						tpXmlElement->SetAttribute( "Value", tpByteStr.c_str() );
						break;
					}
				case FieldDescriptor::TYPE_STRING:
					{
						tpXmlElement->SetAttribute( "Type", "string" );
						tpXmlElement->SetAttribute("Value", StrValue.c_str() );
						break;
					}
				default:
					break;
				}//end switch( pFieldDescriptor->enum_type() )
				tpParent->LinkEndChild( tpXmlElement );
			}
			else
			{
				TiXmlElement* tpXmlElement = new TiXmlElement("Optional");
				std::string StrValue = pReflection->GetString(rMessage, pFieldDescriptor);

				switch( pFieldDescriptor->type() )
				{
				case FieldDescriptor::TYPE_BYTES:
					{
						tpXmlElement->SetAttribute( "Type", "bytes" );
						tpXmlElement->SetAttribute( "Field", pFieldDescriptor->name().c_str() );

						std::string tpByteStr = ParseByte2Xml(StrValue);
						if ( tpByteStr.empty() )
						{
							printf("Parse Byte2Xml Failed ! The Field Name : %s \n", pFieldDescriptor->name().c_str());
							return -1;
						}
						tpXmlElement->SetAttribute( "Value", tpByteStr.c_str() );
						break;
					}
				case FieldDescriptor::TYPE_STRING:
					{
						tpXmlElement->SetAttribute( "Type", "string" );
						tpXmlElement->SetAttribute( "Field", pFieldDescriptor->name().c_str() );
						tpXmlElement->SetAttribute("Value", StrValue.c_str() );
						break;
					}
				default:
					break;
				}//end switch( pFieldDescriptor->type() )
				tpParent->LinkEndChild( tpXmlElement );
			}
			break;
		}
	case FieldDescriptor::CPPTYPE_MESSAGE:
		{
			bool bRepeated = pFieldDescriptor->is_repeated();
			if ( bRepeated )
			{
				const Message& rSubMessage = pReflection->GetRepeatedMessage(rMessage, pFieldDescriptor, nIndex);
				const Descriptor* pFieldTypeDescriptor = pFieldDescriptor->message_type();

				//char tNodeName[MAX_INDEX_DIGIT] = { 0 };
				//snprintf( tNodeName, sizeof(tNodeName), "%d", nIndex );
				TiXmlElement* tpXmlElement = new TiXmlElement("Index");
				tpXmlElement->SetAttribute("Id", nIndex);
				tpXmlElement->SetAttribute( "Type", "Message" );
				tpXmlElement->SetAttribute( "Class", pFieldTypeDescriptor->name().c_str() );// PB 名称
				//tpXmlElement->SetAttribute( "Field", pFieldDescriptor->name().c_str() );// 字段名

				tpParent->LinkEndChild( tpXmlElement );
				Message2Xml(rSubMessage, tpXmlElement );
			}
			else
			{
				const Message& rSubMessage = pReflection->GetMessage(rMessage, pFieldDescriptor);
				const Descriptor* pFieldTypeDescriptor = pFieldDescriptor->message_type();

				TiXmlElement* tpXmlElement = new TiXmlElement("Optional");
				tpXmlElement->SetAttribute( "Type", "Message" );
				tpXmlElement->SetAttribute( "Class", pFieldTypeDescriptor->name().c_str() );// PB 名称
				tpXmlElement->SetAttribute( "Field", pFieldDescriptor->name().c_str() );// 字段名

				tpParent->LinkEndChild( tpXmlElement );
				Message2Xml(rSubMessage, tpXmlElement );
			}
			break;
		}
	default:
		{
			const char* tpUnParsedFieldName = pFieldDescriptor->name().c_str();
			const Descriptor* pFieldTypeDescriptor = pFieldDescriptor->message_type();
			if ( tpUnParsedFieldName != NULL && pFieldTypeDescriptor != NULL )
			{
				const char* tpUnParsedMsgTypeName = pFieldTypeDescriptor->name().c_str();
				// 打印其它尚未进行解析处理的 cpp_type 类型
				printf( "There is an UnParsed CppType ( Enum ) in FieldDescriptor::CppType : %d . ", (int)emCppType );
				printf( "The UnParsed Field Name is : %s, it's CppType is : %s\n", tpUnParsedFieldName, tpUnParsedMsgTypeName );
			}
			break;
		}	
	}//end switch ( pFieldDescriptor->cpp_type() )
	return 0;
}

int CProtoXMLManager::Message2Xml(const Message& rMessage, TiXmlElement* tpParent )
{
	if ( tpParent == NULL )
	{
		return 0;
	}

	const Reflection* pReflection = rMessage.GetReflection();
	const Descriptor* pDescriptor = rMessage.GetDescriptor();
	for(int i = 0; i < pDescriptor->field_count(); ++ i)
	{
		const FieldDescriptor* pFieldDescriptor = pDescriptor->field(i);
		if( pFieldDescriptor->is_repeated() )
		{
			const Descriptor* pFieldTypeDescriptor = pFieldDescriptor->message_type();

			TiXmlElement* tpXmlElement = new TiXmlElement("Repeated");
			switch( pFieldDescriptor->cpp_type() )
			{
			case FieldDescriptor::CPPTYPE_INT32:
				{
					tpXmlElement->SetAttribute( "Type", "int32" );
					break;
				}
			case FieldDescriptor::CPPTYPE_INT64:
				{
					tpXmlElement->SetAttribute( "Type", "int64" );
					break;
				}
			case FieldDescriptor::CPPTYPE_UINT32:
				{
					tpXmlElement->SetAttribute( "Type", "uint32" );
					break;
				}
			case FieldDescriptor::CPPTYPE_MESSAGE:
				{
					//tpXmlElement->SetAttribute( "Type", pFieldTypeDescriptor->name().c_str() );
					tpXmlElement->SetAttribute( "Type", "Message" );
					tpXmlElement->SetAttribute( "Class", pFieldTypeDescriptor->name().c_str() );
					break;
				}
			default:
				break;
			}//end switch( pFieldDescriptor->cpp_type() )
			tpXmlElement->SetAttribute("Field", pFieldDescriptor->name().c_str() );
			tpParent->LinkEndChild( tpXmlElement );

			int nCount = pReflection->FieldSize(rMessage, pFieldDescriptor);// ★ FieldSize 参数1只接受具体的PB
			tpXmlElement->SetAttribute( "Size", nCount );
			for(int iRepeated = 0; iRepeated < nCount; ++ iRepeated)
			{
				int tRet = Field2Xml( rMessage, pReflection, pFieldDescriptor, iRepeated, tpXmlElement ); 
				if ( tRet != 0 )
				{
					printf("Field2Xml Failed !\n");
					return -1;
				}
			}
		}//end if( pFieldDescriptor->is_repeated() )
		else
		{
			int tRet = Field2Xml( rMessage, pReflection, pFieldDescriptor, -1, tpParent );
			if ( tRet != 0 )
			{
				printf("Field2Xml Failed !\n");
				return -1;
			}
		} 
	}
	return 0;
}

//************************************
// Method:    Xml2Message
// Returns:    int
// Parameter: pMessage —— 要组装的 PB
// Parameter: pReflection —— 要组装的 PB 的反射
// Parameter: tpRoot —— 要组装的 PB 字段对应 xml 结点
// Author	: Sunrise Meier
// Date		: 5/5/2014 15:37
// Comment	: 组装某个 pMessage（PB） 
//************************************
int CProtoXMLManager::Xml2Message( Message* pMessage, const Reflection* pReflection ,TiXmlElement* tpParent, int nDepth )
{
	if ( pMessage == NULL || tpParent == NULL || pReflection == NULL )
	{
		return -1;
	}

	const Descriptor* tpDescriptor = pMessage->GetDescriptor();
	if ( tpDescriptor == NULL )
	{
		return -1;
	}

	TiXmlElement* tpChildFieldElem = tpParent->FirstChildElement();
	// 组装每一个字段到 pMessage 上
	for(int iField = 0; iField < tpDescriptor->field_count(); ++iField)
	{
		const FieldDescriptor* tpFieldDescriptor = tpDescriptor->field(iField);// 每一条字段的描述符
		if ( tpFieldDescriptor == NULL || tpChildFieldElem == NULL )
		{
			continue;
		}

		// PrintXmlNodeBegin( tpChildFieldElem, nDepth + 1 );

		if ( tpFieldDescriptor->is_repeated() )
		{
			// 确定Repeated 字段的 Size，暂时用 xml 结点记录的
			const char* tpRepeatedSize = tpChildFieldElem->Attribute("Size");
			if ( tpRepeatedSize == NULL )
			{
				continue;
			}
			int nCount = atoi(tpRepeatedSize);
			// 测试临时修改：只输出一条 Repeated 字段的信息
			/*if ( nCount > 0 )
			{
				nCount = 1;
			}*/
			TiXmlElement* tpRepeatedElem = tpChildFieldElem->FirstChildElement();
			for(int iRepeated = 0; iRepeated < nCount; ++iRepeated)
			{
				if ( tpRepeatedElem == NULL )
				{
					continue;
				}
				
				// PrintXmlNodeBegin( tpRepeatedElem, nDepth + 2 );
				
				int tRet = Xml2Field( pMessage, pReflection, tpFieldDescriptor, tpRepeatedElem, nDepth + 2 );
				if ( tRet != 0 )
				{
					printf("Xml2Field Failed !\n");
					return -1;
				}
				//PrintXmlNodeEnd( tpRepeatedElem, nDepth + 2 );
				tpRepeatedElem = tpRepeatedElem->NextSiblingElement();
			}
		}//end if ( tpFieldDescriptor->is_repeated() )
		else
		{
			int tRet = Xml2Field( pMessage, pReflection, tpFieldDescriptor, tpChildFieldElem, nDepth + 1 );
			if ( tRet != 0 )
			{
				printf("Xml2Field Failed !\n");
				return -1;
			}
		}

		tpChildFieldElem = tpChildFieldElem->NextSiblingElement();
		//PrintXmlNodeEnd( tpChildFieldElem, nDepth + 1 );
	}//end for (int iField = 0; iField < tFieldVec.size(); ++iField)

	return 0;
}

//************************************
// Method:    Xml2Field
// Returns:    int
// Parameter: pMessage —— 父PB
// Parameter: pReflection —— 父 PB 反射
// Parameter: pFieldDescriptor —— 父 PB 的某个字段描述符
// Parameter: tpFieldNode —— 父 PB 的 字段的 xml 结点
// Author	: Sunrise Meier
// Date		: 5/5/2014 16:56
// Comment	: 把某个字段组装到它的父 PB 上
//************************************
int CProtoXMLManager::Xml2Field( Message* pMessage, const Reflection* pReflection, const FieldDescriptor* pFieldDescriptor, TiXmlElement* tpFieldNode, int nDepth )
{
	if ( pMessage == NULL || pReflection == NULL || pFieldDescriptor == NULL || tpFieldNode == NULL )
	{
		return -1;
	}

	// 这里应该为xml结点对应的字段名，和当前字段描述符的字段名做比较，如果不一致则报错
	const char* tpXmlFieldName = tpFieldNode->Attribute( "Field" );
	const char* tpFieldDescriptorName = pFieldDescriptor->name().c_str();
	if ( tpXmlFieldName != NULL && tpFieldDescriptorName != NULL )
	{
		int tCmp = strcmp( tpXmlFieldName, tpFieldDescriptorName );
		if ( tCmp != 0 )
		{
			printf("The Descriptor Field Name : 【%s】 is different to it's Xml Node : 【%s】 .\n", tpFieldDescriptorName, tpXmlFieldName);
			printf("Please check your Xml file, is missing parse some PB field in Xml Node ?\n");
			return -1;
		}
	}

	switch( pFieldDescriptor->cpp_type() )
	{
	case FieldDescriptor::CPPTYPE_BOOL:
		{
			bool tValue = 0;
			const char* tpValue = tpFieldNode->Attribute("Value");// 每个 repeated PB 结点
			if ( tpValue != NULL )
			{
				tValue = atoi(tpValue);
			}

			bool isRepeated = pFieldDescriptor->is_repeated();
			if ( isRepeated )
			{
				pReflection->AddBool( pMessage, pFieldDescriptor, tValue );
			}
			else
			{
				pReflection->SetBool( pMessage, pFieldDescriptor, tValue );
			}
			break;
		}
	case FieldDescriptor::CPPTYPE_INT32:
		{
			int tValue = 0;
			const char* tpValue = tpFieldNode->Attribute("Value");// 每个 repeated PB 结点
			if ( tpValue != NULL )
			{
				tValue = atoi(tpValue);
			}

			bool isRepeated = pFieldDescriptor->is_repeated();
			if ( isRepeated )
			{
				pReflection->AddInt32( pMessage, pFieldDescriptor, tValue );
			}
			else
			{
				pReflection->SetInt32( pMessage, pFieldDescriptor, tValue );
			}
			break;
		}
	case FieldDescriptor::CPPTYPE_UINT32 :
		{
			int tValue = 0;
			const char* tpValue = tpFieldNode->Attribute("Value");// 每个 repeated PB 结点
			if ( tpValue != NULL )
			{
				tValue = atoi(tpValue);
			}

			bool isRepeated = pFieldDescriptor->is_repeated();
			if ( isRepeated )
			{
				pReflection->AddUInt32( pMessage, pFieldDescriptor, tValue );
			}
			else
			{
				pReflection->SetUInt32( pMessage, pFieldDescriptor, tValue );
			}
			break;
		}
	case FieldDescriptor::CPPTYPE_INT64:
		{
			long long tValue = 0;
			const char* tpValue = tpFieldNode->Attribute("Value");// 每个 repeated PB 结点
			if ( tpValue != NULL )
			{
				tValue = atoll(tpValue);
			}

			bool isRepeated = pFieldDescriptor->is_repeated();
			if ( isRepeated )
			{
				pReflection->AddInt64( pMessage, pFieldDescriptor, tValue );
			}
			else
			{
				pReflection->SetInt64( pMessage, pFieldDescriptor, tValue );
			}
			break;
		}
	case FieldDescriptor::CPPTYPE_UINT64 :
		{
			long long tValue = 0;
			const char* tpValue = tpFieldNode->Attribute("Value");// 每个 repeated PB 结点
			if ( tpValue != NULL )
			{
				tValue = atoll(tpValue);
			}

			bool isRepeated = pFieldDescriptor->is_repeated();
			if ( isRepeated )
			{
				pReflection->AddUInt64( pMessage, pFieldDescriptor, tValue );
			}
			else
			{
				pReflection->SetUInt64( pMessage, pFieldDescriptor, tValue );
			}
			break;
		}
	case FieldDescriptor::CPPTYPE_MESSAGE :
		{
			Message* tpChildMsg = NULL;
			bool isRepeated = pFieldDescriptor->is_repeated();
			if ( isRepeated )
			{
				tpChildMsg = pReflection->AddMessage( pMessage, pFieldDescriptor );
			}
			else
			{
				tpChildMsg = pReflection->MutableMessage( pMessage, pFieldDescriptor );
			}

			if ( tpChildMsg != NULL )
			{
				const Reflection* tpChildReflection = tpChildMsg->GetReflection();
				if ( tpChildReflection != NULL )
				{	
					Xml2Message( tpChildMsg, tpChildReflection, tpFieldNode, nDepth + 1 );
				}
			}//end if ( tpChildMsg != NULL )
			break;
		}
	case FieldDescriptor::CPPTYPE_STRING:
		{
			switch( pFieldDescriptor->type() )
			{
			case FieldDescriptor::TYPE_BYTES:
				{
					std::string tValue("");
					const char* tpValue = tpFieldNode->Attribute("Value");// 每个 repeated PB 结点
					if ( tpValue != NULL )
					{
						tValue.append( tpValue );
					}

					std::string tConvertStr = WriteXml2Byte( tValue );
					if ( tConvertStr.empty() )
					{
						printf("Parse Byte2Xml Failed ! The Field Name : %s \n", pFieldDescriptor->name().c_str());
						return -1;
					}

					bool isRepeated = pFieldDescriptor->is_repeated();
					if ( isRepeated )
					{
						pReflection->AddString( pMessage, pFieldDescriptor, tConvertStr );
					}
					else
					{
						pReflection->SetString( pMessage, pFieldDescriptor, tConvertStr );
					}
					break;
				}
			case FieldDescriptor::TYPE_STRING:
				{
					std::string tValue("");
					const char* tpValue = tpFieldNode->Attribute("Value");// 每个 repeated PB 结点
					if ( tpValue != NULL )
					{
						tValue.append( tpValue );
					}

					bool isRepeated = pFieldDescriptor->is_repeated();
					if ( isRepeated )
					{
						pReflection->AddString( pMessage, pFieldDescriptor, tValue );
					}
					else
					{
						pReflection->SetString( pMessage, pFieldDescriptor, tValue );
					}
					break;
				}
			default:
			    break;
			}
		}
	default:
		printf("%s %d this type don't parse,please improve it,thanks!", tpFieldDescriptorName, pFieldDescriptor->cpp_type());
		break;
	}//end switch

	return 0;
}

// 为调试方便，打印读取的每一个Xml结点
void CProtoXMLManager::PrintXmlNodeBegin( TiXmlElement* tpXmlElement, int nDepth )
{
	if ( tpXmlElement == NULL || nDepth < 0 )
	{
		return ;
	}

	const char* tpXmlNodeName = tpXmlElement->Value();
	if ( tpXmlNodeName == NULL )
	{
		return ;
	}

	TiXmlAttribute* tpFirstTiXmlAttribute = tpXmlElement->FirstAttribute();
	if ( tpFirstTiXmlAttribute == NULL )
	{
		return ;
	}

	for (int i = 0; i < nDepth; ++i)
	{
		printf(" ");
	}

	printf( "<%s", tpXmlNodeName );
	// printf("");
	for ( ;tpFirstTiXmlAttribute != NULL; tpFirstTiXmlAttribute = tpFirstTiXmlAttribute->Next() )
	{
		const char* tpAttributeName = tpFirstTiXmlAttribute->Name();
		const char* tpAttributeVal = tpFirstTiXmlAttribute->Value();
		if ( tpAttributeVal == NULL || tpAttributeName == NULL )
		{
			continue;
		}
		printf( " %s=\"%s\"", tpAttributeName, tpAttributeVal );
	}//end for
	printf(">\n");
}

// 为调试方便，打印读取的每一个Xml结点
void CProtoXMLManager::PrintXmlNodeEnd( TiXmlElement* tpXmlElement, int nDepth )
{
	if ( tpXmlElement == NULL || nDepth < 0 )
	{
		return ;
	}

	const char* tpXmlNodeName = tpXmlElement->Value();
	if ( tpXmlNodeName == NULL )
	{
		return ;
	}

	TiXmlAttribute* tpFirstTiXmlAttribute = tpXmlElement->FirstAttribute();
	if ( tpFirstTiXmlAttribute == NULL )
	{
		return ;
	}

	printf("\n");
	for (int i = 0; i < nDepth; ++i)
	{
		printf(" ");
	}
	printf( "</%s>", tpXmlNodeName );
}

// 把内存中的 byte 转成 xml 要用的字符串
std::string CProtoXMLManager::ParseByte2Xml( const std::string &StrValue )
{
	const int tMemBitArrCapacity = MAX_BIT_DATA_SIZE / BYTE_NUM + 1;
	unsigned char tMemBitArr[tMemBitArrCapacity] = { 0 };
	
	// 用来存储位的 unsigned char 数组定长过小
	if ( tMemBitArrCapacity < StrValue.length() )
	{
		printf("MAX_BIT_DATA_SIZE length not enough !\n");
		return std::string("");
	}
	// 定义的最大位数 MAX_BIT_DATA_SIZE，必须是 8 的倍数
	if ( MAX_BIT_DATA_SIZE % BYTE_NUM != 0 )
	{
		printf("MAX_BIT_DATA_SIZE is not divisible by BYTE_NUM !\n");
		return std::string("");
	}
	memcpy( tMemBitArr, StrValue.c_str(), StrValue.length() );// [ 0, StrValue.length() ) 范围内的才有效
	
	// 准备用来显示每一位的字符串，一个字节存储一位
	char tBitShowStr[MAX_BIT_DATA_SIZE + 1] = { 0 };
	int tBitShowStrCapacity = sizeof(tBitShowStr)/sizeof(char);
	int tBitShowStrSize = 0;
	
	char* tItByteString = tBitShowStr;
	char* tEndByteString = tBitShowStr + tBitShowStrCapacity;
	
	// 把 tMaxBitArr 中的每个成员的每一个数据位转换为一个对应的字符，存储在一个数组里
	for (int i = 0; i < StrValue.length() && i < tBitShowStrCapacity; ++i)
	{
		// 解析每一个字节的每一个位。注意：最低位在调试打印显示出来的最右
		// 故解析每一个字节的时候，字符串应该反着存。
		// 如 x/1tb 0xbff95cdb，打印该字节位 00001000，则应该从最后边的位开始 push_back
		// 因为 string 顺序和位顺序是相反的
		char tByteArr[BYTE_NUM] = { 0 };
		int tEndIndex = BYTE_NUM - 1;
		for (int iBit = 0; iBit < BYTE_NUM && tEndIndex >= 0; ++iBit)
		{
			unsigned char tModData = ( (1<<1) - 1 ) << iBit;
			bool tFinal = ( tMemBitArr[i] & tModData ) >> iBit;
			if ( tFinal )
			{
				tByteArr[tEndIndex--] = '1';
			}
			else
			{
				tByteArr[tEndIndex--] = '0';
			}				
		}//end for (int iBit = 0; iBit < 8; ++iBit)

		// 存入一个字节的所有位
		if ( tItByteString != tEndByteString )
		{
			memcpy( tItByteString, tByteArr, BYTE_NUM );
			tItByteString += BYTE_NUM;
			tBitShowStrSize += BYTE_NUM;
		}
	}

	tBitShowStr[tBitShowStrSize] = '\0';
	std::string tConvertStr( tBitShowStr );
	return tConvertStr;
}

// 把字符形式的位转换成内存中真正的位
std::string CProtoXMLManager::WriteXml2Byte( const std::string &StrValue )
{
	// xml 中显示用的位串，每个字节表示一位
	int tByteNum = StrValue.length();

	// 用来存储位的 unsigned char 数组定长过小
	if ( MAX_BIT_DATA_SIZE < tByteNum )
	{
		printf("MAX_BIT_DATA_SIZE length not enough !\n");
		return std::string("");
	}
	// 定义的最大位数 MAX_BIT_DATA_SIZE，必须是 8 的倍数
	if ( MAX_BIT_DATA_SIZE % BYTE_NUM != 0 )
	{
		printf("MAX_BIT_DATA_SIZE is not divisible by BYTE_NUM !\n");
		return std::string("");
	}

	// 把01形式字符串转成位
	std::string tReverseStr( StrValue.rbegin(), StrValue.rend() );
	std::bitset<MAX_BIT_DATA_SIZE> tBitSet( tReverseStr );
	
	/*std::cout<<tBitSet<<std::endl;
	for (int iBit = 0; iBit < tBitSet.size() && iBit < tByteNum; ++iBit)
	{
		int tBitNum = tBitSet[iBit];
		printf( "%d", tBitNum );
		if ( iBit > 1 && (iBit+1) % 8 == 0 )
		{
			printf(" ");
		}
	}
	printf("\n");*/

	
	// 用来存储在内存的字符串，需要额外多留一个字节存 \0
	const int tMemBitCapacity = MAX_BIT_DATA_SIZE / BYTE_NUM;
	char tMemBitArr[tMemBitCapacity] = { 0 };
	int tByteSize = 0;
	
	int tBitCur = 0;
	for (int i = 0; i < tMemBitCapacity && tBitCur < tBitSet.size() && tBitCur < tByteNum; ++i)
	{
		for (int iBit = tBitCur, tBitIndex = BYTE_NUM - 1; iBit < tBitCur + BYTE_NUM && tBitIndex >= 0; ++iBit, --tBitIndex)
		{
			if ( tBitSet.test(iBit) )
			{
				tMemBitArr[i] |= ( 1 << tBitIndex );
			}
		}

		tBitCur += BYTE_NUM;
		tByteSize += 1;
	}

	//tMemBitArr[tByteSize++] = '\0';

	// tMemBitArr 串中可能会包含\0，所以这里只能一个字符一个字符的 push 进去
	std::string tResultStr("");
	for (int i = 0; i < tByteSize && i < tMemBitCapacity; ++i)
	{
		tResultStr.push_back( tMemBitArr[i] );
	}
	
	return tResultStr;
}