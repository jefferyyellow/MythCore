#ifndef __LOGINMODULE_H__
#define __LOGINMODULE_H__
#include "singleton.h"
#include "messagefactory.h"
class CLoginModule : public CSingleton < CLoginModule >
{
	friend class CSingleton < CLoginModule > ;
private:
	CLoginModule();
	~CLoginModule();

public:
	void		onClientMessage(unsigned int nMessageID, Message* pMessage);


private:
	void		OnMessageLoginRequest(Message* pMessage);
};
#endif