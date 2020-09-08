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
	// 加载错误码
	void LoadErrCode(const char* pErrCodePath);
	/// 加载lua文件里面的错误码
	void LoadLuaErrCode(const char* pErrCodePath);
	/// 广播命令影响和结果
	static void	broadcastCommandResult(CEntityPlayer& rPlayer, char* pResult);
	/// 通知玩家命令影响和结果
	static void	sendCommandResult(CEntityPlayer& rPlayer, const char* pResult);
	/// 执行GM命令
	void		excuteCommand(std::string strCommandName, StrTokens& tTokens, CEntityPlayer& rPlayer);

private:
	/// 帮助命令
	COMMAND_HANDLER_DECL(help);
	/// 错误码
	COMMAND_HANDLER_DECL(error);
	/// 获得经验命令
	COMMAND_HANDLER_DECL(exp);
	/// 插入道具命令
	COMMAND_HANDLER_DECL(ii);
	/// 删除道具命令
	COMMAND_HANDLER_DECL(removeitem);
	/// 清空背包命令
	COMMAND_HANDLER_DECL(clearbag);
	/// 设置玩家等级命令
	COMMAND_HANDLER_DECL(setlevel);
	/// 设置时间
	COMMAND_HANDLER_DECL(settime);
	/// 充值
	COMMAND_HANDLER_DECL(recharge);
	/// 发送邮件
	COMMAND_HANDLER_DECL(sendmail);
	/// 发送全局邮件
	COMMAND_HANDLER_DECL(globalmail);
	/// 重新加载配置文件
	COMMAND_HANDLER_DECL(reloadconfig);


private:
	/// 得到命令的帮助字符串
	std::string getCommnadHelpString(std::string strCommandName);
	/// 得到以strName开头的所以命令
	std::string getAllCommnadWith(std::string strName);
	ErrorCodeHash& getErrorCodeHash(){return mErrorCodeHash;}
private:
	CommandHash		mCommandHash;			// GM命令
	CommandHelpHash	mCommandHelpHash;		// GM命令帮助
	ErrorCodeHash	mErrorCodeHash;
};
#endif