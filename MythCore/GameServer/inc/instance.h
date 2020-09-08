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

/// ���˸���(����union)
struct CSingleInstance
{
public:
	/// ��ʼ��
	void	init();
	/// ����
	void	create();
	/// ����
	void	end();
	/// ����
	void	destroy();
	/// ����
	void	givePrize();
	/// ��ҽ���
	void	playerEnter(CEntityPlayer& rPlayer);
};

/// ���˸����������Ŷӱ���(����union)
struct CMultipleInstance
{
public:
	/// ��ʼ��
	void	init();
	/// ����
	void	create();
	/// ����
	void	end();
	/// ����
	void	destroy();
	/// ����
	void	givePrize();
	/// ��ҽ���
	void	playerEnter(CEntityPlayer& rPlayer);
};


/// ������
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
	/// ����
	void	create();
	/// ����
	void	end();
	/// ����
	void	destroy();
	/// ����
	void	givePrize();
	/// ��ҽ���
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
	/// ��������
	CInstanceConfig*	mConfig;
	/// ��ͼʵ��Id
	int					mMapObjId[MAX_INSTANCE_MAP_NUM];
	/// ��������ʱ��
	time_t				mCreateTime;
	/// ��������ʱ��
	time_t				mExpiredTime;
	/// ״̬ default:emInstanceStatus_None
	EmInstanceStatus	mStatus;
	///  ��������
	EmInstanceType		mType;
	///  ����ID
	uint				mInstanceID;

	// ��ͬ�ĸ������ͣ���ͬ������
	union
	{
		CSingleInstance		mSingle;
		CMultipleInstance	mMultiple;
	};
};

#endif