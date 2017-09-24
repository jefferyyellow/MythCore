#include "stdafx.h"
#include "ParseProtobuf.h"
#include "direct.h"
CParseProtobuf::CParseProtobuf(void)
{
	Initialize();
}

CParseProtobuf::~CParseProtobuf(void)
{
}

void CParseProtobuf::Initialize()
{
	mXmldoc = NULL;
}

int CParseProtobuf::Field2Xml(const Message& rMessage, const Reflection* pReflection, const FieldDescriptor* pFieldDescriptor, int nIndex, XMLElement* tpParent, int* pTempID)
{
	FieldDescriptor::CppType emCppType = pFieldDescriptor->cpp_type();
	switch( emCppType )
	{
	case FieldDescriptor::CPPTYPE_INT32:
		{
			bool bRepeated = pFieldDescriptor->is_repeated();
			if ( bRepeated )
			{
				//char tNodeName[MAX_INDEX_DIGIT] = { 0 };
				//sprintf( tNodeName, "%d", nIndex );
				XMLElement* tpXmlElement = mXmldoc->NewElement(pFieldDescriptor->name().c_str());
				//tpXmlElement->SetAttribute("Id", nIndex);
				int nValue = pReflection->GetRepeatedInt32(rMessage, pFieldDescriptor, nIndex);
				//tpXmlElement->SetAttribute( "Type", "int32" );
				tpXmlElement->SetAttribute("Value", nValue );

				tpParent->LinkEndChild( tpXmlElement );
			}
			else
			{
				XMLElement* tpXmlElement = mXmldoc->NewElement(pFieldDescriptor->name().c_str());
				//tpXmlElement->SetAttribute( "Type", "int32" );
				//tpXmlElement->SetAttribute( "Field", pFieldDescriptor->name().c_str() );
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
				//sprintf( tNodeName, "%d", nIndex );
				XMLElement* tpXmlElement = mXmldoc->NewElement(pFieldDescriptor->name().c_str());
				//tpXmlElement->SetAttribute("Id", nIndex);
				long long int lValue = pReflection->GetRepeatedInt64(rMessage, pFieldDescriptor, nIndex);
				char tLValueArr[20] = { 0 };
				sprintf( tLValueArr, "%lld", lValue );
				//tpXmlElement->SetAttribute( "Type", "int64" );
				tpXmlElement->SetAttribute( "Value", tLValueArr );

				tpParent->LinkEndChild( tpXmlElement );
			}
			else
			{
				XMLElement* tpXmlElement = mXmldoc->NewElement(pFieldDescriptor->name().c_str());
				//tpXmlElement->SetAttribute( "Type", "int64" );
				//tpXmlElement->SetAttribute( "Field", pFieldDescriptor->name().c_str() );
				long long int lValue = pReflection->GetInt64(rMessage, pFieldDescriptor);
				char tLValueArr[20] = { 0 };
				sprintf( tLValueArr, "%lld", lValue );
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
				//sprintf( tNodeName, "%d", nIndex );
				XMLElement* tpXmlElement = mXmldoc->NewElement(pFieldDescriptor->name().c_str());
				//tpXmlElement->SetAttribute("Id", nIndex);
				//tpXmlElement->SetAttribute( "Type", "uint32" );
				unsigned int nValue = pReflection->GetRepeatedUInt32(rMessage, pFieldDescriptor, nIndex);
				tpXmlElement->SetAttribute("Value", nValue );

				tpParent->LinkEndChild( tpXmlElement );
			}
			else
			{
				XMLElement* tpXmlElement = mXmldoc->NewElement(pFieldDescriptor->name().c_str());
				//tpXmlElement->SetAttribute( "Type", "uint32" );
				//tpXmlElement->SetAttribute( "Field", pFieldDescriptor->name().c_str() );
				unsigned int nValue = pReflection->GetUInt32(rMessage, pFieldDescriptor);
				tpXmlElement->SetAttribute( "Value", (int)nValue );

				tpParent->LinkEndChild( tpXmlElement );

				if (0 == strcmp("TempID", pFieldDescriptor->name().c_str()) && NULL != pTempID)
				{
					*pTempID = nValue;
				}
			}
			break;
		}
	case FieldDescriptor::CPPTYPE_UINT64:
		{
			bool bRepeated = pFieldDescriptor->is_repeated();
			if ( bRepeated )
			{
				//char tNodeName[MAX_INDEX_DIGIT] = { 0 };
				//sprintf( tNodeName, "%d", nIndex );
				XMLElement* tpXmlElement = mXmldoc->NewElement(pFieldDescriptor->name().c_str());
				//tpXmlElement->SetAttribute("Id", nIndex);
				unsigned long long lValue = pReflection->GetRepeatedUInt64(rMessage, pFieldDescriptor, nIndex);
				char tLValueArr[20] = { 0 };
				sprintf( tLValueArr, "%llu", lValue );// ע�⣡"%llu"ֻ��32λ��������Ч����64λ�����ϻᱨ���棡
				//tpXmlElement->SetAttribute( "Type", "uint64" );
				tpXmlElement->SetAttribute( "Value", tLValueArr );

				tpParent->LinkEndChild( tpXmlElement );
			}
			else
			{
				XMLElement* tpXmlElement = mXmldoc->NewElement(pFieldDescriptor->name().c_str());
				//tpXmlElement->SetAttribute( "Type", "uint64" );
				//tpXmlElement->SetAttribute( "Field", pFieldDescriptor->name().c_str() );
				unsigned long long lValue = pReflection->GetUInt64(rMessage, pFieldDescriptor);
				char tLValueArr[20] = { 0 };
				sprintf( tLValueArr, "%llu", lValue );// ע�⣡"%llu"ֻ��32λ��������Ч����64λ�����ϻᱨ���棡
				tpXmlElement->SetAttribute( "Value", tLValueArr );

				tpParent->LinkEndChild( tpXmlElement );
			}
			break;
		}
	case FieldDescriptor::CPPTYPE_STRING:// ע�⣺CPPTYPE_STRING ���� TYPE_STRING, TYPE_BYTES ��������
		{
			bool bRepeated = pFieldDescriptor->is_repeated();
			if ( bRepeated )
			{
				//char tNodeName[MAX_INDEX_DIGIT] = { 0 };
				//sprintf( tNodeName, "%d", nIndex );
				XMLElement* tpXmlElement = mXmldoc->NewElement(pFieldDescriptor->name().c_str());
				//tpXmlElement->SetAttribute("Id", nIndex);
				std::string StrValue = pReflection->GetRepeatedString(rMessage, pFieldDescriptor, nIndex);

				switch( pFieldDescriptor->type() )
				{
				case FieldDescriptor::TYPE_BYTES:
					{
						//tpXmlElement->SetAttribute( "Type", "bytes" );
						//tpXmlElement->SetAttribute( "Field", pFieldDescriptor->name().c_str() );
						// ���� CBitData ����Ҳ���� MAX_MAIN_FUNCTION_UNLOCK_NUM : 128 ��template.h �ж��壩
						unsigned char tMaxBitArr[128] = { 0 };
						if ( StrValue.length() > 128 )
						{
							return -1;
						}
						memcpy( tMaxBitArr, StrValue.c_str(), StrValue.length() );// [ 0, StrValue.length() ) ��Χ�ڵĲ���Ч
						// �� tMaxBitArr �е�ÿ����Ա��ÿһ������λת��Ϊһ����Ӧ���ַ����洢��һ��������
						std::string tConvertStr("");
						for (int i = 0; i < StrValue.length() && i < 128 ; ++i)
						{
							for (int iBit = 0; iBit < 8; ++iBit)
							{
								unsigned char tModData = ( (1<<1) - 1 ) << iBit;
								bool tFinal = ( tMaxBitArr[i] & tModData ) >> iBit;
								if ( tFinal )
								{
									tConvertStr.push_back( '1' );
								}
								else
								{
									tConvertStr.push_back( '0' );
								}				
							}//end for (int iBit = 0; iBit < 8; ++iBit)
						}
						// �ַ���˳�����λ˳���෴
						std::string tConvertReverseStr( tConvertStr.rbegin(), tConvertStr.rend() );
						tpXmlElement->SetAttribute("Value", tConvertReverseStr.c_str() );
						break;
					}
				case FieldDescriptor::TYPE_STRING:
					{
						//tpXmlElement->SetAttribute( "Type", "string" );
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
				XMLElement* tpXmlElement = mXmldoc->NewElement(pFieldDescriptor->name().c_str());
				std::string StrValue = pReflection->GetString(rMessage, pFieldDescriptor);

				switch( pFieldDescriptor->type() )
				{
				case FieldDescriptor::TYPE_BYTES:
					{
						//tpXmlElement->SetAttribute( "Type", "bytes" );
						//tpXmlElement->SetAttribute( "Field", pFieldDescriptor->name().c_str() );

						// ���� CBitData ����Ҳ���� MAX_MAIN_FUNCTION_UNLOCK_NUM : 128 ��template.h �ж��壩
						unsigned char tMaxBitArr[128] = { 0 };
						if ( StrValue.length() > 128 )
						{
							return -1;
						}
						memcpy( tMaxBitArr, StrValue.c_str(), StrValue.length() );// [ 0, StrValue.length() ) ��Χ�ڵĲ���Ч
						// �� tMaxBitArr �е�ÿ����Ա��ÿһ������λת��Ϊһ����Ӧ���ַ����洢��һ��������
						std::string tConvertStr("");
						for (int i = 0; i < StrValue.length() && i < 128 ; ++i)
						{
							for (int iBit = 0; iBit < 8; ++iBit)
							{
								unsigned char tModData = ( (1<<1) - 1 ) << iBit;
								bool tFinal = ( tMaxBitArr[i] & tModData ) >> iBit;
								if ( tFinal )
								{
									tConvertStr.push_back( '1' );
								}
								else
								{
									tConvertStr.push_back( '0' );
								}				
							}//end for (int iBit = 0; iBit < 8; ++iBit)
						}
						// �ַ���˳�����λ˳���෴
						std::string tConvertReverseStr( tConvertStr.rbegin(), tConvertStr.rend() );
						tpXmlElement->SetAttribute("Value", tConvertReverseStr.c_str() );
						break;
					}
				case FieldDescriptor::TYPE_STRING:
					{
						//tpXmlElement->SetAttribute( "Type", "string" );
						//tpXmlElement->SetAttribute( "Field", pFieldDescriptor->name().c_str() );
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

				XMLElement* tpXmlElement = NULL;
				if (NULL != tpParent)
				{
					//char tNodeName[MAX_INDEX_DIGIT] = { 0 };
					//sprintf( tNodeName, "%d", nIndex );
					tpXmlElement = mXmldoc->NewElement(pFieldDescriptor->name().c_str());
					//tpXmlElement->SetAttribute("Id", nIndex);
					//tpXmlElement->SetAttribute( "Type", "Message" );
					//tpXmlElement->SetAttribute( "Class", pFieldTypeDescriptor->name().c_str() );// PB ����
					//tpXmlElement->SetAttribute( "Field", pFieldDescriptor->name().c_str() );// �ֶ���

					tpParent->LinkEndChild( tpXmlElement );
				}

				Message2Xml(rSubMessage, tpXmlElement, pTempID);
			}
			else
			{
				const Message& rSubMessage = pReflection->GetMessage(rMessage, pFieldDescriptor);
				const Descriptor* pFieldTypeDescriptor = pFieldDescriptor->message_type();

				XMLElement* tpXmlElement = NULL;
				if (NULL != tpParent)
				{
					tpXmlElement = mXmldoc->NewElement(pFieldDescriptor->name().c_str());
					//tpXmlElement->SetAttribute( "Type", "Message" );
					//tpXmlElement->SetAttribute( "Class", pFieldTypeDescriptor->name().c_str() );// PB ����
					//tpXmlElement->SetAttribute( "Field", pFieldDescriptor->name().c_str() );// �ֶ���

					tpParent->LinkEndChild( tpXmlElement );
				}

				Message2Xml(rSubMessage, tpXmlElement, pTempID);
			}
			break;
		}
	default:
		{
			const char* tpUnParsedFieldName = pFieldDescriptor->name().c_str();
			const char* tpUnParsedMsgTypeName = pFieldDescriptor->message_type()->name().c_str();
			if ( tpUnParsedFieldName != NULL && tpUnParsedMsgTypeName != NULL )
			{
				// ��ӡ������δ���н�������� cpp_type ����
				printf( "There is an UnParsed CppType ( Enum ) in FieldDescriptor::CppType : %d . ", (int)emCppType );
				printf( "The UnParsed Field Name is : %s, it's CppType is : %s\n", tpUnParsedFieldName, tpUnParsedMsgTypeName );
			}
			break;
		}	
	}//end switch ( pFieldDescriptor->cpp_type() )
	return 0;
}

int CParseProtobuf::Message2Xml(const Message& rMessage, XMLElement* tpParent, int* pTempID)
{
	const Reflection* pReflection = rMessage.GetReflection();
	const Descriptor* pDescriptor = rMessage.GetDescriptor();
	for(int i = 0; i < pDescriptor->field_count(); ++ i)
	{
		const FieldDescriptor* pFieldDescriptor = pDescriptor->field(i);

		// �Ƿ���Ҫ�����ļ�
		bool bCreateFile = false;
		// �Ƿ�������
		bool bConfig = false;
		// �Ƿ���
		bool bItem = false;
		if (strcmp(pDescriptor->name().c_str(), "PBTemplate") == 0)
		{
			bCreateFile = true;
			if (strcmp(pFieldDescriptor->name().c_str(), "ItemSet") == 0)
			{
				bCreateFile = false;
			}
			else if (strcmp(pFieldDescriptor->name().c_str(), "ConfigTable") == 0)
			{
				bCreateFile = false;
			}
			else if (strcmp(pFieldDescriptor->name().c_str(), "SkillSet") == 0)
			{
				bCreateFile = false;
			}
		}
		else if (strcmp(pDescriptor->name().c_str(), "PBItemSet") == 0)
		{
			bCreateFile = true;
			bItem = true;
		}
		else if (strcmp(pDescriptor->name().c_str(), "PBConfigTable") == 0)
		{
			bCreateFile = true;
			bConfig = true;
		}
		else if (strcmp(pDescriptor->name().c_str(), "PBSkillSet") == 0)
		{
			bCreateFile = true;
		}

		mXmldoc = NULL;

		if( pFieldDescriptor->is_repeated() )
		{
			const Descriptor* pFieldTypeDescriptor = pFieldDescriptor->message_type();
			int nCount = pReflection->FieldSize(rMessage, pFieldDescriptor);// �� FieldSize ����1ֻ���ܾ����PB
			if (0 == nCount)
			{
				continue;
			}

			// �����Ҫ�����ļ��Ҳ���ֻ����һ�����õ�����
			if (bCreateFile && !(nCount <= 1 && bConfig)&& !bItem)
			{
				char acDirectoryName[MAX_PATH] = {0};
				_snprintf(acDirectoryName, sizeof(acDirectoryName), "client/%s", pFieldDescriptor->name().c_str());
				_mkdir(acDirectoryName);

			}
			//XMLElement* tpXmlElement = new XMLElement(pFieldTypeDescriptor->name().c_str());
			//switch( pFieldDescriptor->cpp_type() )
			//{
			//case FieldDescriptor::CPPTYPE_INT32:
			//	{
			//		tpXmlElement->SetAttribute( "Type", "int32" );
			//		break;
			//	}
			//case FieldDescriptor::CPPTYPE_INT64:
			//	{
			//		tpXmlElement->SetAttribute( "Type", "int64" );
			//		break;
			//	}
			//case FieldDescriptor::CPPTYPE_UINT32:
			//	{
			//		tpXmlElement->SetAttribute( "Type", "uint32" );
			//		break;
			//	}
			//case FieldDescriptor::CPPTYPE_MESSAGE:
			//	{
			//		//tpXmlElement->SetAttribute( "Type", pFieldTypeDescriptor->name().c_str() );
			//		tpXmlElement->SetAttribute( "Type", "Message" );
			//		tpXmlElement->SetAttribute( "Class", pFieldTypeDescriptor->name().c_str() );
			//		break;
			//	}
			//default:
			//	break;
			//}//end switch( pFieldDescriptor->cpp_type() )
			//tpXmlElement->SetAttribute("Field", pFieldDescriptor->name().c_str() );
			//tpParent->LinkEndChild( tpXmlElement );

			//tpXmlElement->SetAttribute( "Size", nCount );
			for(int iRepeated = 0; iRepeated < nCount; ++ iRepeated)
			{
				int nTempID = 0;

				if (bCreateFile)
				{
					
					mXmldoc = new tinyxml2::XMLDocument();
					XMLDeclaration* pxmlDec = mXmldoc->NewDeclaration();
					mXmldoc->LinkEndChild( pxmlDec );

					XMLElement* pRootElem = mXmldoc->NewElement( "template_data" );
					mXmldoc->LinkEndChild(pRootElem);
					tpParent = pRootElem;
					pTempID = &nTempID;
				}

				Field2Xml( rMessage, pReflection, pFieldDescriptor, iRepeated, tpParent, pTempID); 

				if (bCreateFile)
				{
					char acXmlFileName[MAX_PATH] = {0};
					if (bConfig && nCount <= 1)
					{
						// ֱ�ӽ�������Ϊ�ļ���
						_snprintf(acXmlFileName, sizeof(acXmlFileName), "client/%s%s", pFieldDescriptor->name().c_str(), ".xml");
					}
					else
					{
						if (*pTempID == 0)
						{
							AfxMessageBox(_T("����ǰ��ģ���xml���ֳ���!,��ȷ��ģ��PB��ģ��ID���ֶ���Ϊ��TempID����"));
						}
						// �ļ���Ϊģ��ID
						if (bItem)
						{
							_snprintf(acXmlFileName, sizeof(acXmlFileName), "client/Item/%d%s", *pTempID, ".xml");
						}
						else
						{
							_snprintf(acXmlFileName, sizeof(acXmlFileName), "client/%s/%d%s", pFieldDescriptor->name().c_str(), *pTempID, ".xml");
						}
					}

					mXmldoc->SaveFile(acXmlFileName);
					delete mXmldoc;
				}
			}
		}//end if( pFieldDescriptor->is_repeated() )
		else
		{
			int nTemplateID = 0;

			if (bCreateFile)
			{
				mXmldoc = new tinyxml2::XMLDocument();
				XMLDeclaration* pxmlDec = mXmldoc->NewDeclaration();
				mXmldoc->LinkEndChild( pxmlDec );

				XMLElement* pRootElem = mXmldoc->NewElement( "template_data" );
				mXmldoc->LinkEndChild(pRootElem);


				XMLElement* tpPB = mXmldoc->NewElement(pFieldDescriptor->name().c_str() );
				pRootElem->LinkEndChild( tpPB );
				tpParent = pRootElem;
				pTempID = &nTemplateID;
			}
			
			Field2Xml( rMessage, pReflection, pFieldDescriptor, -1, tpParent, pTempID);
			if (bCreateFile)
			{
				char acXmlFileName[MAX_PATH] = {0};
				if (bConfig)
				{
					_snprintf(acXmlFileName, sizeof(acXmlFileName), "client/%s%s", pFieldDescriptor->name().c_str(), ".xml");
				}
				else
				{

					if (*pTempID == 0)
					{
						AfxMessageBox(_T("����ǰ��ģ���xml���ֳ���!,��ȷ��ģ��PB��ģ��ID���ֶ���Ϊ��TempID����"));
					}
					if (bItem)
					{
						_snprintf(acXmlFileName, sizeof(acXmlFileName), "client/Item/%d%s", *pTempID, ".xml");
					}
					else
					{
						_snprintf(acXmlFileName, sizeof(acXmlFileName), "client/%s/%d%s", pFieldDescriptor->name().c_str(), *pTempID, ".xml");
					}
				}
				mXmldoc->SaveFile(acXmlFileName);
				delete mXmldoc;
			}
		} 

		//if (bCreateFile)
		//{
		//	char acXmlFileName[MAX_PATH] = {0};
		//	_snprintf(acXmlFileName, sizeof(acXmlFileName), "client/%s%s", pFieldDescriptor->name().c_str(), ".xml");

		//	mXmldoc->SaveFile(acXmlFileName);
		//	delete mXmldoc;
		//}
	}
	return 0;
}