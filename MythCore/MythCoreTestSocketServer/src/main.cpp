#include "selectmodel.h"
#include <stdio.h>
int main(int argc, char** argv)
{
	printf("I am server\n");
	Myth::CTcpSocket mSocket[128];
	Myth::CSelectModel tSeletModel(mSocket, 128);
	Myth::CSelectModel::initSocketSystem();
	tSeletModel.createListenSocket(NULL, 6688, 5);
	//while (1)
	//{
	//	tSeletModel.selectAllFd();
	//	tSeletModel.processRead();

	//	for (int i = 0; i < 128; ++i)
	//	{
	//		if (mSocket[i].getSocketFd() >= 0 && !mSocket[i].GetListen())
	//		{
	//			char* pBuffer = mSocket[i].getRecvBuff();
	//			if (mSocket[i].getRecvBuffSize() > 0)
	//			{
	//				char acBuff[256] = { 0 };
	//				strncpy(acBuff, mSocket[i].getRecvBuff() , mSocket[i].getRecvBuffSize());
	//				printf("%s\n", acBuff);
	//				int nResult = mSocket[i].getRecvBuffSize();
	//				mSocket[i].setRecvBuffSize(0);
	//				mSocket[i].resetRecvBuffPoint(nResult);
	//			}
	//		}
	//	}
	//	Sleep(100);
	//}
}