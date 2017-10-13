#ifndef __LOGINMODULE_H__
#define __LOGINMODULE_H__
#include "singleton.h"
#include "messagefactory.h"
#include "commontype.h"
#include "logintype.h"
#include "logicmodule.h"
#include "timemanager.h"
#include <map>

using namespace Myth;
class CInternalMsg;
#define  MAKE_LOGIN_KEY(AccountID, ChannelID, ServerID) ( (ChannelID << 48) | (ServerID << 32) | AccountID)
class CLoginModule : public CLogicModule, public CSingleton < CLoginModule >
{
	friend class CSingleton < CLoginModule > ;
public:
	typedef std::map<uint16, uint32> LOGIN_LIST;
private:
	CLoginModule();
	~CLoginModule();

public:
	/// Ê±¼äº¯Êý
	virtual		void OnTimer(unsigned int nTickOffset);

public:
	void		onClientMessage(CExchangeHead& rExchangeHead, unsigned int nMessageID, Message* pMessage);
	void		onIMPlayerLoginResponse(CInternalMsg* pMsg);
	void		onIMCreateRoleResponse(CInternalMsg* pMsg);
	void		onIMEnterSceneResponse(CInternalMsg* pMsg);

private:
	void		onMessageLoginRequest(CExchangeHead& rExchangeHead, Message* pMessage);
	void		onMessageCreateRoleRequest(CExchangeHead& rExchangeHead, Message* pMessage);
	void		onMessageEnterSceneRequest(CExchangeHead& rExchangeHead, Message* pMessage);

private:
	LOGIN_LIST			mLoginList;
	CAutoResetTimer		mLoginCheckTime;
};
#endif