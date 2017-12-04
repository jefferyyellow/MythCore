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
	ERR_TEMPLATE_INVALID				= 260,				// ģ��Ƿ�
};




// 300 - 399
enum ERR_ITEM
{
	ERR_OBTAIN_MONEY_INVALID			= 300,		// �õ��Ľ�ҷǷ�
	ERR_CONSUME_MONEY_INVALID			= 301,		// ���ѵĽ�ҷǷ�
	ERR_OBTAIN_DIAMOND_INVALID			= 302,		// �õ�����ʯ�Ƿ�
	ERR_CONSUME_DIAMOND_INVALID			= 303,		// ���ѵ���ʯ�Ƿ�
	ERR_ITEM_INDEX_OBJ_ID_INVALID		= 304,		// ��Ӧ�����ĵ���ID�Ƿ�
	ERR_ITEM_INDEX_OBJ_DATA_NULL		= 305,		// ��Ӧ�����ĵ�������Ϊ��
	ERR_ITEM_INDEX_NUM_NOT_ENOUGH		= 306,		// ��Ӧ�����ĵ�����������
	ERR_ITEM_INDEX_INVALID				= 307,		// ���������Ƿ�
	ERR_TEMPLATE_IS_NOT_ITEM			= 308,		// ��Ӧ��ģ�岻�ǵ���
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
	ERR_TASK_ALREADY_COMPLETE			= 502,		// �����Ѿ����
	ERR_TASK_PRE_TASK_NOT_COMPLETE		= 503,		// ������ʱǰ������û�����
	ERR_TASK_PLAYER_LEVEL_INVALID		= 504,		// ������ʱ��ҵȼ�����������
	ERR_TASK_TASK_LIST_FULL				= 505,		// �����б�����
	ERR_TASK_PLAYER_HAS_NO_TASK			= 506,		// �������û�и�����
	ERR_TASK_TASK_NOT_COMPLETE			= 507,		// 
};
#endif