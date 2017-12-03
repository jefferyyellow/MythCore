syntax = "proto3";
import "common.hxx";
// 消息ID范围（1-65535），共16位，前6位表示模块号，后10位表示模块内的序号，一个模块最多只有1024个消息

enum ITEM_MODULE_MSG_ID
{
	ID_ITEM_MODULE_ERROR					= 0x0;
	ID_S2C_NOTIYF_MONEY_UPDATE				= 0x0800;	// 玩家金钱更新通知
	ID_S2C_NOTIYF_DIAMOND_UPDATE			= 0x0801;	// 玩家钻石更新通知
	ID_S2C_NOTIYF_INSERT_ITEM				= 0x0802;	// 插入道具通知(没有特殊属性的道具)
	ID_S2C_NOTIYF_INSERT_ITEM_OBJ			= 0x0803;	// 插入道具通知(有特殊属性的道具)
	ID_S2C_NOTIYF_REMOVE_ITEM				= 0x0804;	// 删除道具通知
	ID_C2S_REQUEST_USE_ITEM					= 0x0805;	// 使用道具请求
	ID_S2C_RESPONSE_USE_ITEM				= 0x0806;	// 使用道具回应
	ID_C2S_REQUEST_SELL_ITEM				= 0x0807;	// 卖出道具请求
	ID_S2C_RESPONSE_SELL_ITEM				= 0x0808;	// 卖出道具回应
	ID_C2S_REQUEST_PURCHASE_ITEM			= 0x0809;	// 购买道具请求
	ID_S2C_RESPONSE_PURCHASE_ITEM			= 0x080A;	// 购买道具回应
};

// 玩家金钱更新通知 ID_S2C_NOTIYF_MONEY_UPDATE
message CMoneyUpdateNotify
{
	uint32	Money				= 1;		// 玩家当前的金钱
}

// 玩家钻石更新通知 ID_S2C_NOTIYF_DIAMOND_UPDATE
message CDiamondUpdateNotify
{
	uint32	Diamond				= 1;		// 玩家当前的钻石
}

// 插入道具通知(没有特殊属性的道具) ID_S2C_NOTIYF_INSERT_ITEM
message CInsertItemNotify
{
			 uint32 ItemID		= 1;		// 物品ID
	repeated uint32 Index		= 2;		// 物品索引
	repeated uint32 Number		= 3;		// 物品数目
}

// 插入道具通知(有特殊属性的道具) ID_S2C_NOTIYF_INSERT_ITEM_OBJ
message CInsertItemObjNotify
{
	PBItemObject	ItemObject	= 1;		// 装备
}

// 删除道具通知 ID_S2C_NOTIYF_REMOVE_ITEM
message CRemoveItemNotify
{
	uint32 Index		= 1; 		// 物品索引
	uint32 Number		= 2; 		// 物品数目
}

// 使用道具请求 ID_C2S_REQUEST_USE_ITEM
message CUseItemRequest
{
	uint32 Index		= 1; 		// 物品索引
	uint32 Number		= 2; 		// 物品数目
}

// 使用道具回应 ID_S2C_RESPONSE_USE_ITEM
message CUseItemResponse
{
	uint32 Result		= 1;		// 结果
}

// 卖出道具请求 ID_C2S_REQUEST_SELL_ITEM
message CSellItemRequest
{
	uint32 Index		= 1;		// 物品索引
}

// 卖出道具回应 ID_S2C_RESPONSE_SELL_ITEM
message CSellItemResponse
{
	uint32 Result		= 1;		// 结果
}

// 购买道具请求 ID_C2S_REQUEST_PURCHASE_ITEM
message CPurchaseItemRequest
{
	uint32 Index		= 1;		// 索引
	uint32 Num			= 2;		// 数目
}

// 购买道具回应 ID_S2C_RESPONSE_PURCHASE_ITEM
message CPurchaseItemResponse
{
	uint32 Result		= 1;		// 结果
}