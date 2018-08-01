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
	CInstance()
	{
		init();
	}
	virtual ~CInstance(){}

	void			init()
	{
        mConfig = NULL;
        memset(mMapObjId, 0, sizeof(mMapObjId));
        mCreateTime = 0;
        mExpiredTime = 0;
        mStatus = emInstanceStatus_None;
        mSingle = false;
	}
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

public:
	/// autocode don't edit!!!
    CInstanceConfig* getConfig(){ return mConfig;}
    void setConfig(CInstanceConfig* value){ mConfig = value;}

    int getMapObjId(int nIndex)
    {
        if(nIndex < 0 || nIndex >= MAX_INSTANCE_MAP_NUM)
        {
            return 0;
        }
        return mMapObjId[nIndex];
    }
    void setMapObjId(int nIndex, int value)
    {
        if(nIndex < 0 || nIndex >= MAX_INSTANCE_MAP_NUM)
        {
            return;
        }
        mMapObjId[nIndex] = value;
    }

    time_t getCreateTime(){ return mCreateTime;}
    void setCreateTime(time_t value){ mCreateTime = value;}

    time_t getExpiredTime(){ return mExpiredTime;}
    void setExpiredTime(time_t value){ mExpiredTime = value;}

    EmInstanceStatus getStatus(){ return mStatus;}
    void setStatus(EmInstanceStatus value){ mStatus = value;}

    bool getSingle(){ return mSingle;}
    void setSingle(bool value){ mSingle = value;}

    EmInstanceType& getType(){ return mType;}
	/// end autocode

private:
	/// 副本配置
	CInstanceConfig*	mConfig;
	/// 地图实体Id
	int					mMapObjId[MAX_INSTANCE_MAP_NUM];
	/// 副本创建时间
	time_t				mCreateTime;
	/// 副本到期时间
	time_t				mExpiredTime;
	/// 状态 default:emInstanceStatus_None
	EmInstanceStatus	mStatus;
	/// 是否是单人副本
	bool				mSingle;
	///  副本类型
	EmInstanceType		mType;
};

/// 单人副本
class CSingleInstance : public CInstance
{
public:
	CSingleInstance(){}
	virtual ~CSingleInstance(){}
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

public:
	
};

/// 多人副本（包括团队本）
class CMultipleInstance : public CInstance
{
public:
	CMultipleInstance(){}
	virtual ~CMultipleInstance(){}

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

#endif