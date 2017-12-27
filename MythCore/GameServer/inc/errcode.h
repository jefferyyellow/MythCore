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
	ERROR_TASK_CONFIG_IS_NULL			= 500,			// ��������Ϊ��
	ERROR_TASK_PLAYER_ALREADY_COMPLETE	= 501,			// ����Ѿ���ɸ�����
	ERROR_TASK_PLAYER_ALREADY_ACCEPT	= 502,			// ����Ѿ����ܸ�����
	ERROR_TASK_PRE_TASK_NOT_COMPLETE	= 503,			// ǰ������û�����
	ERROR_TASK_PLAYER_LEVEL_NOT_ENOUGH	= 504,			// ��ҵȼ�����
	ERROR_TASK_SCHOOL_NOT_MATCH			= 505,			// �����Ʊ��ƥ��
	ERROR_TASK_PRESTIGE_NOT_ENOUGH		= 506,			// ��������
	ERROR_TASK_FAVORABILITY_NOT_ENOUGH	= 507,			// ����ѺöȲ���
	ERROR_TASK_PARTNER_NOT_IN_ARRAY		= 508,			// ��Ӧ�Ļ�鲻������
	ERROR_TASK_PARTNER_NUM_NOT_ENOUGH	= 509,			// �����Ŀ����
	ERROR_TASK_HAS_NO_TASK				= 510,			// û�ж�Ӧ������
	ERROR_TASK_KILL_OGRE_NOT_ENOUGH		= 511,			// ɱ�ּ�������
	ERROR_TASK_ITEM_NUM_NOT_ENOUGH		= 512,			// ������Ŀ����
	ERROR_TASK_ITEM_DATA_IS_NULL		= 513,			// ������ĿΪ��
	ERROR_TASK_ITEM_INDEX_NOT_MATCH		= 514,			// �������ĵ��߲����������
	ERROR_TASK_COLLECT_NUM_NOT_MATCH	= 515,			// �ɼ�����Ŀ����
	ERROR_TASK_CHALLENGE_NOT_ENOUGH		= 516,			// ��սNPC�Ĵ�������
	ERROR_TASK_COMBAT_POWER_NOT_ENOUGH	= 517,			// ���ս������
	ERROR_TASK_ALL_COMBAT_POWER_NOT_ENOUGH = 518,		// �����������ս������
	ERROR_TASK_ALL_EQUIP_LEVEL_NOT_ENOUGH = 519,		// ������е�װ���ȼ�����
	ERROR_TASK_EQUIP_DATA_NULL			= 520,			// ���װ������Ϊ��
	ERROR_TASK_EQUIP_LEVEL_NOT_ENOUGH	= 521,			// ���װ���ȼ�����
	ERROR_TASK_INSTANCE_TIMES_NOT_ENOUGH= 522,			// ���ͨ��������������
	ERROR_TASK_TELEPORT_FAIL			= 523,			// ����ʧ��
	ERROR_TASK_PARTNER_LIST_FULL		= 524,			// ����б�����
	ERROR_TASK_PLAYER_TASK_LIST_FULL	= 525,			// ��������б�����
	ERROR_TASK_ALREADY_END				= 526,			// ��������Ѿ���ɹ���
	ERROR_TASK_CLOSE_PRESTIGE			= 527,			// ������������̫�߹ر�
	ERROR_TASK_CLOSE_LEVEL				= 528,			// �������ڵȼ�̫�߹ر�
	ERROR_TASK_CLOSE_PRE_TASK_COMPLETE	= 529,			// ��������ǰ��������ɹر�
	ERROR_TASK_HAVENOT_POLT				= 530,			// ����û�о���
	ERROR_TASK_NPC_IS_VALID				= 531,			// ����NPC�Ƿ�
	ERROR_TASK_FAR_FROM_NPC				= 532,			// ����NPC��������̫Զ
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