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

/// 单人副本(用于union)
struct CSingleInstance
{
public:
	/// 初始化
	void	init();
	/// 创建
	void	create();
	/// 结束
	void	end();
	/// 销毁
	void	destroy();
	/// 发奖
	void	givePrize();
	/// 玩家进入
	void	playerEnter(CEntityPlayer& rPlayer);
};

/// 多人副本（包括团队本）(用于union)
struct CMultipleInstance
{
public:
	/// 初始化
	void	init();
	/// 创建
	void	create();
	/// 结束
	void	end();
	/// 销毁
	void	destroy();
	/// 发奖
	void	givePrize();
	/// 玩家进入
	void	playerEnter(CEntityPlayer& rPlayer);
};


/// 副本类
class CInstance : public CObj
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
		mType = emInstance_None;
	}
public:
	/// 创建
	void	create();
	/// 结束
	void	end();
	/// 销毁
	void	destroy();
	/// 发奖
	void	givePrize();
	/// 玩家进入
	void	playerEnter(CEntityPlayer& rPlayer);

public:
	/// autocode don't edit!!!
	CInstanceConfig* getConfig(){ return mConfig; }
	void setConfig(CInstanceConfig* value){ mConfig = value; }

	int getMapObjId(int nIndex)
	{
		if (nIndex < 0 || nIndex >= MAX_INSTANCE_MAP_NUM)
		{
			return 0;
		}
		return mMapObjId[nIndex];
	}
	void setMapObjId(int nIndex, int value)
	{
		if (nIndex < 0 || nIndex >= MAX_INSTANCE_MAP_NUM)
		{
			return;
		}
		mMapObjId[nIndex] = value;
	}

	time_t getCreateTime(){ return mCreateTime; }
	void setCreateTime(time_t value){ mCreateTime = value; }

	time_t getExpiredTime(){ return mExpiredTime; }
	void setExpiredTime(time_t value){ mExpiredTime = value; }

	EmInstanceStatus getStatus(){ return mStatus; }
	void setStatus(EmInstanceStatus value){ mStatus = value; }

	EmInstanceType getType(){ return mType; }
	void setType(EmInstanceType eType){mType = eType;}

	uint getInstanceID() const { return mInstanceID; }
	void setInstanceID(uint nValue) { mInstanceID = nValue; }
	/// end autocode

	CSingleInstance& getSingle(){return mSingle;}
	CMultipleInstance& getMultiple(){return mMultiple;}

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
	///  副本类型
	EmInstanceType		mType;
	///  副本ID
	uint				mInstanceID;

	// 不同的副本类型，不同的数据
	union
	{
		CSingleInstance		mSingle;
		CMultipleInstance	mMultiple;
	};
};

#endif