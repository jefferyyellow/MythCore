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
		mInvalid = false;
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

    EmServerActState getState(){ return mState;}
    void setState(EmServerActState value){ mState = value;}

	bool getInvalid() const { return mInvalid; }
	void setInvalid(bool nValue) { mInvalid = nValue; }

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
	EmServerActState	mState;
	/// �Ƿ�����Ч�
	bool				mInvalid;
};

#endif
