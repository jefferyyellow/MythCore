#ifndef __ERRCODE_H__
#define __ERRCODE_H__

// ͨ�ô�����
#define SUCCESS			0

// linux�Ĵ�����0-255���������Ǵ�256��ʼ
#define ERROR_UNKNOWN					256				// δ֪����
#define ERROR_PARAMETER_INVALID			257				// �����Ƿ�
#define ERROR_MONEY_NOT_ENOUGH			258				// ��Ҳ���
#define ERROR_DIAMOND_NOT_ENOUGH		258				// ��Ҳ���



// 300 - 399
enum ERROR_ITEM
{
	ERROR_ITEM_OBTAIN_MONEY_INVALID		= 300,		// �õ��Ľ�ҷǷ�
	ERROR_ITEM_CONSUME_MONEY_INVALID	= 301,		// ���ѵĽ�ҷǷ�
	ERROR_ITEM_OBTAIN_DIAMOND_INVALID	= 302,		// �õ�����ʯ�Ƿ�
	ERROR_ITEM_CONSUME_DIAMOND_INVALID	= 303,		// ���ѵ���ʯ�Ƿ�

};

#endif