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
	// CreateForm2FieldMapping();// ��������������Blob�ֶ������ϡ���ӳ��
}

// ��ʼ�����ݿ�����
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
	printf("------------------------------------------------- �������ݿ�ɹ� ! -------------------------------------------------\n");
	printf("*********************************************************************************************************************\n");
	printf("\n");
	mbMySql = true;
	return true;
}

// ��ĳһ�������л�������ݿ�
bool CParseProtobuf::SwitchOtherDataBase(const char* pDataBase)
{
	if ( pDataBase == NULL )
	{
		return false;
	}
	return mMysql.SwitchDataBase(pDataBase);
}

// ��������������Blob�ֶ������ϡ���ӳ��
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

// ͨ�ý�����bAll Ϊtrue��ʾ����ĳ�������µ������ֶ�PB����ʱ pFieldName �������ã�
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

	Tokens tQueryFields;// ��Ҫ��ѯ���ֶ��б�
	// ƴ��ѯ�ַ���
	std::string tQueryString("select ");

	if ( bAll )// һ������
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

	// ����������
	CParser tParser;

	for(int iRow = 0; iRow < nRow; ++iRow)
	{
		Field* pField = pResult->GetCurrentRow();

		for (int iCol = 0; iCol < nColumn ; ++iCol)
		{
			// ������ Blob ���͵��ֶβ��������
			if ( pField[iCol].GetDataType() != emDataType_Blob )
			{
				printf("The Field is not a Blob Field, The Tool can't Parse . \n");
				return -1;
			}

			// ��ʼ������������
			CXmlNode tXmlNode;
			int tRet = tXmlNode.Initialize( tQueryFields[iCol].c_str(), pFormName, pPrimaryKeyVal );
			if ( tRet != 0 )
			{
				return -1;
			}

			// ���ݶ�Ӧ PB ö�ٽ��н���
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

// ��xmlд�ص����ݿ�
int CParseProtobuf::SerializeToArray( const char* pName )
{
	return 0;
}

// �����ɾ��
void CParseProtobuf::ClearNodes( TiXmlDocument* tpXmlDoc )
{
	if ( tpXmlDoc != NULL )
	{
		tpXmlDoc->Clear();
		delete tpXmlDoc;
		tpXmlDoc = NULL;
	}
}

// ���� xml ������������Ӧ�� PB ����
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

	TiXmlElement* tpMsgXmlNode = tpRoot->FirstChildElement();// <UMS_��>�µ��ֶα�ǩ���
	if ( tpMsgXmlNode == NULL )
	{
		printf("Xml First Child is Null !\n");
		return -1;
	}

	const std::string& tFormName = tpRoot->ValueStr();
	const std::string& tPBName = tpMsgXmlNode->ValueStr();// �õ� PB �������ַ���
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

	// �������ݿ���� tpFormName�����ݿ��ֶ��� tpFieldName 
	// �õ������ PB ���� pRoot ���ֶζ�Ӧ PB ���� pMessage
	Message* pMessage = GetMessage(pRoot, tpFormName, tpFieldName);

	// ���� tpEachXmlNode �Ľ����Ϣ����䵽���ݿ��ֶζ�Ӧ�� PB ��
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

// ��xmlд�ص����ݿ�
int CParseProtobuf::SerializeToDataBase( Message* pMessage, const char* pFormName, const char* pFieldName, const char* pPrimaryName, const char* pPrimaryKeyVal )
{
	if ( pMessage == NULL || pFormName == NULL || pFieldName == NULL || pPrimaryName == NULL || pPrimaryKeyVal == NULL )
	{
		return -1;
	}

	bool tRet = false;
	char tBuff[10240] = { 0 };// ÿ��PB�Ĵ�С��һ��......
	char tEscapeBuff[10240] = { 0 };// EscapeDataString() ����ת���ַ�����ַ�

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

// �������ý���
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

	// �����������ʧ�ܵĽ�ɫID
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

// �������ý���
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

	// �������д��ʧ�ܵĽ�ɫID
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