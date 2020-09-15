#ifndef __INSTANCE_H__
#define __INSTANCE_H__
#include "instancetype.h"
#include "servercommon.h"
#include "obj.h"
#include "bit_set.h"
using namespace Myth;
class CEntityPlayer;
/// ����������(ֻ��ȡC++����Ҫ��)
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
	/// ��������
	short	mType;
	/// ����ID
	short	mID;
	/// ʱ��
	uint	mTime;
	/// ������ͼID
	int		mMapId[MAX_INSTANCE_MAP_NUM];
};

/// ���˸���(����union)
struct CSingleInstance
{
public:
};

/// ���˸����������Ŷӱ���(����union)
struct CMultipleInstance
{
public:

};


/// ������
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
	/// ����
	void	create(CInstanceConfig* pInstanceConfig);
	/// ����
	void	end();
	/// ����
	void	destroy();
	/// ����
	void	givePrize(CEntityPlayer& rPlayer);
	/// ��ҽ���
	void	playerEnter(CEntityPlayer& rPlayer);
	/// �¼�����
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

	/// �����¼�
	bool checkCareEvent(EmInstanceEvent eEvent){mCareEvent.getBit(eEvent);}
	void addCareEvent(EmInstanceEvent eEvent){mCareEvent.setBit(eEvent);}
	void clearCareEvent(EmInstanceEvent eEvent){mCareEvent.clearBit(eEvent);}

private:
	/// ��ͼʵ��Id
	int					mMapObjId[MAX_INSTANCE_MAP_NUM];
	/// ��������ʱ��
	time_t				mCreateTime;
	/// ��������ʱ��
	time_t				mExpiredTime;
	/// ����ʱ��
	time_t				mDestoryTime;
	/// ״̬ default:emInstanceStatus_None
	EmInstanceStatus	mStatus;
	///  ��������
	EmInstanceType		mType;
	///  ����ID
	uint				mInstanceID;
	/// ���������¼�
	INST_EVENT_BIT_SET	mCareEvent;
	// ��ͬ�ĸ������ͣ���ͬ������
	union
	{
		CSingleInstance		mSingle;
		CMultipleInstance	mMultiple;
	};
};

#endif