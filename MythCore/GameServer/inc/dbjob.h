#ifndef __DBJOB_H__
#define __DBJOB_H__
#include "job.h"
#include "mysqldatabase.h"
#include "bytestream.h"
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
	virtual void doing(int uParam);

public:
	int		setBuffer(int nBufferSize);
	/// ѹ�빤������
	void	pushBackJobData(byte* pData, int nDataLength);
	/// ȡ����������
	void	popUpJobData(byte* pData, int &rLength);
	/// ����DB���������
	void	checkDBStream();
private:
	void	onTask(CInternalMsg* pMsg);

private:
	CMysqlDataBase	mDataBase;
	CByteStream		mJobStream;
	byte*			mJobBuffer;
	CSimpleLock		mJobStreamLock;
	CDBRequest		mDBRequest;
	CDBResponse		mDBResponse;
};
#endif
