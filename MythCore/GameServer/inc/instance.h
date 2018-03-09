#ifndef __INSTANCE_H__
#define __INSTANCE_H__
#include "instancetype.h"
#include "servercommon.h"
#include "obj.h"
class CEntityPlayer;
/// 副本配置类
class CInstanceConfig
{

public:
	/// 副本类型
	int		mType;
	/// 副本ID
	int		mID;
	/// 时间
	int		mTime;
	/// 玩家等级要求
	int		mPlayerLevel;
	/// 副本地图ID
	int		mMapId[MAX_INSTANCE_MAP_NUM];
};

/// 副本类
class CInstance : CObj
{
public:
	CInstance(){}
	virtual ~CInstance(){}

public:
	/// 创建
	virtual void	create();
	/// 结束
	virtual void	end();
	/// 销毁
	virtual void	destroy();
	/// 发奖
	virtual void	givePrize();
	/// 玩家进入
	virtual	void	playerEnter(CEntityPlayer* pPlayer);
	/// 玩家离开
	virtual void	playerLeave(CEntityPlayer* pPlayer);

private:
	/// 副本配置
	CInstanceConfig*	mConfig;
	/// 地图实体Id
	int					mMapObjId[MAX_INSTANCE_MAP_NUM];
	/// 副本创建时间
	time_t				mCreateTime;
	/// 副本到期时间
	time_t				mExpiredTime;
	/// 状态
	EmInstanceStatus	mStatus;
};

/// 普通副本
class CCommonInstance : public CInstance
{
public:
	CCommonInstance(){}
	virtual ~CCommonInstance(){}
public:
	/// 创建
	virtual void	create();
	/// 结束
	virtual void	end();
	/// 销毁
	virtual void	destroy();
	/// 发奖
	virtual void	givePrize();
	/// 玩家进入
	virtual	void	playerEnter(CEntityPlayer* pPlayer);
	/// 玩家离开
	virtual void	playerLeave(CEntityPlayer* pPlayer);
};


union CInstanceUnion
{
	char mCommonInstance[sizeof(CCommonInstance)];
};
#endif