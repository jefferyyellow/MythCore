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
	/// ����DB���������
	void	checkDBTask();
	/// ����PB�ṹ�����update��sql���
	int		parsePBForSql(const Message& rMessage);
	/// ����PB�ṹ����ɵ���precedure���
	int		parsePBForPrecedure(const Message& rMessage);
	/// ��������һ�������
	int		onSavePlayerBaseProperty(const Message* pMessage);
	/// ����������ʼ�
	int		onSaveMail(const Message* pMessage);
	/// ������ȫ���ʼ�
	int		onSaveGlobalMail(const Message* pMessage);
private:
	CMysqlDataBase	mDataBase;
	CSimpleLock		mJobStreamLock;
	CDBRequest		mDBRequest;
	CDBResponse		mDBResponse;
	int				mSqlLength;
};
#endif
