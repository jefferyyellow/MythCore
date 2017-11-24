#ifndef __DBJOB_H__
#define __DBJOB_H__
#include "job.h"
#include "mysqldatabase.h"
#include "socketstream.h"
#include "dbmessage.h"
using namespace Myth;

class CInternalMsg;
class CIMPlayerLoginMsg;
class CDBJob : public CJob<200, 20>
{
public:
	CDBJob();
	~CDBJob();
	int		init(char* pHost, char* pUserName, char* pPasswd, char* pDataBase, int nPort, char* pUnixSocket);

public:
	virtual void doing(uint32 uParam);

public:
	int		setBuffer(int nBufferSize);
	/// ѹ�빤������
	void	pushBackJobData(uint8* pData, int nDataLength);
	/// ȡ����������
	void	popBackJobData(uint8* pData, int &rLength);
	/// ����DB���������
	void	checkDBStream();
private:
	void	onTask(CInternalMsg* pMsg);
	/// ��ҵ�½��Ϸ
	void	onIMPlayerLoginRequest(CInternalMsg* pMsg);
	/// ��Ҵ�����ɫ
	void	onIMCreateRoleRequest(CInternalMsg* pMsg);
	// ��ҽ��볡��
	void	onIMEnterSceneRequest(CInternalMsg* pMsg);
	// ������ҵ�½��Ϣ
	void	copyPlayerLoginMsg(CIMPlayerLoginMsg* pDstMsg, CIMPlayerLoginMsg* pSrcMsg);

private:
	CMysqlDataBase	mDataBase;
	CSocketStream	mJobStream;
	uint8*			mJobBuffer;
	CSimpleLock		mJobStreamLock;
	CDBRequest		mDBRequest;
	CDBResponse		mDBResponse;
};
#endif
