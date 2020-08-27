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

message PBTask
{
				uint32				TaskID			= 1;		// ����ID
	repeated	uint32				Param			= 2;		// �������
};

message PBTaskList
{
				uint32				MaxCompleteTaskID=1;		// �����Ѿ���ɵ�����ID
	repeated	fixed32				CompleteTasks	= 2;		// ���ܿ���
	repeated	PBTask				TaskList		= 3;		// �����б�
};

message PBMailItem
{
	uint32			ItemID			= 1;	// ����ID
	uint32			ItemNum			= 2;	// ������Ŀ
}

message PBMailItemList
{
	repeated PBMailItem	MailItem	= 1;	// �ʼ�����
}

message	PBMail
{
	uint32			mail_id			= 1;	// �ʼ�ID
	uint32			mail_status		= 2;	// �ʼ�״̬
	uint32			role_id			= 3;	// ��ɫID
	uint32			mail_type		= 4;	// �ʼ�����
	uint32			item_log		= 5;	// ������־����
	uint32			create_time		= 6;	// �ʼ�����ʱ��
	string			mail_title		= 7;	// �ʼ�����
	string			mail_body		= 8;	// �ʼ�����
	PBMailItemList	mail_item		= 9;	// �ʼ�����
}
