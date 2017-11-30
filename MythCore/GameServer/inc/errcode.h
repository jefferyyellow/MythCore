#ifndef __ERRCODE_H__
#define __ERRCODE_H__

// ͨ�ô�����
#define SUCCESS			0

// linux�Ĵ�����0-255���������Ǵ�256��ʼ
// windows�Ĵ������ر�࣬�Ȳ������ˣ����ʹ��GetLastError��ת�����Լ��Ĵ�����
enum ERR_COMMON
{
	ERR_UNKNOWN							= 256,				// δ֪����
	ERR_PARAMETER_INVALID				= 257,				// �����Ƿ�
	ERR_MONEY_NOT_ENOUGH				= 258,				// ��Ҳ���
	ERR_DIAMOND_NOT_ENOUGH				= 259,				// ��Ҳ���
};




// 300 - 399
enum ERR_ITEM
{
	ERR_ITEM_OBTAIN_MONEY_INVALID		= 300,		// �õ��Ľ�ҷǷ�
	ERR_ITEM_CONSUME_MONEY_INVALID		= 301,		// ���ѵĽ�ҷǷ�
	ERR_ITEM_OBTAIN_DIAMOND_INVALID		= 302,		// �õ�����ʯ�Ƿ�
	ERR_ITEM_CONSUME_DIAMOND_INVALID	= 303,		// ���ѵ���ʯ�Ƿ�

};

// 400 - 499
enum ERR_MAP
{
	ERR_MAP_CREATE_MAP_UNIT_FAILURE		= 400,		// ������ͼ��Ԫʧ��
	ERR_MAP_TELEPORT_MAP_FAILURE		= 401,		// ���͵�ͼʧ��
};

// 500 - 599
enum ERR_TASK
{
	ERR_TASK_ID_INVALID					= 500,		// ����ID�Ƿ�
	ERR_TASK_CONFIG_IS_NULL				= 501,		// ��������Ϊ��
	ERR_TASK_
};
#endif