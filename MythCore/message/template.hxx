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

}

message PBTplConfigSet
{
	repeated PBTplPlayerLevelExpConfig	LevelExpConfig			= 1;
	repeated PBTplVIPConfig				VIPConfig				= 2;	

}

message PBTplSkillSet
{
	
}

message PBTplNPCSet
{
	repeated PBTplFuncNPC				FuncNpc					= 1;
	repeated PBTplOgre					Ogre					= 2;
}

// 道具基本信息
message PBTplItem
{
	uint32				TempID			= 1;	// 模板ID
	string				Name			= 2;	// 道具显示名称
	string				Describe		= 3;	// 物品详细说明
	uint32				PileLimit		= 4;	// 堆叠上限
}

message PBTplPlayerLevelExpConfig
{
	repeated uint32		LevelUpExp		= 1;	// 升到下一级经验值
}

message PBTplVIPConfig
{
	repeated uint32		VIPExp			= 1;	// VIP升到下一级经验值
}

// NPC基本信息
message PBTplNPC
{
	uint32				TempID			= 1;	// 模板ID
	uint32				Name			= 2;	// NPC名字
}

// 功能NPC
message	PBTplFuncNPC
{
	PBTplNPC			BaseInfo		= 1;	// NPC基本信息
}

// 怪物NPC
message PBTplOgre
{
	PBTplNPC			BaseInfo		= 1;	// NPC基本信息
}