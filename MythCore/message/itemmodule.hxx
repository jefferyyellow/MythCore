syntax = "proto3";

// 消息ID范围（1-65535），共16位，前6位表示模块号，后10位表示模块内的序号，一个模块最多只有1024个消息

enum ITEM_MODULE_MSG_ID
{
	ID_ITEM_MODULE_ERROR					= 0x0;
	ID_S2C_NOTIYF_PLAYER_MONEY_UPDATE		= 0x0800;	// 服务器->客户端 玩家金钱更新通知
	ID_S2C_NOTIYF_PLAYER_DIAMOND_UPDATE		= 0x0801;	// 服务器->客户端 玩家
};

// 玩家金钱更新通知
message CMessagePlayerMoneyUpdateNotify
{
	uint32	Money				= 1;		// 玩家当前的金钱
}

// 玩家钻石更新通知
message CMessagePlayerDiamondUpdateNotify
{
	uint32	Diamond				= 1;		// 玩家当前的钻石
}


// 插入道具通知
message CMessageInsertItemNotify
{
	uint32 ItemID				= 1;		// 物品ID
	uint32 Index				= 2;		// 物品索引
	uint32 Number				= 3;		// 物品数目
}

// 插入属性道具
message CMessageInsertItemNotify
{
	uint32 ItemID				= 1;		// 道具ID
	uint32 Index				= 2;		// 道具在包裹中的索引
}