#ifndef __ENTITYPLAYER_H__
#define __ENTITYPLAYER_H__
#include "entity.h"
#include "propertyunit.h"
#include "itemunit.h"
#include "taskunit.h"
class CEntityPlayer : public CEntityCharacter
{
public:
	CEntityPlayer()
		:mPropertyUnit(this), mItemUnit(this), mTaskUnit(this)
	{

	}
	~CEntityPlayer(){}

public:
	/// 角色ID
	uint32			getRoleID(){ return mRoleID; }
	void			setRoleID(uint32 nRoleID){ mRoleID = nRoleID; }
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
	CExchangeHead	GetExhangeHead(){ return mExhangeHead; }
private:
	/// socket连接信息
	CExchangeHead	mExhangeHead;
	/// 属性单元
	CPropertyUnit	mPropertyUnit;
	/// 道具单元
	CItemUnit		mItemUnit;
	/// 任务单元
	CTaskUnit		mTaskUnit;

	/// 角色名字
	char			mName[PLAYER_NAME_LENGTH];
	/// 角色ID
	uint32			mRoleID;
	/// 上次下线时间
	time_t			mLastOffTime;
	/// 这次上线时间
	time_t			mOnTime;
};
#endif 