syntax = "proto3";
message PBItemList
{
	repeated	PBItemObject	ItemObject	= 1;
				uint32			Size		= 2;
}

message PBItemObject
{
	uint32			ItemID			= 1;		// ID
	uint32			Number			= 2;		// ����
	uint32			Index			= 3;		// �����е�����

	PBItemEquip		ItemEquip		= 4;		// װ��
}

message PBItemEquip
{
	uint32			Level		= 4;			// �ȼ�
}

// ����
message PBTask
{

}