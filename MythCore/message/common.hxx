syntax = "proto3";

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

