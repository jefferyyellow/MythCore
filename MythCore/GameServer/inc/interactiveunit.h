#ifndef __INTERACTIVEUNIT_H__
#define __INTERACTIVEUNIT_H__
#include "playersubunit.h"
#include "mail.h"
#include "messagefactory.h"
#include "mythlist.h"
using namespace Myth;
class CEntityPlayer;
/// ������Ԫ,�����ʼ������죬�������
class CInteractiveUnit : public CPlayerSubUnit
{
public:
	typedef CMythList<CMail, MAX_PLAYER_MAIL>	MAIL_LIST;

public:
	CInteractiveUnit(CEntityPlayer& rPlayer)
		:CPlayerSubUnit(rPlayer)
	{
		init();
	}
	~CInteractiveUnit()
	{

	}
	void init()
	{
		
	}

public:
	//// ���һ���ʼ�
	void		addMail(CMail& tMail);
	// �õ��ʼ�ID��Ӧ�ĵ�����
	MAIL_LIST::iterator	getMail(uint nMailID);
	/// �����ݿ�ɾ����ӦID���ʼ�
	void		deleteMailFromDB(uint nMailID);
	/// �����ʼ���״̬�����ݿ�
	void		updateMailStatus2DB(uint nMailID, EmMailStatus eMailStatus);

public:
	/// ��ȡ�ʼ��б������
	void		onGetMailListRequest(Message* pMessage);
	/// �鿴�ʼ�������
	void		onReadMailRequest(Message* pMessage);
	/// ���Ͳ鿴�ʼ��Ļ�Ӧ
	void		sendReadMailResponse(int nResult, EmMailStatus eStatus);
	/// ��ȡ�ʼ�����������
	void		onGetMailAttachmentRequest(Message* pMessage);
	/// ���ͻ�ȡ�ʼ������Ļ�Ӧ
	void		sendGetMailAttachmentResponse(int nResult, int nStatus);
	/// ɾ���ʼ�������
	void		onDeleteMailRequest(Message* pMessage);
	/// ����ɾ���ʼ��Ļ�Ӧ
	void		sendDeleteMailResponse(int nResult);
	/// �õ��ʼ�����ϸ��Ϣ����
	void		onGetMailDetailRequest(Message* pMessage);
	/// �����ʼ���ϸ��Ϣ�Ļ�Ӧ
	void		sendGetMailDetailResponse(int nResult, CMail& rMail);
public:
	/// ��ȡ�������ȫ���ʼ�ID
	uint		getGlobalMailTime() const { return mGlobalMailTime; }
	void		setGlobalMailTime(uint nValue) { mGlobalMailTime = nValue; }

	/// �ʼ��б�
	MAIL_LIST&	getMailList(){return mMailList;}

private:
	/// �ʼ��б�
	MAIL_LIST	mMailList;
	/// �չ������µ�ȫ���ʼ�ʱ�䣬ע�ⲻ����ȫ���ʼ�ID����Ϊ�漰���Ϸ�
	uint		mGlobalMailTime;
};
#endif