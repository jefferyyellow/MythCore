#include "ParseProtobuf.h"
#include "base.h"

// Linux Use
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "db_msg_pb.hxx.pb.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/descriptor.pb.h"

#include "property_pb.hxx.pb.h"
#include "servermessage_pb.hxx.pb.h"
#include "arenamessage_pb.hxx.pb.h"
#include "activitymessag_pb.hxx.pb.h"
//#include "servermessage_pb.hxx"
#include <iostream>
#include <string>
#include <fstream>

typedef ::google::protobuf::Message Message;
typedef ::google::protobuf::Descriptor Descriptor;
typedef ::google::protobuf::Reflection Reflection;
typedef ::google::protobuf::FieldDescriptor FieldDescriptor;
typedef ::google::protobuf::EnumValueDescriptor EnumValueDescriptor;



CParseProtobuf::CParseProtobuf(void) : mbMySql(false)
{
	Initialize();
}

CParseProtobuf::~CParseProtobuf(void)
{
}

void CParseProtobuf::Initialize()
{
	// CreateForm2FieldMapping();// 建立“表名——Blob字段名集合”的映射
}

// 初始化数据库连接
bool CParseProtobuf::InitializeMysql(const char* pIP, int nPort, const char* pUser, const char* pPasswd, const char* pDBName)
{
	if ( pIP == NULL || pUser == NULL || pPasswd == NULL || pDBName == NULL )
	{
		return false;
	}

	bool bResult = mMysql.Initialize( pIP, 3306, pUser, pPasswd, pDBName );
	if ( bResult == false )
	{
		return false;
	}
	//printf("------------------------------------------------- Initialize Mysql Successfully ! -------------------------------------------------\n");
	printf("\n");
	printf("*********************************************************************************************************************\n");
	printf("------------------------------------------------- 连接数据库成功 ! -------------------------------------------------\n");
	printf("*********************************************************************************************************************\n");
	printf("\n");
	mbMySql = true;
	return true;
}

// 在某一连接下切换别的数据库
bool CParseProtobuf::SwitchOtherDataBase(const char* pDataBase)
{
	if ( pDataBase == NULL )
	{
		return false;
	}
	return mMysql.SwitchDataBase(pDataBase);
}

// 建立“表名——Blob字段名集合”的映射
void CParseProtobuf::CreateForm2FieldMapping()
{
	mForm2FieldHashMap.initailize();
	const char* tFields = "";
	tFields = "event_param";
	mForm2FieldHashMap.insert( Form2Field_HashMap::value_type("GAME_EVENT", tFields) );
	tFields = "apprentice_data";
	mForm2FieldHashMap.insert( Form2Field_HashMap::value_type("UMS_APPRENTICE", tFields) );
	tFields = "faction_member, faction_apply";
	mForm2FieldHashMap.insert( Form2Field_HashMap::value_type("UMS_FACTION", tFields) );
	tFields = "arena_place_trend, arena_old_place, activity_multifb, mystic_shop_sell_log, global_data, faction_data, jxz_global_rank, player_level_rank, worldwar_global, player_horse_rank, global_status" ;
	mForm2FieldHashMap.insert( Form2Field_HashMap::value_type("UMS_GLOBAL_DATA", tFields) );
	//const char* tUMS_ROLE_INFO_Fields = "role_basic, role_task, role_skill, role_buff, role_horse, role_package, role_equipment, role_storage, role_title, role_activity, role_task_package, role_array, role_partner, role_heartmagic, role_allinfo, role_arena_data, role_player_data, slave_id, role_arena_history_data, role_gem_package, role_artifact";
	tFields = "role_basic, role_task, role_skill, role_buff, role_horse, role_package, role_storage, role_title, role_activity, role_array, role_partner, role_heartmagic, role_allinfo, role_arena_data, role_player_data, role_arena_history_data, role_gem_package, role_artifact";
	mForm2FieldHashMap.insert( Form2Field_HashMap::value_type("UMS_ROLE_INFO", tFields) );
	tFields = "war_data, war_data1, war_data2, war_data3, war_data4, war_data5, war_data6, war_data7, war_data8";
	mForm2FieldHashMap.insert( Form2Field_HashMap::value_type("UMS_WORLDWAR", tFields) );
	tFields = "property_data";
	mForm2FieldHashMap.insert( Form2Field_HashMap::value_type("UMS_WORLDWAR_ROLE", tFields) );
}

