#ifndef __SHOPCONFIG_H__
#define __SHOPCONFIG_H__
#include <vector>
enum EmShopType
{
	emShopType_Common	= 0,
	emShopType_Level	= 1,
	emShopTypeMax
};

class PBShopGoods;
class CShopGoods
{
public:
	void		createToPB(PBShopGoods* pbGoods); 

public:
	/// 商品ID
	int			mGoodsID;
	/// 商品数量
	int			mGoodsNum;
	/// 消耗的货币（道具）ID
	int			mConsumeID;
	/// 消耗的货币（道具）数量
	int			mConsumeNum;
	/// 标签页
	int			mTabIndex;
};
typedef std::vector<CShopGoods>			SHOP_GOODS_LIST;

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
	CShopConfig(EmShopType eType) 
		:mShopType(eType)
	{
		clear();
	}
	~CShopConfig()
	{

	}
	void clear()
	{
		mLevelList.clear();
	}

public:
	bool			loadShopConfig(const char* pFilePath);
	CShopLevelData*	GetLevelData(int nLevel);

public:
	SHOP_LEVEL_DATA	mLevelList;
	
public:
	EmShopType	GetShopType() const { return mShopType; }
	void		SetShopType(EmShopType nValue) { mShopType = nValue; }

private:
	EmShopType		mShopType;
};
typedef std::vector<CShopConfig>	SHOP_CONFIG_LIST;
#endif