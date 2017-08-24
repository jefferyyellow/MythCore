#ifndef __GMCOMMANDMANAGER_H__
#define __GMCOMMANDMANAGER_H__
#include <vector>
#include <string>
#include <map>
typedef std::vector<std::string> GMTokens;
class CEntityPlayer;
#define COMMAND_HANDLER_DECL(Commandname)	static void Commandname##Handler(std::string strCommandName, GMTokens& tTokens, CEntityPlayer* pPlayer)
#define COMMAND_HANDLER_IMPL(Commandname)	void CGMCommandManager::Commandname##Handler(std::string strCommandName, GMTokens& tTokens, CEntityPlayer* pPlayer)
#define COMMAND_HANDLER_ADD(Commandname, HelpString) \
	mCommandHash.insert( CommandHash::value_type(#Commandname, Commandname##Handler)); \
	mCommandHelpHash.insert(CommandHelpHash::value_type(#Commandname, HelpString))

class CGMCommandManager
{
public:
	typedef void(*COMMANDHANDLER)(std::string strCommandName, GMTokens& tTokens, CEntityPlayer* pPlayer);
	typedef std::map<std::string, COMMANDHANDLER > CommandHash;
	typedef std::map<std::string, std::string> CommandHelpHash;

public:
	CGMCommandManager()
	{
	}

	~CGMCommandManager()
	{
	}
	void Init();

public:
	/// ִ��GM����
	void		excuteCommand(std::string strCommandName, GMTokens& tTokens, CEntityPlayer* pPlayer);


private:
	COMMAND_HANDLER_DECL(help);
	COMMAND_HANDLER_DECL(exp);
	COMMAND_HANDLER_DECL(money);
	COMMAND_HANDLER_DECL(diamond);


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