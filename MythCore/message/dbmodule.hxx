syntax = "proto3";
import "common.hxx";
// ע�⣺���̵���Ϣ��Ҫ��repeated��parseMessage���ᴦ��repeated
message PBSavePlayer
{
	PBItemList		bag				= 1;	// ����
	PBItemList		equip			= 2;	// װ��
	PBTaskList		task			= 3;	// ����
}


// ����proto3����ͨ�ֶ�û��hasfield���жϣ�
// ����ֻ����ͨ�ʼ���ȫ���ʼ�ʹ������PB�ṹ
message PBGlobalMail
{
	uint32			mail_id			= 1;	// �ʼ�ID
	uint32			mail_type		= 2;	// �ʼ�����
	uint32			create_time		= 3;	// �ʼ�����ʱ��
	string			mail_title		= 4;	// �ʼ�����
	string			mail_body		= 5;	// �ʼ�����
	PBMailItemList	mail_item		= 6;	// �ʼ�����
}

message PBGameEventParam
{
	repeated uint32	Param			= 1;	// int����
	repeated string	StrParam		= 2;	// �ַ�������
}

message PBGameEvent
{
	uint32			event_id		= 1;	// �¼�ID
	uint32			event_type		= 2;	// �¼�����
	uint32			role_id			= 3;	// ��ɫID
	uint32			create_time		= 4;	// ����ʱ��
	PBGameEventParam event_param	= 5;	// �¼�����
}