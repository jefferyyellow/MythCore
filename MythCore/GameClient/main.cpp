#include "gameclient.h"
//
//void sendFormatData(Myth::CTcpSocket& tSocket, char* pData)
//{
//	short nLen = 6 + strlen(pData);
//	char acSendBuffer[1024] = { 0 };
//	char* pTemp = acSendBuffer;
//	memcpy(pTemp, &nLen, sizeof(short));
//
//	pTemp += sizeof(short);
//	short nMessageID = 1304;
//	memcpy(pTemp, &nMessageID, sizeof(short));
//
//	pTemp += sizeof(short);
//	short nContextLen = strlen(pData);
//	memcpy(pTemp, &nContextLen, sizeof(short));
//	pTemp += sizeof(short);
//
//	strcpy(pTemp, pData);
//	tSocket.sendData(acSendBuffer, nLen);
//}
//
//int main(int argc, char** argv)
//{
//	Myth::CTcpSocket tTcpSocket;
//	Myth::CSelectModel::initSocketSystem();
//	tTcpSocket.createSocket();
//	int nResult = tTcpSocket.connectServer("127.0.0.1", 6688);
//	int nError = WSAGetLastError();
//	char acData[128] = "Hello World,My MythCore!";
//	//tTcpSocket.sendData(acData, strlen(acData));
//	sendFormatData(tTcpSocket, acData);
//
//	Sleep(100000);
//	tTcpSocket.closeSocket();
//}

int main(int argc, char** argv)
{
	CGameClient tGameClient;
	tGameClient.init();
	tGameClient.run();
}