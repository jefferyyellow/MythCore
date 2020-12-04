#ifndef __LOGINJOB_H__
#define __LOGINJOB_H__
#include "job.h"
#include "logintype.h"
#include "timeutility.h"
using namespace Myth;
class CDBResponse;
class CLoginJob : public CJob
{
public:
	typedef std::map<short, int> LOGIN_LIST;

public:
	CLoginJob();
	~CLoginJob();

public:
	void	init();
	void	clear();

public:
	virtual void doing(int uParam);
	void onTimer();
	void checkLoginList(int nElapseTime);

public:
	void OnIMPlayerLoginRequest(CInternalMsg* pIMMsg);
	///  һ��Socket�Ͽ�
	void onSocketDisconnect(int nSocketIndex);
	/// �������ݿ�������ɫID���ֶ�
	void loadAllocateRoleId();
	/// �������ݿ�������ɫID�Ĵ���
	void onLoadAllocateRoleId(CDBResponse& rResponse);

	unsigned int getBeginRoleID(int nServerId)
	{
		if (nServerId <= 0 || nServerId > MAX_ROLE_PER_SERVER)
		{
			return 0;
		}
		return (nServerId - 1) * MAX_SERVER_ID + 1;
	}

	unsigned int allocateRoleID(int nServerId);

public:
	unsigned int getAllocateRoleId(int nIndex)
	{
		if (nIndex < 0 || nIndex >= MAX_SERVER_ID)
		{
			return 0;
		}

		return mAllocateRoleId[nIndex];
	}

	void		setAllocateRoleId(int nIndex, int nRoleID)
	{
		if (nIndex < 0 || nIndex >= MAX_SERVER_ID)
		{
			return;
		}
		mAllocateRoleId[nIndex] = nRoleID;
	}

private:
	LOGIN_LIST			mLoginList;						// ��¼�б�
	CAutoResetTimer		mLoginCheckTime;				// ��¼У��ʱ��
	unsigned int		mAllocateRoleId[MAX_SERVER_ID];	// ��ɫID������
	/// ��һ��ˢ�¼�ʱ����ʱ��
	uint64				mLastTimerTick;
};
#endif