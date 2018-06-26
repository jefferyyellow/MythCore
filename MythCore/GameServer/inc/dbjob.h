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
	/// 压入工作数据
	void	pushBackJobData(byte* pData, int nDataLength);
	/// 取出工作数据
	void	popUpJobData(byte* pData, int &rLength);
	/// 处理DB流里的数据
	void	checkDBStream();
	/// 处理保存玩家基本属性
	int		onSavePlayerBaseProperty(int nLength);
	/// 分析PB结构，组成update的sql语句
	int		parsePBForSql(Message& rMessage);
	/// 分析PB结构，组成调用precedure语句
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
