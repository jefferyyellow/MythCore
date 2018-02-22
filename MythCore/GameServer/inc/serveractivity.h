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

class CServerActivity
{
public:
	CServerActivity()
	{
		
	}
	~CServerActivity()
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
        mState = emServerActState_End;
	}

public:
	// autocode
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

    EmServerActState getState(){ return mState;}
    void setState(EmServerActState value){ mState = value;}
	// end autocode
public:
	/// ���������ļ�
	virtual int loadActivity(XMLElement* pActivityElem)		= 0;
	/// �õ������ļ�������
	virtual const char* getConfigFileName()					= 0;
	/// �����
	virtual void start()									= 0;
	/// �����
	virtual void end()										= 0;
	/// ˢ���������
	virtual void refreshPlayerData(CEntityPlayer* pPlayer, int nParam) = 0;
	/// ����������
	virtual void clearPlayerData(CEntityPlayer* pPlayer)	= 0;
	virtual	void getActivityPrize(CEntityPlayer* pPlayer, int nParam)	= 0;

public:
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
	EmServerActState	mState;
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

	int					mCondNum;
	CServerActPrize		mPrize[ACTIVITY_PRIZE_NUM];
};

/// �����׶λ
class CPhaseActivity : public CServerActivity
{
public:
	typedef vector<CCondPrizeData>	VEC_COND_PRIZE_LIST;

public:
	CPhaseActivity()
	{
		
	}
	~CPhaseActivity()
	{
		
	}

public:
	/// ���������ļ�
	virtual int loadActivity(XMLElement* pActivityElem);
	/// �õ������ļ�������
	virtual const char* getConfigFileName();
	/// �����
	virtual void start();
	/// �����
	virtual void end();
	/// ˢ���������
	virtual void refreshPlayerData(CEntityPlayer* pPlayer, int nParam);
	/// ����������
	virtual  void clearPlayerData(CEntityPlayer* pPlayer);
	/// �õ������
	virtual	void getActivityPrize(CEntityPlayer* pPlayer, int nParam);
public:
	/// ���������б�
	VEC_COND_PRIZE_LIST		mCondPrizeList;
};

#endif
