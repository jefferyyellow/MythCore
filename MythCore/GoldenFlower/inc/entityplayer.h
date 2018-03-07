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
	/// 角色ID
	unsigned int	getRoleID(){ return mRoleID; }
	void			setRoleID(unsigned int nRoleID){ mRoleID = nRoleID; }

	/// 上次下线的时间
	time_t getLastOffTime() const { return mLastOffTime; }
	void setLastOffTime(time_t val) { mLastOffTime = val; }

	/// 角色名字
	char*			getName(){ return mName; }
	void			setName(char* pName)
	{
		if (NULL == pName)
		{
			return;
		}
		strncpy(mName, pName, sizeof(mName)-1);
	}

	/// 与TCP交换用的头
	CExchangeHead&	GetExhangeHead(){ return mExhangeHead; }

	/// 上次的存储时间
	time_t			getLastSaveTime() const{return mLastSaveTime;}
	void			setLastSaveTime(time_t val)	{mLastSaveTime = val;}

	/// 玩家状态
	byte			getPlayerStauts(){ return mPlayerStauts;	}
	void			setPlayerStauts(byte val){ mPlayerStauts = val;}

	/// 存储状态
	byte			getSaveStatus()const{return mSaveStatus;}
	void			setSaveStatus(byte val){mSaveStatus = val;}
	void			setSaveStatusBit(byte val){mSaveStatus |= val;}

	/// 加载状态
	byte			getLoadStatus()const{return mLoadStatus;}
	void			setLoadStatus(byte val){mLoadStatus = val;}
	void			setLoadStatusBit(byte val){mLoadStatus |= val;}
	  
	time_t getOnTime() const { return mOnTime; }
	void setOnTime(time_t nValue) { mOnTime = nValue; }

private:
	/// socket连接信息
	CExchangeHead	mExhangeHead;
	/// 角色名字
	char			mName[PLAYER_NAME_LENGTH];
	/// 角色ID
	unsigned int	mRoleID;
	/// 上次下线时间
	time_t			mLastOffTime;
	/// 这次上线时间
	time_t			mOnTime;
	/// 上次存盘的时间
	time_t			mLastSaveTime;
	/// 玩家状态 EmPlayerStatus
	byte			mPlayerStauts;
	/// 玩家存储状态 EmPlayerSaveStatus
	byte			mSaveStatus;
	/// 玩家加载状态 EmPlayerLoadStatus
	byte			mLoadStatus;
};
#endif 