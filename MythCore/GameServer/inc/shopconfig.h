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
	/// ��ƷID
	int			mGoodsID;
	/// ��Ʒ����
	int			mGoodsNum;
	/// ���ĵĻ��ң����ߣ�ID
	int			mConsumeID;
	/// ���ĵĻ��ң����ߣ�����
	int			mConsumeNum;
	/// ��ǩҳ
	int			mTabIndex;
};
typedef std::vector<CShopGoods>			SHOP_GOODS_LIST;

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