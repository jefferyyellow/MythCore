#ifndef __MESSAGEFACTORY_H__
#define __MESSAGEFACTORY_H__

#include "singleton.h"
#include "google/protobuf/message.h"
using namespace Myth;

#define MESSAGE_MODULE_MASK				0xFC00
typedef ::google::protobuf::Message Message;
#define MAX_MSG_BUFF_LEN  60 * 1024
class CMessageFactory : public CSingleton<CMessageFactory>
{
	friend class CSingleton < CMessageFactory > ;
private:
	CMessageFactory(){}
	~CMessageFactory(){}

public:
	Message*		createClientMessage(unsigned short nMessageID);
	Message*		createServerMessage(unsigned short nMessageID);
	void			destroyMessage(Message* pMessage);

private:
	static char sMsgBuff[ MAX_MSG_BUFF_LEN ];
};
#endif // !__MESSAGEFACTORY_H__
