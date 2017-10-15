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
	/// ʱ�亯��
	virtual		void OnTimer(unsigned int nTickOffset);

public:
	/// ����ͻ�����Ϣ
	void		onClientMessage(CExchangeHead& rExchangeHead, unsigned int nMessageID, Message* pMessage);
	/// �������ݿ���Ϣ
	void		OnDBMessage(CInternalMsg* pMsg);

private:
	LOGIN_LIST			mLoginList;				// ��¼�б�
	CAutoResetTimer		mLoginCheckTime;		// ��¼У��ʱ��
};
#endif