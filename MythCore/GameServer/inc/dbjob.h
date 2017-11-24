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
	/// 压入工作数据
	void	pushBackJobData(uint8* pData, int nDataLength);
	/// 取出工作数据
	void	popBackJobData(uint8* pData, int &rLength);
	/// 处理DB流里的数据
	void	checkDBStream();
private:
	void	onTask(CInternalMsg* pMsg);
	/// 玩家登陆游戏
	void	onIMPlayerLoginRequest(CInternalMsg* pMsg);
	/// 玩家创建角色
	void	onIMCreateRoleRequest(CInternalMsg* pMsg);
	// 玩家进入场景
	void	onIMEnterSceneRequest(CInternalMsg* pMsg);
	// 拷贝玩家登陆消息
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
