#include "DBMsgFactory.h"

#include "db_msg_pb.hxx.pb.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/descriptor.pb.h"

#include "property_pb.hxx.pb.h"
#include "servermessage_pb.hxx.pb.h"
#include "arenamessage_pb.hxx.pb.h"
#include "activitymessag_pb.hxx.pb.h"
//#include "servermessage_pb.hxx"

typedef ::google::protobuf::Message Message;
typedef ::google::protobuf::Descriptor Descriptor;
typedef ::google::protobuf::Reflection Reflection;
typedef ::google::protobuf::FieldDescriptor FieldDescriptor;
typedef ::google::protobuf::EnumValueDescriptor EnumValueDescriptor;


// ParseCore(...) 函数专属宏，不得随意修改
#define CASE_PARSE_PB(pbEnum,PBClass)	\
case pbEnum:\
{\
	PBClass tPBData;\
	if( !tPBData.ParseFromArray( pFieldStr, pFieldLen ) )	\
{\
	printf("PB ParseFromArray failed\n");\
	break;\
	}\
	const Descriptor* tpDescriptor = tPBData.GetDescriptor();\
	if( tpDescriptor != NULL )\
{\
	tpPBXmlNode = new TiXmlElement( tpDescriptor->name().c_str() );\
	}\
	pNodeInfo->GetXmlRoot()->LinkEndChild( tpPBXmlNode );\
	CProtoXMLManager::GetSingleton().Message2Xml( tPBData, tpPBXmlNode );\
	\
	break;\
	}\

