syntax = "proto3";

// 玩家金钱更新通知
message PBTplTemplate
{
	PBTplItemSet		ItemSet			= 1;		// 道具集合
	PBTplConfigSet		ConfigSet		= 2;		// 配置集合
	PBTplSkillSet		SkillSet		= 3;		// 技能集合
	PBTplNPCSet			NPCSet			= 4;		// NPC集合
}

message PBTplItemSet
{
	repeated PBTplEquip					equip					= 1;		// 装备
}

message PBTplConfigSet
{
	repeated PBTplLevelExpConfig		LevelExpConfig			= 1;
	repeated PBTplVIPConfig				VIPConfig				= 2;	
	repeated PBTplDropTable				Droptable				= 3;
	repeated PBTplNewPlayerConfig		NewPlayerConfig			= 4;
}

message PBTplSkillSet
{
	repeated PBTplSkill					Skill					= 1;
}

message PBTplNPCSet
{
	repeated PBTplFuncNPC				FuncNpc					= 1;
	repeated PBTplOgre					Ogre					= 2;
}

// 新玩家信息
message PBTplNewPlayerConfig
{
				uint32	TempID = 1;
	repeated	uint32	ItemID = 2;
	repeated	uint32	ItemNum = 3;
				uint32	Level = 4;
				uint32	VipLevel = 5;
}

// 道具基本信息
message PBItem
{
	uint32				TempID			= 1;	// 模板ID
	string				Name			= 2;	// 道具显示名称
	string				Describe		= 3;	// 物品详细说明
	uint32				ItemType		= 4;	// 道具类型
	uint32				PileLimit		= 5;	// 堆叠上限
}

message PBTplProperty
{
	uint32				Type	= 1;
	uint32				Value	= 2;
}

message PBTplEquip
{
	PBItem				Super			= 1;
	string				EquipModal		= 2;
	uint32				EquipPart		= 3;
	uint32				LevelRequire	= 4;
	repeated PBTplProperty Property		= 5;
}

message PBTplLevelExpConfig
{
	uint32				TempID			= 1;	// 模板ID
	repeated uint32		LevelUpExp		= 2;	// 升到下一级经验值
}

message PBTplVIPConfig
{
	uint32				TempID			= 1;	// 模板ID
	repeated uint32		VIPExp			= 2;	// VIP升到下一级经验值
}

// NPC基本信息
message PBTplNPC
{
	uint32				TempID			= 1;	// 模板ID
	string				Name			= 2;	// NPC名字
}

// 功能NPC
message	PBTplFuncNPC
{
	PBTplNPC			BaseInfo		= 1;	// NPC基本信息
}

// 怪物NPC
message PBTplOgre
{
	PBTplNPC			Super			= 1;	// NPC基本信息
	uint32				Exp				= 2;	// 经验
	repeated uint32		DropTable		= 3;	// 掉落表
}

message PBTplSkill
{
	uint32				TempID			= 1;
	string				Name			= 2;
	string				Description		= 3;
	uint32				SkillCD			= 4;
	uint32				AddDamage		= 5;
}


message PBTplDropItem
{
	uint32					ItemID		= 1;		// 掉落道具ID
	uint32					ItemNum		= 2;		// 掉落道具数量
	uint32					Probability = 3;		// 掉落道具概率
}

message PBTplDropTable
{
	uint32					TempID		= 1;		// 掉落表ID
	repeated PBTplDropItem	DropItem	= 2;		// 掉落道具
}

message PBChatChannelLimit
{
	uint32					WordNum		= 1;		// 频道字数限制
	uint32					PlayerLevel	= 2;		// 频道发言等级
	uint32					Interval	= 3;		// 频道时间间隔
}

message PBTplChatConfig
{
	PBChatChannelLimit		WorldLimit	= 1;		// 世界频道
	PBChatChannelLimit		TeamLimit	= 2;		// 组队频道
	PBChatChannelLimit		FactionLimit= 3;		// 公会频道
}