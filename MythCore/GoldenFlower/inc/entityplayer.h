#ifndef __ENTITYPLAYER_H__
#define __ENTITYPLAYER_H__
#include "servercommon.h"
#include "logintype.h"
#include "obj.h"
class PBPlayerSceneInfo;
#define MAX_PLAYER_POKER_NUM 3
enum EmPokerStatus
{
	emPokerStatus_None		= 0,
	emPokerStatus_Play		= 1,
	emPokerStatus_Discard	= 2,
};

class CEntityPlayer : public CObj
{
public:
	CEntityPlayer()
	{
		mName[0] = '\0';
		mRoleID = 0;
		mLastOffTime = 0;
		mOnTime = 0;
		mPlayerStauts = 0;
		mLoadStatus = 0;
		mPokerNum = 0;
		mReady = false;
	}
	~CEntityPlayer(){}

	void clear()
	{
		mPokerNum = 0;
		mRoomID = 0;
		mReady = false;
	}
public:
	/// ��ɫID
	unsigned int	getRoleID(){ return mRoleID; }
	void			setRoleID(unsigned int nRoleID){ mRoleID = nRoleID; }

	/// �ϴ����ߵ�ʱ��
	time_t getLastOffTime() const { return mLastOffTime; }
	void setLastOffTime(time_t val) { mLastOffTime = val; }

	/// ��ɫ����
	char*			getName(){ return mName; }
	void			setName(char* pName)
	{
		if (NULL == pName)
		{
			return;
		}
		strncpy(mName, pName, sizeof(mName)-1);
	}

	/// ��TCP�����õ�ͷ
	CExchangeHead&	GetExhangeHead(){ return mExhangeHead; }

	/// �ϴεĴ洢ʱ��
	time_t			getLastSaveTime() const{return mLastSaveTime;}
	void			setLastSaveTime(time_t val)	{mLastSaveTime = val;}

	/// ���״̬
	byte			getPlayerStauts(){ return mPlayerStauts;	}
	void			setPlayerStauts(byte val){ mPlayerStauts = val;}

	/// �洢״̬
	byte			getSaveStatus()const{return mSaveStatus;}
	void			setSaveStatus(byte val){mSaveStatus = val;}
	void			setSaveStatusBit(byte val){mSaveStatus |= val;}

	/// ����״̬
	byte			getLoadStatus()const{return mLoadStatus;}
	void			setLoadStatus(byte val){mLoadStatus = val;}
	void			setLoadStatusBit(byte val){mLoadStatus |= val;}
	  
	time_t getOnTime() const { return mOnTime; }
	void setOnTime(time_t nValue) { mOnTime = nValue; }

	void			addPoker(byte nPoker);

	int getRoomID() const { return mRoomID; }
	void setRoomID(int val) { mRoomID = val; }

	bool getReady() const { return mReady; }
	void setReady(bool val) { mReady = val; }

	byte getPoker(int nIndex)
	{
		if (nIndex < 0 || nIndex >= mPokerNum)
		{
			return -1;
		}
		return mPoker[nIndex];
	}

	void setPoker(int nIndex, byte nValue)
	{
		if (nIndex < 0 || nIndex >= mPokerNum)
		{
			return;
		}
		mPoker[nIndex] = nValue;
	}

	EmPokerStatus getPokerStatus() const { return mPokerStatus; }
	void setPokerStatus(EmPokerStatus val) { mPokerStatus = val; }

private:
	/// socket������Ϣ
	CExchangeHead	mExhangeHead;
	/// ��ɫ����
	char			mName[PLAYER_NAME_LENGTH];
	/// ��ɫID
	unsigned int	mRoleID;
	/// �ϴ�����ʱ��
	time_t			mLastOffTime;
	/// �������ʱ��
	time_t			mOnTime;
	/// �ϴδ��̵�ʱ��
	time_t			mLastSaveTime;
	/// ���״̬ EmPlayerStatus
	byte			mPlayerStauts;
	/// ��Ҵ洢״̬ EmPlayerSaveStatus
	byte			mSaveStatus;
	/// ��Ҽ���״̬ EmPlayerLoadStatus
	byte			mLoadStatus;

	/// ����������
	byte			mPoker[MAX_PLAYER_POKER_NUM];
	/// ��������Ƶ���Ŀ
	byte			mPokerNum;
	/// ��ҵķ���ID
	int				mRoomID;
	/// �Ƿ�׼��
	bool			mReady;
	/// �Ƶ�״̬
	EmPokerStatus	mPokerStatus;
};
#endif 