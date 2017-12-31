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
	repeated PBEquip					equip					= 1;		// 装备
}

message PBTplConfigSet
{
	repeated PBTplLevelExpConfig		LevelExpConfig			= 1;
	repeated PBTplVIPConfig				VIPConfig				= 2;	
	repeated PBTplDropTable				Droptable				= 3;
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

// 道具基本信息
message PBItem
{
	uint32				TempID			= 1;	// 模板ID
	string				Name			= 2;	// 道具显示名称
	string				Describe		= 3;	// 物品详细说明
	uint32				ItemType		= 4;	// 道具类型
	uint32				PileLimit		= 5;	// 堆叠上限
}
message PBEquip
{
	PBItem				Super			= 1;
	string				EquipModal		= 2;
	uint32				EquipPart		= 3;
	uint32				LevelRequire	= 4;
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
	uint32	TempID = 1;
	string	Name = 2;
	string	Description = 3;
	uint32	SkillCD = 4;
	uint32	AddDamage = 5;
}


message PBTplDropItem
{
	uint32	ItemID = 1;
	uint32	ItemNum = 2;
	uint32	Probability = 3;
}

message PBTplDropTable
{
	uint32	TempID = 1;
	repeated	PBTplDropItem	DropItem = 2;
}
