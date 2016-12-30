#include "messagefactory.h"
char CMessageFactory::sMsgBuff[MAX_MSG_BUFF_LEN] = { 0 };
#include "loginmessage.pb.h"

#define CASE_NEW_MSG(msg, msgid)						\
		case msgid:										\
		{												\
			pMessage = new(sMsgBuff) msg;		\
			break;										\
		}

Message* CMessageFactory::createClientMessage(unsigned short nMessageID)
{
	Message* pMessage = NULL;
	switch (nMessageID)
	{
		CASE_NEW_MSG(CMessageLoginRequest, ID_C2S_REQUEST_LOGIN)
		default:
			break;
	}

	return pMessage;
}

Message* CMessageFactory::createServerMessage(unsigned short nMessageID)
{
	switch (nMessageID)
	{

		default:
			break;
	}

	return NULL;
}

void CMessageFactory::destroyMessage(Message* pMessage)
{

}
