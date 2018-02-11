#ifndef __SERVERACTIVITY_H__
#define __SERVERACTIVITY_H__
#include "servercommon.h"
#include "serveractivitytype.h"

class CEntityPlayer;


class CServerActPrize
{
public:
	int		mItemID[ACTIVITY_PRIZE_NUM];
	int		mItemNum[ACTIVITY_PRIZE_NUM];
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

    int getStartTime(){ return mStartTime;}
    void setStartTime(int value){ mStartTime = value;}

    int getEndTime(){ return mEndTime;}
    void setEndTime(int value){ mEndTime = value;}

    EmServerActState getState(){ return mState;}
    void setState(EmServerActState value){ mState = value;}
	// end autocode
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

public:
	/// ����
	byte				mType;
	/// ��������
	byte				mSubType;
	/// ID
	short				mID;
	/// ��ʼʱ��
	int					mStartTime;
	/// ����ʱ�� 
	int					mEndTime;
	/// �����״̬ default:emServerActState_End
	EmServerActState	mState;
};

/// �����׶λ
class CPhaseActivity : public CServerActivity
{
public:
	typedef vector<int>					VEC_COND_LIST;
	typedef vector<CServerActPrize>		VEC_PRIZE_LIST;
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

public:
	/// �����б�
	VEC_COND_LIST			mCondList;
	/// �����б�
	VEC_PRIZE_LIST			mPrizeList;
};

#endif