// 必须 使用 "mHashMapMacro" 作为参数传递
// 目前 mHashMapMacro 只能是 mField2emPBMap、mPBName2PBMap
#define InsertField2PB_HashMap( mHashMapMacro, STRING, emPB )	\
	tPairib = mHashMapMacro.insert( DBField2PB_HashMap::value_type( STRING , emPB) );	\
	if ( tPairib.second == false )\
{\
	printf(" " #emPB" insert Failed !\n");\
	return;\
	}\


Message* GetMessage(Message*& pRoot, const char* pTableName, const char* pFieldName)
{
	Message* pMessage = NULL;
	if( strcmp("UMS_ROLE_INFO", pTableName) == 0 )
	{
		if(pRoot == NULL)
		{
			pRoot = new CMessagePlayerProperty;
		}
		const Descriptor* tpDesc = pRoot->GetDescriptor();
		const Reflection* tpRef = pRoot->GetReflection();

		const FieldDescriptor* tpFindFieldDesc = tpDesc->FindFieldByName( pFieldName );
		if ( tpFindFieldDesc != NULL )
		{
			if ( tpFindFieldDesc->cpp_type() == FieldDescriptor::CPPTYPE_MESSAGE )
			{
				pMessage = tpRef->MutableMessage(pRoot, tpFindFieldDesc);
			}
		}
	}	
	else if( strcmp("GAME_EVENT", pTableName) == 0 )
	{
		if(pRoot == NULL)
		{
			pRoot = new PBGameEvent;
		}
		const Descriptor* tpDesc = pRoot->GetDescriptor();
		const Reflection* tpRef = pRoot->GetReflection();
		for(int i = 0; i < tpDesc->field_count(); ++ i)
		{
			const FieldDescriptor* pFieldDesc = tpDesc->field(i);
			if(pFieldDesc->cpp_type() == FieldDescriptor::CPPTYPE_MESSAGE
			   	&& pFieldDesc->name() == std::string(pFieldName))
			{
				pMessage = tpRef->MutableMessage(pRoot, pFieldDesc);
				break;
			}
		}
	}
	else if( strcmp("UMS_GLOBAL_DATA", pTableName) == 0 )
	{
		if(strcmp("global_data", pFieldName) == 0)
		{
			pRoot = new PBGlobalData;
			pMessage = pRoot;
		}
		else if(strcmp("boss_siege", pFieldName) == 0)
		{
			pRoot = new PBSaveBossSiegeInfo;
			pMessage = pRoot;
		}
	}
	else if( strcmp("UMS_FACTION", pTableName) == 0 )
	{
		if( strcmp("faction_member", pFieldName) == 0 )
		{
			pRoot = new PBFactionMemberListDB;
			pMessage = pRoot;
		}
		else if( strcmp("faction_apply", pFieldName) == 0 )
		{
			pRoot = new PBFactionApplyListDB;
			pMessage = pRoot;
		}
		else if( strcmp("faction_data", pFieldName) == 0 )
		{
			pRoot = new PBFactionData;
			pMessage = pRoot;
		}
	}

	return pMessage;
}

// 根据 PB 根对象和 PB 字段名，返回 PB 字段对应的 PB 对象
Message* GetSubMessage(Message*& pRoot, const char* pFieldName)
{
	Message* pMessage = NULL;
	const Descriptor* tpDesc = pRoot->GetDescriptor();
	const Reflection* tpRef = pRoot->GetReflection();
	for(int i = 0; i < tpDesc->field_count(); ++i)
	{
		const FieldDescriptor* pFieldDesc = tpDesc->field(i);
		if(pFieldDesc->cpp_type() == FieldDescriptor::CPPTYPE_MESSAGE
			&& pFieldDesc->name() == std::string(pFieldName))
		{
			pMessage = tpRef->MutableMessage(pRoot, pFieldDesc);
			break;
		}
	}
	return pMessage;
}

TiXmlElement* CParser::PB2Xml(CXmlNode* pNodeInfo, const char* pFieldStr, int pFieldLen)
{
	if ( pNodeInfo == NULL || pFieldStr == NULL || pFieldLen == 0)
	{
		return NULL;
	}

	const char* tpFormName = pNodeInfo->GetFormName();
	const char* tpQueryStr = pNodeInfo->GetQueryStr();
	if ( tpQueryStr == NULL || tpFormName == NULL )
	{
		return NULL;
	}

	std::string tMapKey( tpFormName );
	tMapKey.append(" ");
	tMapKey.append( tpQueryStr );

	TiXmlElement* tpPBXmlNode = NULL;
	Message* pRootMessage = NULL;
	Message* pMessage = GetMessage(pRootMessage, tpFormName, tpQueryStr); 
	if(!pMessage->ParseFromArray( pFieldStr, pFieldLen ) )
	{
		printf("PB ParseFromArray failed\n");
		return NULL;
	}
	const Descriptor* tpDescriptor = pMessage->GetDescriptor();
	if( tpDescriptor != NULL )
	{
		tpPBXmlNode = new TiXmlElement( tpDescriptor->name().c_str() );
	}
	pNodeInfo->GetXmlRoot()->LinkEndChild( tpPBXmlNode );
	CProtoXMLManager::GetSingleton().Message2Xml(*pMessage , tpPBXmlNode );

//	switch( emPB )// 注意 CASE_PB 后不能加分号
//	{
//		// GAME_EVENT
//		CASE_PARSE_PB( emPBEventParam, PBEventParam )
//
//		// UMS_ROLE_INFO : 
//		//CASE_PARSE_PB( emPBPlayerData, PBPlayerData )
//		CASE_PARSE_PB( emPBTaskList, PBTaskList )
//		CASE_PARSE_PB( emPBBuffList, PBBuffList )
//		CASE_PARSE_PB( emPBItemBox_Package, PBItemBox )
//		CASE_PARSE_PB( emPBItemBox_Storage, PBItemBox )
//		CASE_PARSE_PB( emPBPlayerTitleInfo, PBPlayerTitleInfo )
//		CASE_PARSE_PB( emPBActivity, PBActivity )
//		CASE_PARSE_PB( emPBHeartMagicInfo, PBHeartMagicInfo )
//		CASE_PARSE_PB( emPBPlayerAllInfo, PBPlayerAllInfo )
//		CASE_PARSE_PB( emPBArenaData, PBArenaData )
//		CASE_PARSE_PB( emPBPlayerData, PBPlayerData )
//		CASE_PARSE_PB( emPBArenaHistoryData, PBArenaHistoryData )
//		CASE_PARSE_PB( emPBItemBox_Gem_Package, PBItemBox )
//		// UMS_GLOBAL_DATA : 
//		CASE_PARSE_PB( emPBOldArenaPlace, PBOldArenaPlace )
//		CASE_PARSE_PB( emPBGlobalData, PBGlobalData )
//		CASE_PARSE_PB( emPBFactionGlobal, PBFactionGlobal )
//		CASE_PARSE_PB( emPBGlobalLevelRank, PBGlobalLevelRank )
//		CASE_PARSE_PB( emPBWorldWarGlobal, PBWorldWarGlobal )
//		CASE_PARSE_PB( emPBGlobalActivityStatus, PBGlobalActivityStatus )
//		// UMS_WORLDWAR
//		CASE_PARSE_PB( emPBWorldWarDB, PBWorldWarDB )
//		CASE_PARSE_PB( emPBWorldWarDB_1, PBWorldWarDB )
//		CASE_PARSE_PB( emPBWorldWarDB_2, PBWorldWarDB )
//		CASE_PARSE_PB( emPBWorldWarDB_3, PBWorldWarDB )
//		CASE_PARSE_PB( emPBWorldWarDB_4, PBWorldWarDB )
//		CASE_PARSE_PB( emPBWorldWarDB_5, PBWorldWarDB )
//		CASE_PARSE_PB( emPBWorldWarDB_6, PBWorldWarDB )
//		CASE_PARSE_PB( emPBWorldWarDB_7, PBWorldWarDB )
//		CASE_PARSE_PB( emPBWorldWarDB_8, PBWorldWarDB )
//		// UMS_WORLDWAR_ROLE
//		CASE_PARSE_PB( emPBWorldWarRole, PBWorldWarRole )
//	default:
//		{
//			printf("There is No PB\n");
//		}
//	}

	// 设置首个结点属性，并输出xml内容到文件
	TiXmlElement* tpRoot = pNodeInfo->GetXmlRoot();
	TiXmlDocument* tpXmlDoc = pNodeInfo->GetXmlDoc();
	if ( tpRoot != NULL && tpXmlDoc != NULL )
	{
		TiXmlElement* tpFirstNode = tpRoot->FirstChildElement();
		if ( tpFirstNode != NULL )
		{
			tpFirstNode->SetAttribute( "DataBase_Field", pNodeInfo->GetQueryStr() );
			tpFirstNode->SetAttribute( "PrimaryName", pNodeInfo->GetPrimaryName() );
			tpFirstNode->SetAttribute( "PrimaryKeyVal", pNodeInfo->GetPrimaryKeyVal() );
		}

		XML_ASSERT( tpXmlDoc->LinkEndChild( tpRoot ) );
		bool bSave = tpXmlDoc->SaveFile();
		if ( !bSave )
		{
			return NULL;
		}

		std::string tXmlName("");
		tXmlName.append( pNodeInfo->GetFormName() );
		tXmlName.append(".");
		tXmlName.append( pNodeInfo->GetQueryStr() );
		tXmlName.append(".xml");
		printf( "Building new File : %s\n", tXmlName.c_str() );
	}

	delete pRootMessage;	
	return tpPBXmlNode;
}

//************************************
// Method:		Initialize
// Returns:		int
// Parameter:	TiXmlElement * pRoot
// Parameter:	char * pFieldStr	—— pField[iCol].GetString()
// Parameter:	int nFieldLen		—— pField[iCol].GetValueLen()
// Author:			Sunrise Meier
// Date:				10/30/2014 15:12
// Comment:		
//************************************
int CXmlNode::Initialize(const char* pQueryStr, const char* pFormName, const char* pPrimaryKeyVal )
{
	if ( pFormName == NULL )
	{
		return -1;
	}

	mpPrimaryName = PBToolCommon::GetPrimaryName(pFormName);
	if ( mpPrimaryName == NULL )
	{
		return -1;
	}
	mpQueryStr = pQueryStr;
	if ( mpQueryStr == NULL )
	{
		return -1;
	}

	mpPrimaryKeyVal = pPrimaryKeyVal;
	if ( mpPrimaryKeyVal == NULL )
	{
		return -1;
	}

	mpFormName = pFormName;
	if ( mpFormName == NULL )
	{
		return -1;
	}

	mpRoot = new TiXmlElement( pFormName );// Root : 表名

	// 生成xml文件路径
	std::string pBuildPath("");
	const char* tEachPath[3] = { "xmlfiles", pFormName, pPrimaryKeyVal };
	for (int i = 0; i < 3; ++i)
	{
		pBuildPath.append( tEachPath[i] );
		pBuildPath.append("/");

		if( PBToolCommon::CheckDirection( pBuildPath.c_str() ) != 0 )
			continue;
	}

	std::string tpPath(pBuildPath);

	std::string tXmlName( pFormName );
	tXmlName.append(".");
	tXmlName.append( pQueryStr );
	tXmlName.append(".xml");

	tpPath.append( tXmlName );

	// 组装 xml 文件头
	TiXmlDocument* tpXmlDoc = new TiXmlDocument( tpPath.c_str() );
	TiXmlDeclaration tXmlDeclaration("1.0", "gb2312", "yes");
	XML_ASSERT ( tpXmlDoc->InsertEndChild( tXmlDeclaration ) );

	mpXmlDoc = tpXmlDoc;
	return 0;
}

// 清除所有结点
void CXmlNode::ClearNodes()
{
	if ( mpXmlDoc != NULL )
	{
		mpXmlDoc->Clear();
		delete mpXmlDoc;
		mpXmlDoc = NULL;
	}
}

void CParser::CreateDBField2PBMap()
{
	mDBField2PBMap.initailize();

	DBField2PB_HashMap::_Pairib tPairib;
	DBField2PB_HashMap& rHashMapMacro = mDBField2PBMap;

	// GAME_EVENT
	InsertField2PB_HashMap( rHashMapMacro, "GAME_EVENT event_param", emPBEventParam);

	// UMS_APPRENTICE
	InsertField2PB_HashMap( rHashMapMacro, "UMS_APPRENTICE apprentice_data", emPBApprentice);

	// UMS_FACTION
	InsertField2PB_HashMap( rHashMapMacro, "UMS_FACTION faction_member", emPBMemberList);
	InsertField2PB_HashMap( rHashMapMacro, "UMS_FACTION faction_apply", emPBFactionApplyList);

	// UMS_ROLE_INFO
	// InsertField2PB_HashMap( rHashMapMacro, "UMS_ROLE_INFO role_basic", emPBPlayerData) //错误的解析，居然也能解成功？？？
	InsertField2PB_HashMap( rHashMapMacro, "UMS_ROLE_INFO role_basic", emPBPlayerBasicInfo);
	InsertField2PB_HashMap( rHashMapMacro, "UMS_ROLE_INFO role_task", emPBTaskList);
	InsertField2PB_HashMap( rHashMapMacro, "UMS_ROLE_INFO role_skill", emPBSkillList);
	InsertField2PB_HashMap( rHashMapMacro, "UMS_ROLE_INFO role_buff", emPBBuffList);
	InsertField2PB_HashMap( rHashMapMacro, "UMS_ROLE_INFO role_horse", emPBPlayerHorseInfo);
	InsertField2PB_HashMap( rHashMapMacro, "UMS_ROLE_INFO role_package", emPBItemBox_Package);
	//InsertField2PB_HashMap( rHashMapMacro, "UMS_ROLE_INFO role_equipment", )
	InsertField2PB_HashMap( rHashMapMacro, "UMS_ROLE_INFO role_storage", emPBItemBox_Storage);
	InsertField2PB_HashMap( rHashMapMacro, "UMS_ROLE_INFO role_title", emPBPlayerTitleInfo);
	InsertField2PB_HashMap( rHashMapMacro, "UMS_ROLE_INFO role_activity", emPBActivity);
	//InsertField2PB_HashMap( rHashMapMacro, "UMS_ROLE_INFO role_task_package", )
	InsertField2PB_HashMap( rHashMapMacro, "UMS_ROLE_INFO role_array", emPBPlayerArray);
	InsertField2PB_HashMap( rHashMapMacro, "UMS_ROLE_INFO role_partner", emPBPartnerInfo);
	InsertField2PB_HashMap( rHashMapMacro, "UMS_ROLE_INFO role_heartmagic", emPBHeartMagicInfo);
	InsertField2PB_HashMap( rHashMapMacro, "UMS_ROLE_INFO role_allinfo", emPBPlayerAllInfo);
	InsertField2PB_HashMap( rHashMapMacro, "UMS_ROLE_INFO role_arena_data", emPBArenaData);
	InsertField2PB_HashMap( rHashMapMacro, "UMS_ROLE_INFO role_player_data", emPBPlayerData);
	//InsertField2PB_HashMap( rHashMapMacro, "UMS_ROLE_INFO slave_id", )
	InsertField2PB_HashMap( rHashMapMacro, "UMS_ROLE_INFO role_arena_history_data", emPBArenaHistoryData);
	InsertField2PB_HashMap( rHashMapMacro, "UMS_ROLE_INFO role_gem_package", emPBItemBox_Gem_Package);
	InsertField2PB_HashMap( rHashMapMacro, "UMS_ROLE_INFO role_artifact", emPBPlayerArtifactData);
	// UMS_GLOBAL_DATA
	InsertField2PB_HashMap( rHashMapMacro, "UMS_GLOBAL_DATA arena_place_trend", emPBArenaPlaceTrend);
	InsertField2PB_HashMap( rHashMapMacro, "UMS_GLOBAL_DATA arena_old_place", emPBOldArenaPlace);
	InsertField2PB_HashMap( rHashMapMacro, "UMS_GLOBAL_DATA activity_multifb", emPBMultiFBRandInvitePlayer);
	InsertField2PB_HashMap( rHashMapMacro, "UMS_GLOBAL_DATA mystic_shop_sell_log", emPBMysticShopSellLog);
	InsertField2PB_HashMap( rHashMapMacro, "UMS_GLOBAL_DATA global_data", emPBGlobalData);
	InsertField2PB_HashMap( rHashMapMacro, "UMS_GLOBAL_DATA faction_data", emPBFactionGlobal);
	InsertField2PB_HashMap( rHashMapMacro, "UMS_GLOBAL_DATA jxz_global_rank", emPBJXZGlobalRank);
	InsertField2PB_HashMap( rHashMapMacro, "UMS_GLOBAL_DATA player_level_rank", emPBGlobalLevelRank);
	InsertField2PB_HashMap( rHashMapMacro, "UMS_GLOBAL_DATA worldwar_global", emPBWorldWarGlobal);
	InsertField2PB_HashMap( rHashMapMacro, "UMS_GLOBAL_DATA player_horse_rank", emPBPlayerHorseRank);
	InsertField2PB_HashMap( rHashMapMacro, "UMS_GLOBAL_DATA global_status", emPBGlobalActivityStatus);
	// UMS_WORLDWAR
	InsertField2PB_HashMap( rHashMapMacro, "UMS_WORLDWAR war_data", emPBWorldWarDB);
	InsertField2PB_HashMap( rHashMapMacro, "UMS_WORLDWAR war_data1", emPBWorldWarDB_1);
	InsertField2PB_HashMap( rHashMapMacro, "UMS_WORLDWAR war_data2", emPBWorldWarDB_2);
	InsertField2PB_HashMap( rHashMapMacro, "UMS_WORLDWAR war_data3", emPBWorldWarDB_3);
	InsertField2PB_HashMap( rHashMapMacro, "UMS_WORLDWAR war_data4", emPBWorldWarDB_4);
	InsertField2PB_HashMap( rHashMapMacro, "UMS_WORLDWAR war_data5", emPBWorldWarDB_5);
	InsertField2PB_HashMap( rHashMapMacro, "UMS_WORLDWAR war_data6", emPBWorldWarDB_6);
	InsertField2PB_HashMap( rHashMapMacro, "UMS_WORLDWAR war_data7", emPBWorldWarDB_7);
	InsertField2PB_HashMap( rHashMapMacro, "UMS_WORLDWAR war_data8", emPBWorldWarDB_8);
	// UMS_WORLDWAR_ROLE
	InsertField2PB_HashMap( rHashMapMacro, "UMS_WORLDWAR_ROLE property_data", emPBWorldWarRole);
}
