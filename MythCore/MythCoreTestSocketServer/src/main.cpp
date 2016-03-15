#include "selectmodel.h"
int main(int argc, char** argv)
{
	Myth::CTcpSocket mSocket[128];
	Myth::CSelectModel tSeletModel(&mSocket[0], 128);
	Myth::CSelectModel::initSocketSystem();
	tSeletModel.createListenSocket(NULL, 6688, 5);
	while (1)
	{
		tSeletModel.selectAllFd();
		tSeletModel.processRead();
		Sleep(100);
	}
}