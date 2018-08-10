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

#define MAX_PLAYER_TIMER_NUM	64		// ������ļ�ʱ����Ŀ
#define MAX_NPC_TIMER_NUM		16		// NPC���ļ�ʱ����Ŀ
#endif