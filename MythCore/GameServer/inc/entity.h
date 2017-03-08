#ifndef __ENTITY_H__
#define __ENTITY_H__
#include "obj.h"
#include "logintype.h"
#define PLAYER_NAME_LENGTH   32
class CEntity : public CObj
{
public:
	CEntity(){}
	~CEntity(){}
	
protected:

};

class CEntityCharacter : public CEntity
{
public:
	CEntityCharacter(){}
	~CEntityCharacter(){}

};

class CEntityPlayer : public CEntityCharacter
{
public:
	CEntityPlayer(){}
	~CEntityPlayer(){}

public:
	/// ��ɫID
	uint32			getRoleID(){return mRoleID;}
	void			setRoleID(uint32 nRoleID){mRoleID = nRoleID;}
	/// ��ɫ����
	char*			getName(){return mName;}
	void			setName(char* pName)
	{
		if (NULL == pName)
		{
			return;
		}
		strncpy(mName, pName, sizeof(mName) - 1);
	}

	/// �ȼ�
	uint32			getRoleLevel() const { return mRoleLevel; }
	void			setRoleLevel(uint32 nValue) { mRoleLevel = nValue; }
	/// ����
	uint64			getRoleExp() const { return mRoleExp; }
	void			setRoleExp(uint64 nValue) { mRoleExp = nValue; }

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