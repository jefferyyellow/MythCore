#ifndef __SHOPCONFIG_H__
#define __SHOPCONFIG_H__
#include <vector>
enum EmShopType
{
	emShopType_Common	= 0,
	emShopType_Level	= 1,
	emShopTypeMax
};

class CGoods
{
public:
	/// 商品ID
	int			mGoodsID;
	/// 商品数量
	int			mGoodsNum;
	/// 消耗的货币（道具）ID
	int			mConsumeItemID;
	/// 消耗的货币（道具）数量
	int			mConsumeItemNum;
	/// 商品库存
	int			mGoodsStockNum;
	/// 标签页
	int			mTabIndex;
};
typedef std::vector<CGoods>			SHOP_GOODS_LIST;

class CShopLevelData
{
public:
	/// 等级最小值
	int			mMinLevel;
	/// 等级最大值
	int			mMaxLevel;
	/// 商品列表
	SHOP_GOODS_LIST	mGoodsList;
};
typedef std::vector<CShopLevelData>	SHOP_LEVEL_DATA;

class CShopConfig
{
public:
	CShopConfig()
	{
		
	}
	~CShopConfig()
	{

	}
	void clear()
	{
		mLevelData.clear();
	}

public:
	void			loadShopConfig(const char* pFilePath);
public:
	SHOP_LEVEL_DATA	mLevelData;
};
typedef std::vector<CShopConfig>	SHOP_CONFIG_LIST;
#endif