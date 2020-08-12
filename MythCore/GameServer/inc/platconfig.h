#ifndef __PLATCONFIG_H__
#define __PLATCONFIG_H__
#include "commontype.h"
#include <string>
#include <vector>
using namespace std;

enum EmRechargeType
{
	emRechargeType_None				= 0,	// 未知
	emRechargeType_Diamond			= 1,	// 钻石
};
class CRechargeGoods
{
public:
	/// 商品ID的CRC32编码
	uint		mGoodsIDCRC;
	/// 商品ID
	string		mGoodsID;
	/// 标题
	string		mTitle;
	/// 类型
	byte		mType;
	/// 游戏货币数量
	int			mGameCoin;
	/// 真实货币数量
	int			mCoinNum;
	/// 真实货币类型
	string		mCoinType;
	/// VIP经验
	int			mVIPExp;
	/// 渠道名
	string		mChannelName;
	/// 活动货币数量
	int			mActivityCoin;
};
class CRechargeConfig
{
public:
	typedef vector<CRechargeGoods> RECHARGE_GOODS_LIST;

public:
	/// 加载充值配置
	bool	LoadRechargeConfig(const char* pFilePath);
	/// 得到商品
	CRechargeGoods* getGoods(uint nCRC32, const char* pGoodsID);

public:
	RECHARGE_GOODS_LIST		mGoodsList;
	bool					mCheckChannelName;
};
#endif