#ifndef __LOGINMODULE_H__
#define __LOGINMODULE_H__
#include "singleton.h"
#include "messagefactory.h"
#include "commontype.h"
#include <map>
using namespace Myth;
class CInternalMsg;
#define  MAKE_LOGIN_KEY(AccountID, ChannelID, WorldID) ( (ChannelID << 48) | (WorldID << 32) | AccountID)
class CLoginModule : public CSingleton < CLoginModule >
{
	friend class CSingleton < CLoginModule > ;
public:
	typedef std::map<uint64, uint32> LOGIN_LIST;
private:
	CLoginModule();
	~CLoginModule();

public:
	void		onClientMessage(uint32 nSocketIndex, unsigned int nMessageID, Message* pMessage);
	void		onIMPlayerLoginResponse(CInternalMsg* pMsg);
	void		onIMCreateRoleResponse(CInternalMsg* pMsg);
	void		onIMEnterSceneResponse(CInternalMsg* pMsg);

private:
	void		onMessageLoginRequest(uint32 nSocketIndex, Message* pMessage);
	void		onMessageCreateRoleRequest(uint32 nSocketIndex, Message* pMessage);
	void		onMessageEnterSceneRequest(uint32 nSocketIndex, Message* pMessage);

private:
	LOGIN_LIST mLoginList;
};
#endif