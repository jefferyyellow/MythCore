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
	/// ��ɫID
	uint32			getRoleID(){ return mRoleID; }
	void			setRoleID(uint32 nRoleID){ mRoleID = nRoleID; }
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

	/// �ȼ�
	uint32			getRoleLevel() const { return mRoleLevel; }
	void			setRoleLevel(uint32 nValue) { mRoleLevel = nValue; }
	/// ����
	uint64			getRoleExp() const { return mRoleExp; }
	void			setRoleExp(uint64 nValue) { mRoleExp = nValue; }
	/// ��TCP�����õ�ͷ
	CExchangeHead	GetExhangeHead(){ return mExhangeHead; }
private:
	/// ��ɫID
	uint32			mRoleID;
	/// ��ɫ����
	char			mName[PLAYER_NAME_LENGTH];
	/// ��ɫ�ȼ�
	uint32			mRoleLevel;
	/// ��ɫ����
	uint64			mRoleExp;
	/// socket������Ϣ
	CExchangeHead	mExhangeHead;
};
#endif 