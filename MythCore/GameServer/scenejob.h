#ifndef __SCENEJOB_H__
#define __SCENEJOB_H__
#include "job.h"
#include "sharememory.h"
#include "socketstream.h"
#include "messagefactory.h"
using namespace Myth;


#define PIPE_SIZE					((int)0x1000000)	/*�ڴ�ܵ��Ĵ�С*/
#define MAX_SOCKET_BUFF_SIZE		4096				// Socket��������С

struct CExchangeHead
{
	uint32	mTcpIndex;
	uint8	mTcpState;
};
class CSceneJob : public CJob < 1000, 100 >
{
public:
	CSceneJob(){}
	~CSceneJob(){}

public:
	bool		init();

public:
	virtual void doing(uint32 uParam);

private:
	void	onTask(CInternalMsg* pMsg);
	/// ����ǰ����Ϣ
	void		processClientMessage();
	/// ����ǰ����Ϣ
	void		sendClientMessage(uint nSocketIndex, uint nSocketTime, unsigned short nMessageID, Message* pMessage);
	/// �ַ�ǰ����Ϣ
	void		dispatchClientMessage(unsigned short nMessageID, Message* pMessage);
	/// ��ʼ�������ڴ�
	bool		initShareMemory();

private:
	CShareMemory*			mShareMemory;
	CSocketStream*			mTcp2ServerMemory;
	CSocketStream*			mServer2TcpMemory;
	char					mBuffer[MAX_SOCKET_BUFF_SIZE + sizeof(CExchangeHead)];
};
#endif