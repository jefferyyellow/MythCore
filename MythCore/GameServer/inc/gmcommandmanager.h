#ifndef __GMCOMMANDMANAGER_H__
#define __GMCOMMANDMANAGER_H__

#include <map>
#include "strutility.h"

class CEntityPlayer;
#define COMMAND_HANDLER_DECL(Commandname)	static void Commandname##Handler(std::string strCommandName, StrTokens& tTokens, CEntityPlayer* pPlayer)
#define COMMAND_HANDLER_IMPL(Commandname)	void CGMCommandManager::Commandname##Handler(std::string strCommandName, StrTokens& tTokens, CEntityPlayer* pPlayer)
#define COMMAND_HANDLER_ADD(Commandname, HelpString) \
	mCommandHash.insert( CommandHash::value_type(#Commandname, Commandname##Handler)); \
	mCommandHelpHash.insert(CommandHelpHash::value_type(#Commandname, HelpString))

class CGMCommandManager
{
public:
	typedef void(*COMMANDHANDLER)(std::string strCommandName, StrTokens& tTokens, CEntityPlayer* pPlayer);
	typedef std::map<std::string, COMMANDHANDLER > CommandHash;
	typedef std::map<std::string, std::string> CommandHelpHash;

public:
	CGMCommandManager()
	{
		InitCommand();
	}

	~CGMCommandManager()
	{
	}
	void InitCommand();

public:
	/// �㲥����Ӱ��ͽ��
	static void	broadcastCommandResult(CEntityPlayer* pPlayer, char* pResult);
	/// ֪ͨ�������Ӱ��ͽ��
	static void	sendCommandResult(CEntityPlayer* pPlayer, char* pResult);
	/// ִ��GM����
	void		excuteCommand(std::string strCommandName, StrTokens& tTokens, CEntityPlayer* pPlayer);

private:
	/// ��������
	COMMAND_HANDLER_DECL(help);
	/// ��þ�������
	COMMAND_HANDLER_DECL(exp);
	/// �����������
	COMMAND_HANDLER_DECL(ii);
	/// ɾ����������
	COMMAND_HANDLER_DECL(removeitem);
	/// ��ձ�������
	COMMAND_HANDLER_DECL(clearbag);
	/// ������ҵȼ�����
	COMMAND_HANDLER_DECL(setlevel);
	/// ����ʱ��
	COMMAND_HANDLER_DECL(settime);
	/// ��ֵ
	COMMAND_HANDLER_DECL(recharge);
	/// �����ʼ�
	COMMAND_HANDLER_DECL(sendmail);
	/// ����ȫ���ʼ�
	COMMAND_HANDLER_DECL(globalmail);

private:
	/// �õ�����İ����ַ���
	std::string getCommnadHelpString(std::string strCommandName);
	/// �õ���strName��ͷ����������
	std::string getAllCommnadWith(std::string strName);

private:
	CommandHash		mCommandHash;			// GM����
	CommandHelpHash	mCommandHelpHash;		// GM�������
};
#endif