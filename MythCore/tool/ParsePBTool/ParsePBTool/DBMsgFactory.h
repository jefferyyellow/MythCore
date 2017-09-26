#ifndef _DB_MSG_FACTORY_H_
#define _DB_MSG_FACTORY_H_

#include "PBToolCommon.h"
#include "ProtoXMLManager.h"
#include "sk_hashmap.h"

#include "tinyxml.h"
#include "tinystr.h"

#include "google/protobuf/descriptor.h"
#include "google/protobuf/descriptor.pb.h"
// 要解析的各种PB文件
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
// 所有的 PB 的索引
enum emPBIndex
{
	emPBListNone = 0,
	// GAME_EVENT
	emPBEventParam,
	// UMS_APPRENTICE
	emPBApprentice,
	// UMS_FACTION：
	emPBMemberList,
	emPBFactionApplyList,
	// UMS_ROLE_INFO：
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



// Xml 结点类，用来解析成 xml 文件用
/*
	xml 结构示例：

	<?xml version="1.0" encoding="gb2312" standalone="yes" ?>     → xml文件头结点 mpXmlDoc 
	<UMS_GLOBAL_DATA>     → 表名作为头结点 mpRoot
		<PBFactionGlobal DataBase_Field="faction_data" PrimaryName="global_id" PrimaryKeyVal="1"> → 头结点下的第一个结点 mFirstNode
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
	// 初始化结点信息，包括xml文件头和表头
	int Initialize(const char* pQueryStr, const char* pFormName, const char* pPrimaryKeyVal );
	// 清除所有结点
	void ClearNodes();

public:
	// 获取xml文件头结点
	TiXmlDocument* GetXmlDoc() { return mpXmlDoc; }
	// 获取表头xml结点
	TiXmlElement* GetXmlRoot() { return mpRoot; }
	// 获取需要查询的字段名
	const char* GetQueryStr() { return mpQueryStr; }
	// 获取主键名
	const char* GetPrimaryName() { return mpPrimaryName; }
	// 获取主键值
	const char* GetPrimaryKeyVal() { return mpPrimaryKeyVal; }
	// 获取表名
	const char* GetFormName() { return mpFormName; }

private:
	// 主键名
	const char* mpPrimaryName;

	// 主键值
	const char* mpPrimaryKeyVal;

	// 表名
	const char* mpFormName;

	// 需要查询的字段名
	const char* mpQueryStr;

private:
	// xml文件头结点
	TiXmlDocument* mpXmlDoc;
	// 表名作为头结点
	TiXmlElement* mpRoot;
};


// 解析器，集创建PB字典和反向的解析一体
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

	// 参数含义：要解析的PB序号、二进制字符串流、二进制字符串长度
	TiXmlElement* PB2Xml(CXmlNode* pNodeInfo, const char* pFieldStr, int pFieldLen);

private:
	// 只建立一次
	void CreateDBField2PBMap();

private:
	// 【数据库字段】→【PB枚举】映射（PB2Xml）
	DBField2PB_HashMap mDBField2PBMap;
};

#endif
