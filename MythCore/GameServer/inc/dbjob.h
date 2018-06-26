#ifndef __DBJOB_H__
#define __DBJOB_H__
#include "job.h"
#include "mysqldatabase.h"
#include "bytestream.h"
#include "dbmessage.h"
#include "messagefactory.h"
using namespace Myth;

class CInternalMsg;
class CIMPlayerLoginMsg;
class CDBJob : public CJob<100, 100>
{
public:
	CDBJob();
	~CDBJob();
	void	init();
	int		initDB(char* pHost, char* pUserName, char* pPasswd, char* pDataBase, int nPort, char* pUnixSocket);

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
	/// ��������һ�������
	int		onSavePlayerBaseProperty(int nLength);
	/// ����PB�ṹ�����update��sql���
	int		parsePBForSql(Message& rMessage);
	/// ����PB�ṹ����ɵ���precedure���
	int		parsePBForPrecedure(Message& rMessage);
private:
	void	onTask(CInternalMsg* pMsg);

private:
	CMysqlDataBase	mDataBase;
	CByteStream		mJobStream;
	byte*			mJobBuffer;
	CSimpleLock		mJobStreamLock;
	CDBRequest		mDBRequest;
	CDBResponse		mDBResponse;
	int				mSqlLength;
};
#endif
