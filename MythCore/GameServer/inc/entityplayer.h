#ifndef __ENTITYPLAYER_H__
#define __ENTITYPLAYER_H__
#include "entity.h"
#include "itemsubmodule.h"
class CEntityPlayer : public CEntityCharacter
{
public:
	CEntityPlayer(){}
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

	/// 等级
	uint32			getRoleLevel() const { return mRoleLevel; }
	void			setRoleLevel(uint32 nValue) { mRoleLevel = nValue; }
	/// 经验
	uint64			getRoleExp() const { return mRoleExp; }
	void			setRoleExp(uint64 nValue) { mRoleExp = nValue; }
	/// 与TCP交换用的头
	CExchangeHead	GetExhangeHead(){ return mExhangeHead; }
private:
	/// 角色ID
	uint32			mRoleID;
	/// 角色名字
	char			mName[PLAYER_NAME_LENGTH];
	/// 角色等级
	uint32			mRoleLevel;
	/// 角色经验
	uint64			mRoleExp;
	/// socket连接信息
	CExchangeHead	mExhangeHead;
};
#endif 