#ifndef __INTERACTIVEUNIT_H__
#define __INTERACTIVEUNIT_H__
#include "playersubunit.h"
#include "mail.h"
#include "indexarray.h"
#include "messagefactory.h"
using namespace Myth;
class CEntityPlayer;
/// ������Ԫ,�����ʼ������죬�������
class CInteractiveUnit : public CPlayerSubUnit
{
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
	// �õ�һ���ʼ�
	void		addMail(CMail& tMail);
	// ɾ��һ���ʼ�
	void		deleteMail(int nIndex);
	// �õ��ʼ�
	CMail*		getMail(int nMailID);
public:
	/// ��ȡ�ʼ��б������
	void		onGetMailListRequest(Message* pMessage);
	/// �鿴�ʼ�������
	void		onReadMailRequest(Message* pMessage);
	/// ���Ͳ鿴�ʼ��Ļ�Ӧ
	void		sendReadMailResponse(int nResult, EmMailStatus eStatus);
	/// ��ȡ�ʼ�����������
	void		onGetMailAttachmentRequest(Message* pMessage);
	/// ɾ���ʼ�������
	void		onDeleteMailRequest(Message* pMessage);

private:
	CIndexArray<CMail, MAX_PLAYER_MAIL>	mMailList;
};
#endif