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
	/// ��ƷID
	int			mGoodsID;
	/// ��Ʒ����
	int			mGoodsNum;
	/// ���ĵĻ��ң����ߣ�ID
	int			mConsumeItemID;
	/// ���ĵĻ��ң����ߣ�����
	int			mConsumeItemNum;
	/// ��Ʒ���
	int			mGoodsStockNum;
	/// ��ǩҳ
	int			mTabIndex;
};
typedef std::vector<CGoods>			SHOP_GOODS_LIST;

class CShopLevelData
{
public:
	/// �ȼ���Сֵ
	int			mMinLevel;
	/// �ȼ����ֵ
	int			mMaxLevel;
	/// ��Ʒ�б�
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