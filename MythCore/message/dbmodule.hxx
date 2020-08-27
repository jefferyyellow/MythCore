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