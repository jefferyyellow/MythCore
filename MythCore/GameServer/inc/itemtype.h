#ifndef __ITEMTYPE_H__
#define __ITEMTYPE_H__
// 货币类型枚举
enum EmCurrencyType
{
	emCurrency_Money		= 0,	// 金币
	emCurrency_Diamond		= 1,	// 钻石
	emCurrencyMax
};

// 货币ID, 这个顺序和EmCurrencyType一致才行
// 在模板里定义货币的ID道具
#define MONEY_ID			80			// 金币
#define DIAMOND_ID			81			// 钻石

#define MAX_CURRENCY_ID		99			// 最大的货币ID

// 各种货币
#define CURRENCY_TYPE_2_ID(eType) (MONEY_ID + eType)
#define CURRENCY_ID_2_TYPE(nId) (nId - MONEY_ID)

#define MAX_CURRENCY_NUM     2000000000			// 最大的货币数量（20亿）
#define MAX_EQUIP_LIST		 6					// 最大的装备列表

#define EQUIP_PROPERTY_NUM	6					// 装备最大的属性数目
#endif