#ifndef __GAMESERVERNET_H__
#define __GAMESERVERNET_H__
#include "selectmodel.h"
#include "servercommon.h"
using namespace Myth;
class CGameServerNet
{
public:
	CGameServerNet()
	{

	}
	~CGameServerNet()
	{

	}
	bool	initSocket(int nMaxSocketNum, int nCacheSize, int* pListenPort, int nPortNum, int nBackLogNum);
	void	receiveMessage();

public:
	// socket
	CTcpSocket*		mpTcpSocket;
	CSelectModel	mSelectModel;

};
#endif