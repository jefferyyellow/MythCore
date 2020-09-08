#ifndef __GMCOMMANDMANAGER_H__
#define __GMCOMMANDMANAGER_H__

#include <map>
#include "strutility.h"

class CEntityPlayer;
#define COMMAND_HANDLER_DECL(Commandname)	static void Commandname##Handler(std::string strCommandName, StrTokens& tTokens, CEntityPlayer& rPlayer)
#define COMMAND_HANDLER_IMPL(Commandname)	void CGMCommandManager::Commandname##Handler(std::string strCommandName, StrTokens& tTokens, CEntityPlayer& rPlayer)
#define COMMAND_HANDLER_ADD(Commandname, HelpString) \
	mCommandHash.insert( CommandHash::value_type(#Commandname, Commandname##Handler)); \
	mCommandHelpHash.insert(CommandHelpHash::value_type(#Commandname, HelpString))

class CGMCommandManager
{
public:
	typedef void(*COMMANDHANDLER)(std::string strCommandName, StrTokens& tTokens, CEntityPlayer& rPlayer);
	typedef std::map<std::string, COMMANDHANDLER > CommandHash;
	typedef std::map<std::string, std::string> CommandHelpHash;
	typedef std::map<int, std::string> ErrorCodeHash;

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
	// ���ش�����
	void LoadErrCode(const char* pErrCodePath);
	/// ����lua�ļ�����Ĵ�����
	void LoadLuaErrCode(const char* pErrCodePath);
	/// �㲥����Ӱ��ͽ��
	static void	broadcastCommandResult(CEntityPlayer& rPlayer, char* pResult);
	/// ֪ͨ�������Ӱ��ͽ��
	static void	sendCommandResult(CEntityPlayer& rPlayer, const char* pResult);
	/// ִ��GM����
	void		excuteCommand(std::string strCommandName, StrTokens& tTokens, CEntityPlayer& rPlayer);

private:
	/// ��������
	COMMAND_HANDLER_DECL(help);
	/// ������
	COMMAND_HANDLER_DECL(error);
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
	/// ���¼��������ļ�
	COMMAND_HANDLER_DECL(reloadconfig);


private:
	/// �õ�����İ����ַ���
	std::string getCommnadHelpString(std::string strCommandName);
	/// �õ���strName��ͷ����������
	std::string getAllCommnadWith(std::string strName);
	ErrorCodeHash& getErrorCodeHash(){return mErrorCodeHash;}
private:
	CommandHash		mCommandHash;			// GM����
	CommandHelpHash	mCommandHelpHash;		// GM�������
	ErrorCodeHash	mErrorCodeHash;
};
#endif