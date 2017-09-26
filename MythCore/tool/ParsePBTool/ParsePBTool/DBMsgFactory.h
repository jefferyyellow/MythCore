#ifndef _DB_MSG_FACTORY_H_
#define _DB_MSG_FACTORY_H_

#include "PBToolCommon.h"
#include "ProtoXMLManager.h"
#include "sk_hashmap.h"

#include "tinyxml.h"
#include "tinystr.h"

#include "google/protobuf/descriptor.h"
#include "google/protobuf/descriptor.pb.h"
// Ҫ�����ĸ���PB�ļ�
#include "db_msg_pb.hxx.pb.h"
#include "property_pb.hxx.pb.h"
#include "servermessage_pb.hxx.pb.h"
#include "arenamessage_pb.hxx.pb.h"
#include "activitymessag_pb.hxx.pb.h"
#include "factionmessage_pb.hxx.pb.h"

typedef ::google::protobuf::Message Message;
typedef ::google::protobuf::Descriptor Descriptor;
typedef ::google::protobuf::Reflection Reflection;
typedef ::google::protobuf::FieldDescriptor FieldDescriptor;
typedef ::google::protobuf::EnumValueDescriptor EnumValueDescriptor;

Message* GetMessage(Message*& pRoot, const char* pTableName, const char* pFieldName);
Message* GetSubMessage(Message*& pRoot, const char* pFieldName);
// ���е� PB ������
enum emPBIndex
{
	emPBListNone = 0,
	// GAME_EVENT
	emPBEventParam,
	// UMS_APPRENTICE
	emPBApprentice,
	// UMS_FACTION��
	emPBMemberList,
	emPBFactionApplyList,
	// UMS_ROLE_INFO��
	emPBPlayerBasicInfo,
	emPBTaskList,
	emPBSkillList,
	emPBBuffList,
	emPBPlayerHorseInfo,
	emPBItemBox,
	emPBItemBox_Package,//
	//emPBItemBox_Equipment,
	emPBItemBox_Storage,
	emPBPlayerTitleInfo,
	emPBActivity,
	// role_task_package ???
	emPBPlayerArray,
	emPBPartnerInfo,
	emPBHeartMagicInfo,
	emPBPlayerAllInfo,
	emPBArenaData,
	emPBPlayerData,
	emPBArenaHistoryData,
	emPBItemBox_Gem_Package,
	emPBPlayerArtifactData,
	// UMS_GLOBAL_DATA : 
	emPBArenaPlaceTrend,
	emPBOldArenaPlace,
	emPBMultiFBRandInvitePlayer,
	emPBMysticShopSellLog,
	emPBGlobalData,
	emPBFactionGlobal,
	emPBJXZGlobalRank,
	emPBGlobalLevelRank,
	emPBWorldWarGlobal,
	emPBPlayerHorseRank,
	emPBGlobalActivityStatus,
	// UMS_WORLDWAR
	emPBWorldWarDB,
	emPBWorldWarDB_1,
	emPBWorldWarDB_2,
	emPBWorldWarDB_3,
	emPBWorldWarDB_4,
	emPBWorldWarDB_5,
	emPBWorldWarDB_6,
	emPBWorldWarDB_7,
	emPBWorldWarDB_8,
	// UMS_WORLDWAR_ROLE
	emPBWorldWarRole,

	emPBMax
};



// Xml ����࣬���������� xml �ļ���
/*
	xml �ṹʾ����

	<?xml version="1.0" encoding="gb2312" standalone="yes" ?>     �� xml�ļ�ͷ��� mpXmlDoc 
	<UMS_GLOBAL_DATA>     �� ������Ϊͷ��� mpRoot
		<PBFactionGlobal DataBase_Field="faction_data" PrimaryName="global_id" PrimaryKeyVal="1"> �� ͷ����µĵ�һ����� mFirstNode
	</UMS_GLOBAL_DATA>
*/
class CXmlNode
{
public:
	CXmlNode() : 
	  mpPrimaryName(""), mpPrimaryKeyVal(""), mpFormName(""), mpQueryStr(""),
	  mpXmlDoc(NULL), mpRoot(NULL) {}
	  ~CXmlNode() {}

public:
	// ��ʼ�������Ϣ������xml�ļ�ͷ�ͱ�ͷ
	int Initialize(const char* pQueryStr, const char* pFormName, const char* pPrimaryKeyVal );
	// ������н��
	void ClearNodes();

public:
	// ��ȡxml�ļ�ͷ���
	TiXmlDocument* GetXmlDoc() { return mpXmlDoc; }
	// ��ȡ��ͷxml���
	TiXmlElement* GetXmlRoot() { return mpRoot; }
	// ��ȡ��Ҫ��ѯ���ֶ���
	const char* GetQueryStr() { return mpQueryStr; }
	// ��ȡ������
	const char* GetPrimaryName() { return mpPrimaryName; }
	// ��ȡ����ֵ
	const char* GetPrimaryKeyVal() { return mpPrimaryKeyVal; }
	// ��ȡ����
	const char* GetFormName() { return mpFormName; }

private:
	// ������
	const char* mpPrimaryName;

	// ����ֵ
	const char* mpPrimaryKeyVal;

	// ����
	const char* mpFormName;

	// ��Ҫ��ѯ���ֶ���
	const char* mpQueryStr;

private:
	// xml�ļ�ͷ���
	TiXmlDocument* mpXmlDoc;
	// ������Ϊͷ���
	TiXmlElement* mpRoot;
};


// ��������������PB�ֵ�ͷ���Ľ���һ��
class CParser
{
public:
	typedef sk::hash_map<std::string, int, emPBMax> DBField2PB_HashMap;

public:
	CParser()
	{
		Initialize();
	}
	~CParser() {}

public:
	int Initialize()
	{
		CreateDBField2PBMap();
		return 0;
	}

	// �������壺Ҫ������PB��š��������ַ��������������ַ�������
	TiXmlElement* PB2Xml(CXmlNode* pNodeInfo, const char* pFieldStr, int pFieldLen);

private:
	// ֻ����һ��
	void CreateDBField2PBMap();

private:
	// �����ݿ��ֶΡ�����PBö�١�ӳ�䣨PB2Xml��
	DBField2PB_HashMap mDBField2PBMap;
};

#endif
