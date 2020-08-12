#ifndef __PLATCONFIG_H__
#define __PLATCONFIG_H__
#include "commontype.h"
#include <string>
#include <vector>
using namespace std;

enum EmRechargeType
{
	emRechargeType_None				= 0,	// δ֪
	emRechargeType_Diamond			= 1,	// ��ʯ
};
class CRechargeGoods
{
public:
	/// ��ƷID��CRC32����
	uint		mGoodsIDCRC;
	/// ��ƷID
	string		mGoodsID;
	/// ����
	string		mTitle;
	/// ����
	byte		mType;
	/// ��Ϸ��������
	int			mGameCoin;
	/// ��ʵ��������
	int			mCoinNum;
	/// ��ʵ��������
	string		mCoinType;
	/// VIP����
	int			mVIPExp;
	/// ������
	string		mChannelName;
	/// ���������
	int			mActivityCoin;
};
class CRechargeConfig
{
public:
	typedef vector<CRechargeGoods> RECHARGE_GOODS_LIST;

public:
	/// ���س�ֵ����
	bool	LoadRechargeConfig(const char* pFilePath);
	/// �õ���Ʒ
	CRechargeGoods* getGoods(uint nCRC32, const char* pGoodsID);

public:
	RECHARGE_GOODS_LIST		mGoodsList;
	bool					mCheckChannelName;
};
#endif