// 通用解析，bAll 为true表示解析某个主键下的所有字段PB（此时 pFieldName 不起作用）
int CParseProtobuf::ParseCore( const char* pFormName, const char* pFieldName, const char* pPrimaryKeyVal, bool bAll )
{
	if ( pFormName == NULL || pFieldName == NULL || pPrimaryKeyVal == NULL )
	{
		return -1;
	}
	if ( !mbMySql )
	{
		printf("Initialize mysql Failed !\n");
		return -1;
	}

	Tokens tQueryFields;// 需要查询的字段列表
	// 拼查询字符串
	std::string tQueryString("select ");

	if ( bAll )// 一键生成
	{
		Form2Field_HashMap::iterator itForm2FieldMap = mForm2FieldHashMap.find( pFormName );
		std::string& tFieldNames = itForm2FieldMap->second;
		tQueryString.append( tFieldNames );

		tQueryFields = StrSplit( tFieldNames, ", " );
	}
	else
	{
		tQueryFields.push_back( pFieldName );
		tQueryString.append( pFieldName );
	}

	tQueryString.append(" from ");
	tQueryString.append( pFormName );
	tQueryString.append(" where ");
	const char* pPrimartName = PBToolCommon::GetPrimaryName( pFormName );
	if ( pPrimartName == NULL )
	{
		printf("There is not Form : %s and it's Primary Mapping .\n", pFormName);
		return -1;
	}
	tQueryString.append( pPrimartName );
	tQueryString.append(" = ");
	tQueryString.append( pPrimaryKeyVal );

	CQueryResultMysql* pResult = mMysql.Query( tQueryString.c_str() );
	if(NULL == pResult)
	{
		printf("Query Failed !\n");
		return -1;
	}

	int nRow = pResult->GetRowCount();
	int nColumn = pResult->GetFiledCount();
	if(0 == nRow)
	{
		printf("The Result of This Query is Null .\n");
		return -1;
	}
	if ( nColumn != tQueryFields.size() )
	{
		printf("Query Field Num Error !\n");
		return -1;
	}

	// 创建解析器
	CParser tParser;

	for(int iRow = 0; iRow < nRow; ++iRow)
	{
		Field* pField = pResult->GetCurrentRow();

		for (int iCol = 0; iCol < nColumn ; ++iCol)
		{
			// 必须是 Blob 类型的字段才允许解析
			if ( pField[iCol].GetDataType() != emDataType_Blob )
			{
				printf("The Field is not a Blob Field, The Tool can't Parse . \n");
				return -1;
			}

			// 初始化解析器对象
			CXmlNode tXmlNode;
			int tRet = tXmlNode.Initialize( tQueryFields[iCol].c_str(), pFormName, pPrimaryKeyVal );
			if ( tRet != 0 )
			{
				return -1;
			}

			// 根据对应 PB 枚举进行解析
			TiXmlElement* tpPB = tParser.PB2Xml( &tXmlNode, pField[iCol].GetString(), pField[iCol].GetValueLen() );
			tXmlNode.ClearNodes();

		}//enf for iCol

		pResult->NextRow();
	}//end for iRow

	printf("********************************************************************\n");
	printf("*** Parse PB To XML Success ! ***\n" );
	printf("********************************************************************\n");
	return 0;
}

// 把xml写回到数据库
int CParseProtobuf::SerializeToArray( const char* pName )
{
	return 0;
}

// 根结点删除
void CParseProtobuf::ClearNodes( TiXmlDocument* tpXmlDoc )
{
	if ( tpXmlDoc != NULL )
	{
		tpXmlDoc->Clear();
		delete tpXmlDoc;
		tpXmlDoc = NULL;
	}
}

