syntax = "proto3";

message PBItemObject
{
	uint32			ItemID			= 1;		// ID
	uint32			Number			= 2;		// 数量
	uint32			Index			= 3;		// 包裹中的索引

	PBItemEquip		ItemEquip		= 4;		// 装备
}

message PBItemEquip
{
	uint32			Level		= 4;			// 等级
}

