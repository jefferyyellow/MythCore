syntax = "proto3";

// ��ҽ�Ǯ����֪ͨ
message PBTplTemplate
{
	PBTplItemSet		ItemSet			= 1;		// ���߼���
	PBTplConfigSet		ConfigSet		= 2;		// ���ü���
	PBTplSkillSet		SkillSet		= 3;		// ���ܼ���
	PBTplNPCSet			NPCSet			= 4;		// NPC����
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

// ���߻�����Ϣ
message PBTplItem
{
	uint32				TempID			= 1;	// ģ��ID
	string				Name			= 2;	// ������ʾ����
	string				Describe		= 3;	// ��Ʒ��ϸ˵��
	uint32				ItemType		= 4;	// ��������
	uint32				PileLimit		= 5;	// �ѵ�����
}

message PBTplPlayerLevelExpConfig
{
	uint32				TempID			= 1;	// ģ��ID
	repeated uint32		LevelUpExp		= 2;	// ������һ������ֵ
}

message PBTplVIPConfig
{
	uint32				TempID			= 1;	// ģ��ID
	repeated uint32		VIPExp			= 2;	// VIP������һ������ֵ
}

// NPC������Ϣ
message PBTplNPC
{
	uint32				TempID			= 1;	// ģ��ID
	string				Name			= 2;	// NPC����
}

// ����NPC
message	PBTplFuncNPC
{
	PBTplNPC			BaseInfo		= 1;	// NPC������Ϣ
}

// ����NPC
message PBTplOgre
{
	PBTplNPC			BaseInfo		= 1;	// NPC������Ϣ
}