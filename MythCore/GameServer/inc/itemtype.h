#ifndef __ITEMTYPE_H__
#define __ITEMTYPE_H__
// ��������ö��
enum EmCurrencyType
{
	emCurrency_Money		= 0,	// ���
	emCurrency_Diamond		= 1,	// ��ʯ
	emCurrencyMax
};

// ����ID, ���˳���EmCurrencyTypeһ�²���
// ��ģ���ﶨ����ҵ�ID����
#define MONEY_ID			80			// ���
#define DIAMOND_ID			81			// ��ʯ

#define MAX_CURRENCY_ID		99			// ���Ļ���ID

// ���ֻ���
#define CURRENCY_TYPE_2_ID(eType) (MONEY_ID + eType)
#define CURRENCY_ID_2_TYPE(nId) (nId - MONEY_ID)

#define MAX_CURRENCY_NUM     2000000000			// ���Ļ���������20�ڣ�
#define MAX_EQUIP_LIST		 6					// ����װ���б�

#define EQUIP_PROPERTY_NUM	6					// װ������������Ŀ
#endif