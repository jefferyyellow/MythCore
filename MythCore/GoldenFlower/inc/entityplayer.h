#ifndef __ENTITYPLAYER_H__
#define __ENTITYPLAYER_H__
#include "servercommon.h"
#include "logintype.h"
#include "obj.h"
class PBPlayerSceneInfo;

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
	}
	~CEntityPlayer(){}

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
};
#endif 