#ifndef __ERRCODE_H__
#define __ERRCODE_H__

// ͨ�ô�����
#define SUCCESS			0

// linux�Ĵ�����0-255���������Ǵ�300��ʼ
// windows�Ĵ������ر�࣬�Ȳ������ˣ����ʹ��GetLastError��ת�����Լ��Ĵ�����
// 300 - 399
enum ERR_COMMON
{
	ERR_UNKNOWN							= 300,				// δ֪����
	ERR_PARAMETER_INVALID				= 301,				// �����Ƿ�
	ERR_CURRENCY_NOT_ENOUGH				= 302,				// ���Ҳ���
	ERR_TEMPLATE_INVALID				= 303,				// ģ��Ƿ�
	ERR_BAGGAGE_IS_FULL					= 304,				// ��������
};

// 400 - 499
enum ERR_ITEM
{
	ERR_OBTAIN_MONEY_INVALID			= 400,		// �õ��Ľ�ҷǷ�
	ERR_CONSUME_MONEY_INVALID			= 401,		// ���ѵĽ�ҷǷ�
	ERR_OBTAIN_DIAMOND_INVALID			= 402,		// �õ�����ʯ�Ƿ�
	ERR_CONSUME_DIAMOND_INVALID			= 403,		// ���ѵ���ʯ�Ƿ�
	ERR_ITEM_INDEX_OBJ_ID_INVALID		= 404,		// ��Ӧ�����ĵ���ID�Ƿ�
	ERR_ITEM_INDEX_OBJ_DATA_NULL		= 405,		// ��Ӧ�����ĵ�������Ϊ��
	ERR_ITEM_INDEX_NUM_NOT_ENOUGH		= 406,		// ��Ӧ�����ĵ�����������
	ERR_ITEM_INDEX_INVALID				= 407,		// ���������Ƿ�
	ERR_ITEM_TEMPLATE_IS_NOT_ITEM		= 408,		// ��Ӧ��ģ�岻�ǵ���
	ERR_ITEM_MOVE_SAME_INDEX			= 409,		// �����ƶ�����ͬ������
	ERR_ITEM_MOVE_NUM_INVALID			= 410,		// �����ƶ�����Ŀ����
	ERR_ITEM_CREATE_ITEM_OBJ_FAILURE	= 411,		// ��������objʧ��
	ERR_ITEM_MOVE_SRC_NUM_INVALID		= 412,		// �����ƶ���Դ������Ŀ����ȷ
};

// 500 - 599
enum ERR_MAP
{
	ERR_MAP_CREATE_MAP_UNIT_FAILURE		= 500,		// ������ͼ��Ԫʧ��
	ERR_MAP_TELEPORT_MAP_FAILURE		= 501,		// ���͵�ͼʧ��
};

// 600 - 699
enum ERR_TASK
{
	ERROR_TASK_CONFIG_IS_NULL			= 600,			// ��������Ϊ��
	ERROR_TASK_PLAYER_ALREADY_COMPLETE	= 601,			// ����Ѿ���ɸ�����
	ERROR_TASK_PLAYER_ALREADY_ACCEPT	= 602,			// ����Ѿ����ܸ�����
	ERROR_TASK_PRE_TASK_NOT_COMPLETE	= 603,			// ǰ������û�����
	ERROR_TASK_PLAYER_LEVEL_NOT_ENOUGH	= 604,			// ��ҵȼ�����
	ERROR_TASK_SCHOOL_NOT_MATCH			= 605,			// �����Ʊ��ƥ��
	ERROR_TASK_PRESTIGE_NOT_ENOUGH		= 606,			// ��������
	ERROR_TASK_FAVORABILITY_NOT_ENOUGH	= 607,			// ����ѺöȲ���
	ERROR_TASK_PARTNER_NOT_IN_ARRAY		= 608,			// ��Ӧ�Ļ�鲻������
	ERROR_TASK_PARTNER_NUM_NOT_ENOUGH	= 609,			// �����Ŀ����
	ERROR_TASK_HAS_NO_TASK				= 610,			// û�ж�Ӧ������
	ERROR_TASK_KILL_OGRE_NOT_ENOUGH		= 611,			// ɱ�ּ�������
	ERROR_TASK_ITEM_NUM_NOT_ENOUGH		= 612,			// ������Ŀ����
	ERROR_TASK_ITEM_DATA_IS_NULL		= 613,			// ������ĿΪ��
	ERROR_TASK_ITEM_INDEX_NOT_MATCH		= 614,			// �������ĵ��߲����������
	ERROR_TASK_COLLECT_NUM_NOT_MATCH	= 615,			// �ɼ�����Ŀ����
	ERROR_TASK_CHALLENGE_NOT_ENOUGH		= 616,			// ��սNPC�Ĵ�������
	ERROR_TASK_COMBAT_POWER_NOT_ENOUGH	= 617,			// ���ս������
	ERROR_TASK_ALL_COMBAT_POWER_NOT_ENOUGH = 618,		// �����������ս������
	ERROR_TASK_ALL_EQUIP_LEVEL_NOT_ENOUGH = 619,		// ������е�װ���ȼ�����
	ERROR_TASK_EQUIP_DATA_NULL			= 620,			// ���װ������Ϊ��
	ERROR_TASK_EQUIP_LEVEL_NOT_ENOUGH	= 621,			// ���װ���ȼ�����
	ERROR_TASK_INSTANCE_TIMES_NOT_ENOUGH= 622,			// ���ͨ��������������
	ERROR_TASK_TELEPORT_FAIL			= 623,			// ����ʧ��
	ERROR_TASK_PARTNER_LIST_FULL		= 624,			// ����б�����
	ERROR_TASK_PLAYER_TASK_LIST_FULL	= 625,			// ��������б�����
	ERROR_TASK_ALREADY_END				= 626,			// ��������Ѿ���ɹ���
	ERROR_TASK_CLOSE_PRESTIGE			= 627,			// ������������̫�߹ر�
	ERROR_TASK_CLOSE_LEVEL				= 628,			// �������ڵȼ�̫�߹ر�
	ERROR_TASK_CLOSE_PRE_TASK_COMPLETE	= 629,			// ��������ǰ��������ɹر�
	ERROR_TASK_HAVENOT_POLT				= 630,			// ����û�о���
	ERROR_TASK_NPC_IS_VALID				= 631,			// ����NPC�Ƿ�
	ERROR_TASK_FAR_FROM_NPC				= 632,			// ����NPC��������̫Զ
};

// 700 - 799
enum ERR_EQUIP
{	
	ERR_EQUIP_SRC_ITEM_DATA_NULL		= 700,		// װ���ĵ�������Ϊ��
	ERR_EQUIP_ITEM_IS_NOT_EQUIP			= 701,		// ���߲���װ��
	ERR_EQUIP_PART_IS_INVALID			= 702,		// װ����λ�Ƿ�
	ERR_EQUIP_DES_INDEX_DATA_NOT_NULL	= 703,		// Ŀ���������ݲ�Ϊ��
	ERR_EQUIP_PLAYER_LEVEL_NOT_ENOUGH	= 704,		// װ���ȼ�����
	ERR_EQUIP_ITEM_CANNOT_EQUIP			= 705,		// ���߲���װ��
};

// 800 - 1099
enum ERR_SERVER_ACT
{
	
};
#endif