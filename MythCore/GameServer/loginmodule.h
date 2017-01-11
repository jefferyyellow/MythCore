#ifndef __LOGINMODULE_H__
#define __LOGINMODULE_H__
#include "singleton.h"
#include "messagefactory.h"
#include "commontype.h"
class CLoginModule : public CSingleton < CLoginModule >
{
	friend class CSingleton < CLoginModule > ;
private:
	CLoginModule();
	~CLoginModule();

public:
	void		onClientMessage(uint32 nSocketIndex, unsigned int nMessageID, Message* pMessage);


private:
	void		OnMessageLoginRequest(uint32 nSocketIndex, Message* pMessage);
	void		OnMessageCreateRoleRequest(uint32 nSocketIndex, Message* pMessage);
	void		OnMessageEnterSceneRequest(uint32 nSocketIndex, Message* pMessage);
};
#endif