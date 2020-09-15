#ifndef __INSTANCE_H__
#define __INSTANCE_H__
#include "instancetype.h"
#include "servercommon.h"
#include "obj.h"
#include "bit_set.h"
using namespace Myth;
class CEntityPlayer;
/// 副本配置类(只读取C++中需要项)
class CInstanceConfig
{
public:
	CInstanceConfig()
	{
		mType = 0;
		mID = 0;
		mTime = 0;
		memset(mMapId, 0, sizeof(mMapId));
	}
	~CInstanceConfig(){}
public:
	void loadConfig();

public:
	/// 副本类型
	short	mType;
	/// 副本ID
	short	mID;
	/// 时间
	uint	mTime;
	/// 副本地图ID
	int		mMapId[MAX_INSTANCE_MAP_NUM];
};

/// 单人副本(用于union)
struct CSingleInstance
{
public:
};

/// 多人副本（包括团队本）(用于union)
struct CMultipleInstance
{
public:

};


/// 副本类
class CInstance : public CObj
{
public:
	typedef CBitSet<emInstEventMax> INST_EVENT_BIT_SET;

public:
	CInstance()
	{
		init();
	}
	virtual ~CInstance(){}

	void			init()
	{
		memset(mMapObjId, 0, sizeof(mMapObjId));
		mCreateTime = 0;
		mExpiredTime = 0;
		mStatus = emInstanceStatus_None;
		mType = emInstance_None;
	}
public:
	/// 创建
	void	create(CInstanceConfig* pInstanceConfig);
	/// 结束
	void	end();
	/// 销毁
	void	destroy();
	/// 发奖
	void	givePrize(CEntityPlayer& rPlayer);
	/// 玩家进入
	void	playerEnter(CEntityPlayer& rPlayer);
	/// 事件发生
	void	onEvent(EmInstanceEvent eEventType, int nParam1, int nParam2);

public:
	/// autocode don't edit!!!
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

	time_t getDestoryTime() const { return mDestoryTime; }
	void setDestoryTime(time_t nValue) { mDestoryTime = nValue; }
	/// end autocode

	CSingleInstance& getSingle(){return mSingle;}
	CMultipleInstance& getMultiple(){return mMultiple;}

	/// 关心事件
	bool checkCareEvent(EmInstanceEvent eEvent){mCareEvent.getBit(eEvent);}
	void addCareEvent(EmInstanceEvent eEvent){mCareEvent.setBit(eEvent);}
	void clearCareEvent(EmInstanceEvent eEvent){mCareEvent.clearBit(eEvent);}

private:
	/// 地图实体Id
	int					mMapObjId[MAX_INSTANCE_MAP_NUM];
	/// 副本创建时间
	time_t				mCreateTime;
	/// 副本到期时间
	time_t				mExpiredTime;
	/// 销毁时间
	time_t				mDestoryTime;
	/// 状态 default:emInstanceStatus_None
	EmInstanceStatus	mStatus;
	///  副本类型
	EmInstanceType		mType;
	///  副本ID
	uint				mInstanceID;
	/// 副本关心事件
	INST_EVENT_BIT_SET	mCareEvent;
	// 不同的副本类型，不同的数据
	union
	{
		CSingleInstance		mSingle;
		CMultipleInstance	mMultiple;
	};
};

#endif