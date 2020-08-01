syntax = "proto3";
import "common.hxx";
// 消息ID范围（1-65535），共16位，前6位表示模块号，后10位表示模块内的序号，一个模块最多只有1024个消息

enum ITEM_MODULE_MSG_ID
{
	ID_ITEM_MODULE_ERROR					= 0x0;
	ID_S2C_NOTIYF_CURRENCY_UPDATE			= 0x0800;	// 玩家货币更新通知
	ID_S2C_NOTIYF_INSERT_ITEM				= 0x0802;	// 插入道具通知(没有特殊属性的道具)
	ID_S2C_NOTIYF_INSERT_ITEM_OBJ			= 0x0803;	// 插入道具通知(有特殊属性的道具)
	ID_S2C_NOTIYF_REMOVE_ITEM				= 0x0804;	// 删除道具通知
	ID_C2S_REQUEST_USE_ITEM					= 0x0805;	// 使用道具请求
	ID_S2C_RESPONSE_USE_ITEM				= 0x0806;	// 使用道具回应
	ID_C2S_REQUEST_SELL_ITEM				= 0x0807;	// 卖出道具请求
	ID_S2C_RESPONSE_SELL_ITEM				= 0x0808;	// 卖出道具回应
	ID_C2S_REQUEST_PURCHASE_ITEM			= 0x0809;	// 购买道具请求
	ID_S2C_RESPONSE_PURCHASE_ITEM			= 0x080A;	// 购买道具回应
	ID_C2S_REQUEST_EQUIP_ITEM				= 0x080B;	// 装备道具的请求
	ID_S2C_RESPONSE_EQUIP_ITEM				= 0x080C;	// 装备道具的回应
	ID_C2S_REQUEST_UNEQUIP_ITEM				= 0x080D;	// 卸载装备的请求
	ID_S2C_RESPONSE_UNEQUIP_ITEM			= 0x080E;	// 卸载装备的回应
	ID_S2C_NOTIYF_EQUIP_CHANGE				= 0x080F;	// 装备改变的通知
	ID_C2S_REQUEST_GET_SHOP_INFO			= 0x0810;	// 得到商店信息的请求
	ID_S2C_RESPONSE_GET_SHOP_INFO			= 0x0811;	// 得到商店信息的回应
	ID_C2S_REQUEST_PICK_ITEM				= 0x0812;	// 拾取道具的请求
	ID_S2C_RESPONSE_PICK_ITEM				= 0x0813;	// 拾取道具的回应

};

// 玩家货币更新通知 ID_S2C_NOTIYF_CURRENCY_UPDATE
message CCurrencyUpdateNotify
{
	uint32	CurrencyType		= 1;		// 货币类型
	uint32	CurrencyNum			= 2;		// 该类型玩家的货币数目
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
	uint32 ShopType		= 1;		// 商店类型
	uint32 Index		= 2;		// 索引
	uint32 Num			= 3;		// 数目
}

// 购买道具回应 ID_S2C_RESPONSE_PURCHASE_ITEM
message CPurchaseItemResponse
{
	uint32 Result		= 1;		// 结果
}

// 装备道具的请求 ID_C2S_REQUEST_EQUIP_ITEM
message CEquipItemRequest
{
	uint32	ItemIndex	= 1;		// 装备的道具
}

// 装备道具的回应 ID_S2C_RESPONSE_EQUIP_ITEM
message CEquipItemResponse
{
	uint32	Result		= 1;		// 结果	
	uint32	ItemIndex	= 2;		// 装备的道具
}

// 卸载装备的请求 ID_C2S_REQUEST_UNEQUIP_ITEM
message CUnEquipItemRequest
{
	uint32	EquipPart	= 1;		// 装备部位
	uint32	ItemIndex	= 2;		// 装备在包裹里的索引
}

// 卸载装备的回应 ID_S2C_RESPONSE_UNEQUIP_ITEM
message CUnEquipItemResponse
{
	uint32	Result		= 1;		// 结果
	uint32	EquipPart	= 2;		// 装备部位
	uint32	ItemIndex	= 3;		// 装备在包裹里的索引
}

// 装备改变通知 ID_S2C_NOTIYF_EQUIP_CHANGE
message CEquipChangeNotify
{
	uint32	EntityID	= 1;		// 角色ID
	uint32	EquipPart	= 2;		// 装备部位
	uint32	EquipItemID	= 3;		// 装备道具ID
}

// 拾取道具的请求 ID_C2S_REQUEST_PICK_ITEM
message CPickItemRequest
{
	uint32	EntityID	= 1;		// 道具的实体ID
}

// 拾取道具的回应 ID_S2C_RESPONSE_PICK_ITEM
message CPickItemResponse
{
	uint32	Result		= 1;		// 结果
}

// 得到商店信息的请求 ID_C2S_REQUEST_GET_SHOP_INFO
message CGetShopInfoRequest
{
	uint32	ShopType	= 1;		// 商店类型
}

// 得到商店信息的回应 ID_S2C_RESPONSE_GET_SHOP_INFO
message PBShopGoods
{
	uint32	GoodsID		= 1;		// 商品ID
	uint32	GoodsNum	= 2;		// 商品数量
	uint32	ConsumeID	= 3;		// 消耗ID
	uint32	ConsumeNum	= 4;		// 消耗数量
	uint32	TabIndex	= 5;		// 标签页
}
message CGetShopInfoResponse
{
			 uint32			Result	= 1;	// 结果
	repeated PBShopGoods	Goods	= 2;	// 商品信息列表
}