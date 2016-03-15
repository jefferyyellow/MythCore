#include "tcpsocket.h"
#include "selectmodel.h"
int main(int argc, char** argv)
{
	Myth::CTcpSocket tTcpSocket;
	Myth::CSelectModel::initSocketSystem();
	tTcpSocket.createSocket();
	int nResult = tTcpSocket.connectServer("127.0.0.1", 6688);
	int nError = WSAGetLastError();
	char acData[128] = "Hello World,My MythCore!";
	tTcpSocket.sendData(acData, strlen(acData));
	Sleep(10000);
	tTcpSocket.closeSocket();
}