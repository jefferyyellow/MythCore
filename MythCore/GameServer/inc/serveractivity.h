#ifndef __SERVERACTIVITY_H__
#define __SERVERACTIVITY_H__
#include "servercommon.h"
#include "serveractivitytype.h"

class CEntityPlayer;
class CServerActPrize
{
public:
	CServerActPrize()
	{
		mItemID = 0;
		mItemNum = 0;
	}

	~CServerActPrize()
	{
	}

	int		mItemID;
	int		mItemNum;
};
typedef vector<CServerActPrize>	ACT_PRIZE_LIST;

class CServerActivity
{
public:
	CServerActivity()
	{
		
	}
	virtual ~CServerActivity()
	{
		
	}

	void init()
	{
        mType = 0;
        mSubType = 0;
        mID = 0;
        mStartTime = 0;
        mEndTime = 0;
        mPrizeTime = 0;
        mState = emSvrActState_End;
		mAvail = true;
	}

public:
	/// autocode don't edit
    byte getType(){ return mType;}
    void setType(byte value){ mType = value;}

    byte getSubType(){ return mSubType;}
    void setSubType(byte value){ mSubType = value;}

    short getID(){ return mID;}
    void setID(short value){ mID = value;}

    time_t getStartTime(){ return mStartTime;}
    void setStartTime(time_t value){ mStartTime = value;}

    time_t getEndTime(){ return mEndTime;}
    void setEndTime(time_t value){ mEndTime = value;}

    time_t getPrizeTime(){ return mPrizeTime;}
    void setPrizeTime(time_t value){ mPrizeTime = value;}

    EmSvrActState getState(){ return mState;}
    void setState(EmSvrActState value){ mState = value;}

	bool getAvail() const { return mAvail; }
	void setAvail(bool nValue) { mAvail = nValue; }
	// end autocode
public:
	/// ���������ļ�
	virtual int loadActivity(XMLElement* pActivityElem)		= 0;
	/// �õ������ļ�������
	virtual const char* getConfigFileName()					= 0;
	/// �����
	virtual void start()									= 0;
	/// �����������
	virtual void end()										= 0;
	/// �����������
	virtual	void clearPlayerData(CEntityPlayer& rPlayer)	= 0;
	/// ˢ�»����
	virtual void refreshProcess(CEntityPlayer& rPlayer, int nParam1, int nParam2) = 0;

protected:
	/// ����
	byte				mType;
	/// ��������
	byte				mSubType;
	/// ID
	short				mID;
	/// ��ʼʱ��
	time_t				mStartTime;
	/// ����ʱ�� 
	time_t				mEndTime;
	/// �콱ʱ��
	time_t				mPrizeTime;
	/// �����״̬ default:emServerActState_End
	EmSvrActState	mState;
	/// �Ƿ�����Ч�
	bool				mAvail;
};


class CCondPrizeData
{
public:
	CCondPrizeData()
	{
		mCondNum = 0;
	}
	~CCondPrizeData()
	{}

	/// ����
	int					mCondNum;
	/// �����б�
	ACT_PRIZE_LIST		mPrizeList;
};

class CPhasePrizeActivity : public CServerActivity
{
public:
	typedef vector<CCondPrizeData>	COND_PRIZE_DATA_LIST;

public:
	/// ���������ļ�
	virtual int loadActivity(XMLElement* pActivityElem);
	/// �õ������ļ�������
	virtual const char* getConfigFileName() = 0;
	/// �����
	virtual void start() = 0;
	/// �����������
	virtual void end() = 0;
	/// �����������
	virtual	void clearPlayerData(CEntityPlayer& rPlayer) = 0;
	/// ˢ�»����
	virtual void refreshProcess(CEntityPlayer& rPlayer, int nParam1, int nParam2) = 0;
	
	/// �¼ӵ��麯��
	/// �õ������
	virtual	void getActivityPrize(CEntityPlayer& rPlayer, int nIndex) = 0;
public:
	CCondPrizeData*	getCondPrizeData(uint nIndex)
	{
		if (nIndex >= mCondPrizeList.size())
		{
			return NULL;
		}
		return &mCondPrizeList[nIndex];
	}

protected:
	COND_PRIZE_DATA_LIST			mCondPrizeList;
};

// �ۼƳ�ֵ
class CCumulativeRecharge : public CPhasePrizeActivity
{
public:
	/// ���������ļ�
	virtual int loadActivity(XMLElement* pActivityElem);
	/// �õ������ļ�������
	virtual const char* getConfigFileName();
	/// �����
	virtual void start(){};
	/// �����������
	virtual void end(){};
	/// �����������
	virtual	void clearPlayerData(CEntityPlayer& rPlayer);
	/// ˢ�»����
	virtual void refreshProcess(CEntityPlayer& rPlayer, int nParam1, int nParam2);
	/// �õ������
	virtual	void getActivityPrize(CEntityPlayer& rPlayer, int nIndex);
};

/// �ۼ�����
class CCumulativeConsume : public CPhasePrizeActivity
{
public:
	/// ���������ļ�
	virtual int loadActivity(XMLElement* pActivityElem);
	/// �õ������ļ�������
	virtual const char* getConfigFileName();
	/// �����
	virtual void start(){};
	/// �����������
	virtual void end(){};
	/// �����������
	virtual	void clearPlayerData(CEntityPlayer& rPlayer);
	/// ˢ�»����
	virtual void refreshProcess(CEntityPlayer& rPlayer, int nParam1, int nParam2);
	/// �õ������
	virtual	void getActivityPrize(CEntityPlayer& rPlayer, int nIndex);
};


class CRankPrizeData
{
public:
	CRankPrizeData()
	{
		mMinRank = -1;
		mMaxRank = -1;
	}
	~CRankPrizeData()
	{}

	/// ��С����
	int					mMinRank;
	/// �������
	int					mMaxRank;
	/// �����б�
	ACT_PRIZE_LIST		mPrizeList;
};
class CRankActivity : public CServerActivity
{
public:
	typedef vector<CRankPrizeData>	RANK_PRIZE_DATA_LIST;

public:
	/// ���������ļ�
	virtual int loadActivity(XMLElement* pActivityElem);
	/// �õ������ļ�������
	virtual const char* getConfigFileName();
	/// �����
	virtual void start(){};
	/// �����������
	virtual void end(){};
	/// �����������
	virtual	void clearPlayerData(CEntityPlayer& rPlayer);
	/// ˢ�»����
	virtual void refreshProcess(CEntityPlayer& rPlayer, int nParam1, int nParam2){}
	/// �õ������
	void getActivityPrize(CEntityPlayer& rPlayer);

public:
	RANK_PRIZE_DATA_LIST			mRankPrizeList;
};

#endif
