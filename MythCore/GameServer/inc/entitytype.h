#ifndef __ENTITYTYPE_H__
#define __ENTITYTYPE_H__
enum EmEntityType
{
	emEntityType_None			= 0,	// ��
	emEntityType_Player			= 1,	// ���
	emEntityType_Ogre			= 2,	// ����
	emEntityType_FuncNPC		= 3,	// ����NPC
	emEntityType_Item			= 4,	// ����
};


enum EmAliveState
{
	emAliveState_Alive			= 0,	// ����״̬
	emAliveState_Dead			= 1,	// ����״̬
};
#define MAX_PLAYER_TIMER_NUM	64		// ������ļ�ʱ����Ŀ
#define MAX_NPC_TIMER_NUM		16		// NPC���ļ�ʱ����Ŀ
#define ENTITY_PRO_MASK			0x00FF	// ʵ����������
#define ENTITY_PRO_PER_START	0x0100	// ʵ��ٷֱ����Կ�ʼ����EmProType��Ӧ��
#endif