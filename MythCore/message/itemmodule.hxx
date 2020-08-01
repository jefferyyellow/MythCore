syntax = "proto3";
import "common.hxx";
// ��ϢID��Χ��1-65535������16λ��ǰ6λ��ʾģ��ţ���10λ��ʾģ���ڵ���ţ�һ��ģ�����ֻ��1024����Ϣ

enum ITEM_MODULE_MSG_ID
{
	ID_ITEM_MODULE_ERROR					= 0x0;
	ID_S2C_NOTIYF_CURRENCY_UPDATE			= 0x0800;	// ��һ��Ҹ���֪ͨ
	ID_S2C_NOTIYF_INSERT_ITEM				= 0x0802;	// �������֪ͨ(û���������Եĵ���)
	ID_S2C_NOTIYF_INSERT_ITEM_OBJ			= 0x0803;	// �������֪ͨ(���������Եĵ���)
	ID_S2C_NOTIYF_REMOVE_ITEM				= 0x0804;	// ɾ������֪ͨ
	ID_C2S_REQUEST_USE_ITEM					= 0x0805;	// ʹ�õ�������
	ID_S2C_RESPONSE_USE_ITEM				= 0x0806;	// ʹ�õ��߻�Ӧ
	ID_C2S_REQUEST_SELL_ITEM				= 0x0807;	// ������������
	ID_S2C_RESPONSE_SELL_ITEM				= 0x0808;	// �������߻�Ӧ
	ID_C2S_REQUEST_PURCHASE_ITEM			= 0x0809;	// �����������
	ID_S2C_RESPONSE_PURCHASE_ITEM			= 0x080A;	// ������߻�Ӧ
	ID_C2S_REQUEST_EQUIP_ITEM				= 0x080B;	// װ�����ߵ�����
	ID_S2C_RESPONSE_EQUIP_ITEM				= 0x080C;	// װ�����ߵĻ�Ӧ
	ID_C2S_REQUEST_UNEQUIP_ITEM				= 0x080D;	// ж��װ��������
	ID_S2C_RESPONSE_UNEQUIP_ITEM			= 0x080E;	// ж��װ���Ļ�Ӧ
	ID_S2C_NOTIYF_EQUIP_CHANGE				= 0x080F;	// װ���ı��֪ͨ
	ID_C2S_REQUEST_GET_SHOP_INFO			= 0x0810;	// �õ��̵���Ϣ������
	ID_S2C_RESPONSE_GET_SHOP_INFO			= 0x0811;	// �õ��̵���Ϣ�Ļ�Ӧ
	ID_C2S_REQUEST_PICK_ITEM				= 0x0812;	// ʰȡ���ߵ�����
	ID_S2C_RESPONSE_PICK_ITEM				= 0x0813;	// ʰȡ���ߵĻ�Ӧ

};

// ��һ��Ҹ���֪ͨ ID_S2C_NOTIYF_CURRENCY_UPDATE
message CCurrencyUpdateNotify
{
	uint32	CurrencyType		= 1;		// ��������
	uint32	CurrencyNum			= 2;		// ��������ҵĻ�����Ŀ
}

// �������֪ͨ(û���������Եĵ���) ID_S2C_NOTIYF_INSERT_ITEM
message CInsertItemNotify
{
			 uint32 ItemID		= 1;		// ��ƷID
	repeated uint32 Index		= 2;		// ��Ʒ����
	repeated uint32 Number		= 3;		// ��Ʒ��Ŀ
}

// �������֪ͨ(���������Եĵ���) ID_S2C_NOTIYF_INSERT_ITEM_OBJ
message CInsertItemObjNotify
{
	PBItemObject	ItemObject	= 1;		// װ��
}

// ɾ������֪ͨ ID_S2C_NOTIYF_REMOVE_ITEM
message CRemoveItemNotify
{
	uint32 Index		= 1; 		// ��Ʒ����
	uint32 Number		= 2; 		// ��Ʒ��Ŀ
}

