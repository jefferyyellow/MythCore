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
class CDBJob : public CJob
{
public:
	CDBJob();
	~CDBJob();
	void	init();
	int		initDB(const char* pHost, const char* pUserName, const char* pPasswd, 
				   const char* pDataBase, int nPort, const char* pUnixSocket);
	void	clear();

public:
	virtual void doing(int uParam);

public:
	/// 处理DB流里的数据
	void	checkDBTask();
	/// 分析PB结构，组成update的sql语句
	int		parsePBForSql(const Message& rMessage);
	/// 分析PB结构，组成调用precedure语句
	int		parsePBForPrecedure(const Message& rMessage);
	/// 处理保存玩家基本属性
	int		onSavePlayerBaseProperty(const Message* pMessage);
	/// 处理保存玩家邮件
	int		onSaveMail(const Message* pMessage);
	/// 处理保存全局邮件
	int		onSaveGlobalMail(const Message* pMessage);
private:
	CMysqlDataBase	mDataBase;
	CSimpleLock		mJobStreamLock;
	CDBRequest		mDBRequest;
	CDBResponse		mDBResponse;
	int				mSqlLength;
};
#endif
