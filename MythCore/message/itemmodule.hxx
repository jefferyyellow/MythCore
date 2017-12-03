syntax = "proto3";
import "common.hxx";
// ��ϢID��Χ��1-65535������16λ��ǰ6λ��ʾģ��ţ���10λ��ʾģ���ڵ���ţ�һ��ģ�����ֻ��1024����Ϣ

enum ITEM_MODULE_MSG_ID
{
	ID_ITEM_MODULE_ERROR					= 0x0;
	ID_S2C_NOTIYF_MONEY_UPDATE				= 0x0800;	// ��ҽ�Ǯ����֪ͨ
	ID_S2C_NOTIYF_DIAMOND_UPDATE			= 0x0801;	// �����ʯ����֪ͨ
	ID_S2C_NOTIYF_INSERT_ITEM				= 0x0802;	// �������֪ͨ(û���������Եĵ���)
	ID_S2C_NOTIYF_INSERT_ITEM_OBJ			= 0x0803;	// �������֪ͨ(���������Եĵ���)
	ID_S2C_NOTIYF_REMOVE_ITEM				= 0x0804;	// ɾ������֪ͨ
	ID_C2S_REQUEST_USE_ITEM					= 0x0805;	// ʹ�õ�������
	ID_S2C_RESPONSE_USE_ITEM				= 0x0806;	// ʹ�õ��߻�Ӧ
	ID_C2S_REQUEST_SELL_ITEM				= 0x0807;	// ������������
	ID_S2C_RESPONSE_SELL_ITEM				= 0x0808;	// �������߻�Ӧ
	ID_C2S_REQUEST_PURCHASE_ITEM			= 0x0809;	// �����������
	ID_S2C_RESPONSE_PURCHASE_ITEM			= 0x080A;	// ������߻�Ӧ
};

// ��ҽ�Ǯ����֪ͨ ID_S2C_NOTIYF_MONEY_UPDATE
message CMoneyUpdateNotify
{
	uint32	Money				= 1;		// ��ҵ�ǰ�Ľ�Ǯ
}

// �����ʯ����֪ͨ ID_S2C_NOTIYF_DIAMOND_UPDATE
message CDiamondUpdateNotify
{
	uint32	Diamond				= 1;		// ��ҵ�ǰ����ʯ
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
	uint32 Index		= 1;		// ����
	uint32 Num			= 2;		// ��Ŀ
}

// ������߻�Ӧ ID_S2C_RESPONSE_PURCHASE_ITEM
message CPurchaseItemResponse
{
	uint32 Result		= 1;		// ���
}