// ʹ�õ������� ID_C2S_REQUEST_USE_ITEM
message CUseItemRequest
{
	uint32 Index		= 1; 		// ��Ʒ����
	uint32 Number		= 2; 		// ��Ʒ��Ŀ
}

// ʹ�õ��߻�Ӧ ID_S2C_RESPONSE_USE_ITEM
message CUseItemResponse
{
	uint32 Result		= 1;		// ���
}

// ������������ ID_C2S_REQUEST_SELL_ITEM
message CSellItemRequest
{
	uint32 Index		= 1;		// ��Ʒ����
}

// �������߻�Ӧ ID_S2C_RESPONSE_SELL_ITEM
message CSellItemResponse
{
	uint32 Result		= 1;		// ���
}

// ����������� ID_C2S_REQUEST_PURCHASE_ITEM
message CPurchaseItemRequest
{
	uint32 ShopType		= 1;		// �̵�����
	uint32 Index		= 2;		// ����
	uint32 Num			= 3;		// ��Ŀ
}

// ������߻�Ӧ ID_S2C_RESPONSE_PURCHASE_ITEM
message CPurchaseItemResponse
{
	uint32 Result		= 1;		// ���
}

// װ�����ߵ����� ID_C2S_REQUEST_EQUIP_ITEM
message CEquipItemRequest
{
	uint32	ItemIndex	= 1;		// װ���ĵ���
}

// װ�����ߵĻ�Ӧ ID_S2C_RESPONSE_EQUIP_ITEM
message CEquipItemResponse
{
	uint32	Result		= 1;		// ���	
	uint32	ItemIndex	= 2;		// װ���ĵ���
}

// ж��װ�������� ID_C2S_REQUEST_UNEQUIP_ITEM
message CUnEquipItemRequest
{
	uint32	EquipPart	= 1;		// װ����λ
	uint32	ItemIndex	= 2;		// װ���ڰ����������
}

// ж��װ���Ļ�Ӧ ID_S2C_RESPONSE_UNEQUIP_ITEM
message CUnEquipItemResponse
{
	uint32	Result		= 1;		// ���
	uint32	EquipPart	= 2;		// װ����λ
	uint32	ItemIndex	= 3;		// װ���ڰ����������
}

// װ���ı�֪ͨ ID_S2C_NOTIYF_EQUIP_CHANGE
message CEquipChangeNotify
{
	uint32	EntityID	= 1;		// ��ɫID
	uint32	EquipPart	= 2;		// װ����λ
	uint32	EquipItemID	= 3;		// װ������ID
}

// ʰȡ���ߵ����� ID_C2S_REQUEST_PICK_ITEM
message CPickItemRequest
{
	uint32	EntityID	= 1;		// ���ߵ�ʵ��ID
}

// ʰȡ���ߵĻ�Ӧ ID_S2C_RESPONSE_PICK_ITEM
message CPickItemResponse
{
	uint32	Result		= 1;		// ���
}

// �õ��̵���Ϣ������ ID_C2S_REQUEST_GET_SHOP_INFO
message CGetShopInfoRequest
{
	uint32	ShopType	= 1;		// �̵�����
}

// �õ��̵���Ϣ�Ļ�Ӧ ID_S2C_RESPONSE_GET_SHOP_INFO
message PBShopGoods
{
	uint32	GoodsID		= 1;		// ��ƷID
	uint32	GoodsNum	= 2;		// ��Ʒ����
	uint32	ConsumeID	= 3;		// ����ID
	uint32	ConsumeNum	= 4;		// ��������
	uint32	TabIndex	= 5;		// ��ǩҳ
}
message CGetShopInfoResponse
{
			 uint32			Result	= 1;	// ���
	repeated PBShopGoods	Goods	= 2;	// ��Ʒ��Ϣ�б�
}