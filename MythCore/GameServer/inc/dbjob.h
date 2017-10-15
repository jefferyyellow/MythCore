#ifndef __DBJOB_H__
#define __DBJOB_H__
#include "job.h"
#include "mysqldatabase.h"
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
};
#endif
