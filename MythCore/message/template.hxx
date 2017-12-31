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
	repeated PBEquip					equip					= 1;		// װ��
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

// ���߻�����Ϣ
message PBItem
{
	uint32				TempID			= 1;	// ģ��ID
	string				Name			= 2;	// ������ʾ����
	string				Describe		= 3;	// ��Ʒ��ϸ˵��
	uint32				ItemType		= 4;	// ��������
	uint32				PileLimit		= 5;	// �ѵ�����
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
	PBTplNPC			Super			= 1;	// NPC������Ϣ
	uint32				Exp				= 2;	// ����
	repeated uint32		DropTable		= 3;	// �����
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
