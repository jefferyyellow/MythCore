#ifndef __INSTANCE_H__
#define __INSTANCE_H__
#include "instancetype.h"
#include "servercommon.h"
#include "obj.h"
class CEntityPlayer;
/// ����������
class CInstanceConfig
{

public:
	/// ��������
	int		mType;
	/// ����ID
	int		mID;
	/// ʱ��
	int		mTime;
	/// ��ҵȼ�Ҫ��
	int		mPlayerLevel;
	/// ������ͼID
	int		mMapId[MAX_INSTANCE_MAP_NUM];
};

/// ������
class CInstance : CObj
{
public:
	CInstance(){}
	virtual ~CInstance(){}

public:
	/// ����
	virtual void	create();
	/// ����
	virtual void	end();
	/// ����
	virtual void	destroy();
	/// ����
	virtual void	givePrize();
	/// ��ҽ���
	virtual	void	playerEnter(CEntityPlayer* pPlayer);
	/// ����뿪
	virtual void	playerLeave(CEntityPlayer* pPlayer);

private:
	/// ��������
	CInstanceConfig*	mConfig;
	/// ��ͼʵ��Id
	int					mMapObjId[MAX_INSTANCE_MAP_NUM];
	/// ��������ʱ��
	time_t				mCreateTime;
	/// ��������ʱ��
	time_t				mExpiredTime;
	/// ״̬
	EmInstanceStatus	mStatus;
};

/// ��ͨ����
class CCommonInstance : public CInstance
{
public:
	CCommonInstance(){}
	virtual ~CCommonInstance(){}
public:
	/// ����
	virtual void	create();
	/// ����
	virtual void	end();
	/// ����
	virtual void	destroy();
	/// ����
	virtual void	givePrize();
	/// ��ҽ���
	virtual	void	playerEnter(CEntityPlayer* pPlayer);
	/// ����뿪
	virtual void	playerLeave(CEntityPlayer* pPlayer);
};


union CInstanceUnion
{
	char mCommonInstance[sizeof(CCommonInstance)];
};
#endif