// 根据 xml 结点对象生成相应的 PB 对象
int CParseProtobuf::CreatePBCore( const char* pFormName, const char* pFieldName, const char* pPrimaryKeyVal )
{
	if ( pFormName == NULL || pFieldName == NULL || pPrimaryKeyVal == NULL )
	{
		return -1;
	}
	if ( !mbMySql )
	{
		printf("Initialize mysql Failed !\n");
		return -1;
	}

	std::string strPath("./xmlfiles/");
	strPath.append( pFormName );
	strPath.append( "/" );
	strPath.append( pPrimaryKeyVal );
	strPath.append( "/" );
	strPath.append( pFormName );
	strPath.append( "." );
	strPath.append( pFieldName );
	strPath.append( ".xml" );

	const char* pFilePath = strPath.c_str();
	if ( pFilePath == NULL )
	{
		return -1;
	}

	TiXmlDocument tXmlDoc;
	bool bLoad = tXmlDoc.LoadFile( pFilePath );
	if ( !bLoad )
	{
		printf("Load XML File Failed !\n");
		return -1;
	}

	TiXmlElement* tpRoot = tXmlDoc.FirstChildElement();
	if ( tpRoot == NULL )
	{
		return -1;
	}

	TiXmlElement* tpMsgXmlNode = tpRoot->FirstChildElement();// <UMS_表>下的字段标签结点
	if ( tpMsgXmlNode == NULL )
	{
		printf("Xml First Child is Null !\n");
		return -1;
	}

	const std::string& tFormName = tpRoot->ValueStr();
	const std::string& tPBName = tpMsgXmlNode->ValueStr();// 得到 PB 的名称字符串
	const char* tpFieldName = tpMsgXmlNode->Attribute("DataBase_Field");
	const char* tpPrimaryName = tpMsgXmlNode->Attribute("PrimaryName");
	const char* tpPrimaryKeyVal = tpMsgXmlNode->Attribute("PrimaryKeyVal");
	if ( tpFieldName == NULL || tpPrimaryName == NULL || tpPrimaryKeyVal == NULL )
	{
		return -1;
	}

	TiXmlElement* tpPB = NULL;
	Message* pRoot = NULL;
	const char* tpFormName = tFormName.c_str();

	// 根据数据库表名 tpFormName，数据库字段名 tpFieldName 
	// 得到根结点 PB 对象 pRoot 和字段对应 PB 对象 pMessage
	Message* pMessage = GetMessage(pRoot, tpFormName, tpFieldName);

	// 根据 tpEachXmlNode 的结点信息，填充到数据库字段对应的 PB 中
	//XmlToProbuff( tpMsgXmlNode, pMessage );
	CProtoXMLManager::GetSingleton().Xml2Message( pMessage, pMessage->GetReflection(), tpMsgXmlNode );

	int tRet = SerializeToDataBase( pMessage, tFormName.c_str(), tpFieldName, tpPrimaryName, tpPrimaryKeyVal );

	if ( tRet == 0 )
	{
		printf("********************************************************************\n");
		printf("*** Write XML To PB Blob Data Success ! ***\n" );
		printf("********************************************************************\n");
	}

	return tRet;
}

// 把xml写回到数据库
int CParseProtobuf::SerializeToDataBase( Message* pMessage, const char* pFormName, const char* pFieldName, const char* pPrimaryName, const char* pPrimaryKeyVal )
{
	if ( pMessage == NULL || pFormName == NULL || pFieldName == NULL || pPrimaryName == NULL || pPrimaryKeyVal == NULL )
	{
		return -1;
	}

	bool tRet = false;
	char tBuff[10240] = { 0 };// 每个PB的大小不一样......
	char tEscapeBuff[10240] = { 0 };// EscapeDataString() 处理转义字符后的字符

	tRet = pMessage->SerializeToArray( tBuff, sizeof(tBuff) - 1 );
	if ( !tRet )
	{
		printf("Serialize PB Failed !\n");
		return -1;
	}

	std::string tDebugSerializeStr = pMessage->DebugString();
	// printf( "Debug Serialize String : %s\n", tDebugSerializeStr.c_str() );

	char* tpEscapePtr = (char*)tEscapeBuff;
	int tEscapeRet = EscapeDataString( &tpEscapePtr, sizeof(tEscapeBuff) - 1, tBuff, pMessage->ByteSize() );
	if ( tEscapeRet == -1 )
	{
		printf("EscapeDataString PB Failed !\n");
		return -1;
	}

	std::string tQueryStr("update ");
	tQueryStr.append( pFormName );
	tQueryStr.append(" set ");
	tQueryStr.append( pFieldName );
	tQueryStr.append(" = '");
	tQueryStr.append( tEscapeBuff );
	tQueryStr.append("' where ");
	tQueryStr.append( pPrimaryName );
	tQueryStr.append(" = ");
	tQueryStr.append( pPrimaryKeyVal );

	//char* tpSql = NULL;
	//sprintf( tpSql, "update %s set %s = '%s' where %s = %s", tFormName.c_str(), tpFieldName, tEscapeBuff, tpPrimaryName, tpPrimaryKeyVal );
	tRet = mMysql.DirectExecute( tQueryStr.c_str() );
	if ( !tRet )
	{
		printf("Update Form : %s Failed !\n", pFormName );
		return -1;
	}

	return 0;
}

