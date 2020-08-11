#ifndef __PROPERTYCONFIG_H__
#define __PROPERTYCONFIG_H__
#include "commontype.h"
#include <string>
#include <vector>
using namespace std;
class CRechargeGoods
{
public:
	string		mGoodsID;
	string		mTitle;
	byte		mType;
	int			mGameCoin;
	int			mCoinNum;
	string		mCoinType;
	int			mVIPExp;
	string		mChannelName;
	int			mActivityCoin;
};
class CRechargeConfig
{
public:
	typedef vector<CRechargeGoods> RECHARGE_GOODS_LIST;

public:
	bool	LoadRechargeConfig(const char* pFilePath);
public:
	RECHARGE_GOODS_LIST		mGoodsList;
	bool					mCheckChannelName;
};
#endif