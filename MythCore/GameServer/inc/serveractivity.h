#ifndef __SERVERACTIVITY_H__
#define __SERVERACTIVITY_H__
#include "servercommon.h"
#include "serveractivitytype.h"

class CEntityPlayer;
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

	const char* getEndFunc(){return mEndFunc;}
	void setEndFunc(const char* pEndFunc)
	{
		if (NULL == pEndFunc)
		{
			return;
		}
		strncpy(mEndFunc, pEndFunc, sizeof(mEndFunc) - 1);
	}

	const char* getClearPlayerData(){ return mEndFunc; }
	void setClearPlayerData(const char* pClearPlayerData)
	{
		if (NULL == pClearPlayerData)
		{
			return;
		}
		strncpy(mClearPlayerData, pClearPlayerData, sizeof(mClearPlayerData) - 1);
	}

	const char* getRefreshProcess(){ return mRefreshProcess; }
	void setRefreshProcess(const char* pRefreshProcess)
	{
		if (NULL == pRefreshProcess)
		{
			return;
		}
		strncpy(mRefreshProcess, pRefreshProcess, sizeof(mRefreshProcess) - 1);
	}
	// end autocode

protected:
	/// ID
	short				mID;
	/// ����
	byte				mType;
	/// ��������
	byte				mSubType;
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
	/// ��������		
	char				mEndFunc[STR_LENGTH_128];
	/// ����������ݺ���
	char				mClearPlayerData[STR_LENGTH_128];
	/// ˢ�»����
	char				mRefreshProcess[STR_LENGTH_128];
};


#endif
