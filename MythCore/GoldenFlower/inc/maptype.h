#ifndef __MAPTYPE_H__
#define __MAPTYPE_H__

#define VISIBLE_RADIUS_X 10				// �ɼ�����X����뾶
#define VISIBLE_RADIUS_Y 10				// �ɼ�����Y����뾶
enum EmMapType
{
	emMapType_None			= 0,		// ������
	emMapType_City			= 1,		// ����
	emMapType_Field			= 2,		// Ұ��
	emMapType_Instance		= 3,		// ����
};

#define MAX_MAP_ID			256			// ���ĵ�ͼID	
#define MAX_MAP_PORTAL_NUM	8			// ��ͼ�����Ĵ����ŵ���Ŀ
#define MAP_UNIT_SIZE		40			// ��ͼ��Ԫ��С
#endif