// 根据配置解析
int CParseProtobuf::ParseByConfig()
{
	TiXmlDocument tXmlDoc;
	bool bLoad = tXmlDoc.LoadFile( "Config.xml" );
	if ( !bLoad )
	{
		printf("Load XML File Failed !\n");
		return -1;
	}
	
	TiXmlElement* tpRoot = tXmlDoc.FirstChildElement();
	if ( tpRoot == NULL )
	{
		return -1;
	}

	const char* tpIP = tpRoot->Attribute("IP");
	const char* tpPort = tpRoot->Attribute("Port");
	const char* tpUser = tpRoot->Attribute("User");
	const char* tpPasswd = tpRoot->Attribute("Passwd");
	const char* tpDBName = tpRoot->Attribute("DBName");
	const char* tFormName = tpRoot->Attribute("TableName");
	const char* tFieldName = tpRoot->Attribute("FieldName");

	if ( tpIP == NULL || tpPort == NULL || tpUser == NULL || tpPasswd == NULL || tpDBName == NULL || tFormName == NULL || tFieldName == NULL )
	{
		printf("Config.xml Read Error !\n");
		return -1;
	}

	bool bResult = InitializeMysql( tpIP, atoi(tpPort), tpUser, tpPasswd, tpDBName );
	if ( !bResult )
	{
		return -1;
	}

	std::ifstream fin("primarykey.txt");
	if ( !fin )
	{
		std::cout<<"Open primarykey.txt Failed !"<<std::endl;
	}

	std::vector<std::string> tParseFailedStrVec;

	std::string tRoleIDStr("");
	while( getline(fin, tRoleIDStr) )
	{
		int tResult = ParseCore( tFormName, tFieldName, tRoleIDStr.c_str(), false );
		if ( tResult != 0 )
		{
			std::cout<<"Parse : "<<tFormName<<" "<<tFieldName<<" "<<tRoleIDStr<<"Failed ."<<std::endl;
			continue;
		}

		std::string strPath("./xmlfiles/");
		strPath.append( tFormName );
		strPath.append( "/" );
		strPath.append( tRoleIDStr.c_str() );
		strPath.append( "/" );
		strPath.append( tFormName );
		strPath.append( "." );
		strPath.append( tFieldName );
		strPath.append( ".xml" );

		PBToolCommon::CopyFile( strPath.c_str() );
	}

	// 单独输出解析失败的角色ID
	if ( tParseFailedStrVec.size() > 0 )
	{
		std::cout<<"These Role Account Parse Blob To xml Data Falied :"<<std::endl;
		for (int i = 0; i < tParseFailedStrVec.size(); ++i)
		{
			std::cout<<tParseFailedStrVec[i]<<std::endl;
		}
		std::cout<<"Please find the cause of problem ."<<std::endl;
	}

	return 0;
}

// 根据配置解析
int CParseProtobuf::WriteByConfig()
{
	TiXmlDocument tXmlDoc;
	bool bLoad = tXmlDoc.LoadFile( "Config.xml" );
	if ( !bLoad )
	{
		printf("Load XML File Failed !\n");
		return -1;
	}

	TiXmlElement* tpRoot = tXmlDoc.FirstChildElement();
	if ( tpRoot == NULL )
	{
		return -1;
	}

	const char* tpIP = tpRoot->Attribute("IP");
	const char* tpPort = tpRoot->Attribute("Port");
	const char* tpUser = tpRoot->Attribute("User");
	const char* tpPasswd = tpRoot->Attribute("Passwd");
	const char* tpDBName = tpRoot->Attribute("DBName");
	const char* tFormName = tpRoot->Attribute("TableName");
	const char* tFieldName = tpRoot->Attribute("FieldName");

	if ( tpIP == NULL || tpPort == NULL || tpUser == NULL || tpPasswd == NULL || tpDBName == NULL || tFormName == NULL || tFieldName == NULL )
	{
		printf("Config.xml Read Error !\n");
		return -1;
	}

	bool bResult = InitializeMysql( tpIP, atoi(tpPort), tpUser, tpPasswd, tpDBName );
	if ( !bResult )
	{
		return -1;
	}

	std::ifstream fin("primarykey.txt");
	if ( !fin )
	{
		std::cout<<"Open primarykey.txt Failed !"<<std::endl;
	}

	std::vector<std::string> tWriteFailedStrVec;

	std::string tRoleIDStr("");
	while( getline(fin, tRoleIDStr) )
	{
		int tResult = CreatePBCore( tFormName, tFieldName, tRoleIDStr.c_str() );
		if ( tResult != 0 )
		{
			std::cout<<"Write : "<<tFormName<<" "<<tFieldName<<" "<<tRoleIDStr<<"Failed ."<<std::endl;
			tWriteFailedStrVec.push_back( tRoleIDStr );
			continue;
		}
	}

	// 单独输出写入失败的角色ID
	if ( tWriteFailedStrVec.size() > 0 )
	{
		std::cout<<"These Role Account Write xml Data To Blob Falied :"<<std::endl;
		for (int i = 0; i < tWriteFailedStrVec.size(); ++i)
		{
			std::cout<<tWriteFailedStrVec[i]<<std::endl;
		}
		std::cout<<"Please find the cause of problem ."<<std::endl;
	}

	return 0;
}