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
	ERR_CURRENCY_NOT_ENOUGH				= 258,				// �Ҳ���
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
	ERR_TASK_KILL_OGRE_NOT_ENOUGH		= 507,		// ɱ�ֵ���Ŀ����
	ERR_TASK_ITEM_NUM_NOT_ENOUGH		= 508,		// ���ߵ���Ŀ����
};

// 600 - 699
enum ERR_EQUIP
{	
	ERR_EQUIP_SRC_ITEM_DATA_NULL		= 600,		// װ���ĵ�������Ϊ��
	ERR_EQUIP_ITEM_IS_NOT_EQUIP			= 601,		// ���߲���װ��
	ERR_EQUIP_PART_IS_INVALID			= 602,		// װ����λ�Ƿ�
	ERR_EQUIP_DES_INDEX_DATA_NOT_NULL	= 603,		// Ŀ���������ݲ�Ϊ��
	ERR_EQUIP_PLAYER_LEVEL_NOT_ENOUGH	= 604,		// װ���ȼ�����
	ERR_EQUIP_ITEM_CANNOT_EQUIP			= 605,		// ���߲���װ��
};
#endif