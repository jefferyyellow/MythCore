#ifndef __VIPUNIT_H__
#define __VIPUNIT_H__
#include "playersubunit.h"
#include "servercommon.h"
class CEntityPlayer;
class CVIPUnit : public CPlayerSubUnit
{
public:
	CVIPUnit(CEntityPlayer& rPlayer)
		:CPlayerSubUnit(rPlayer)
	{
		init();
	}
	~CVIPUnit()
	{

	}

	void init()
	{
		mVipExp = 0;
		mVipLevel = 0;
	}

public:
	/// ���VIP����
	void obtainVIPExp(int nVIPExp);
	/// �����ֵ
	void processRecharge(char* pGoodsID, char* pOrderID, double dRechargeMoney);
public:
	/// autocode, don't edit
	int getVipExp(){ return mVipExp; }
	void setVipExp(int value){ mVipExp = value; }

	byte getVipLevel(){ return mVipLevel; }
	void setVipLevel(byte value){ mVipLevel = value; }
	/// end autocode
private:
	/// Vip����
	int				mVipExp;
	/// Vip�ȼ�
	byte			mVipLevel;
};
#endif
