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
	int		initDB(const char* pHost, const char* pUserName, const char* pPasswd, 
				   const char* pDataBase, int nPort, const char* pUnixSocket);
	void	clear();

public:
	virtual void doing(int uParam);

public:
	int		setBuffer(int nBufferSize);
	/// ѹ�빤������
	void	pushBackJobData(const byte* pData, int nDataLength);
	/// ȡ����������
	void	popUpJobData(byte* pData, int &rLength);
	/// ��鹤�������Ƿ�Ϊ��
	int		checkJobDataEmpty();
	/// ����DB���������
	void	checkDBStream();
	/// ����PB�ṹ�����update��sql���
	int		parsePBForSql(const Message& rMessage);
	/// ����PB�ṹ����ɵ���precedure���
	int		parsePBForPrecedure(const Message& rMessage);
	/// ��������һ�������
	int		onSavePlayerBaseProperty(int nLength);
	/// ����������ʼ�
	int		onSaveMail(int nLength);
	/// ������ȫ���ʼ�
	int		onSaveGlobalMail(int nLength);
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
