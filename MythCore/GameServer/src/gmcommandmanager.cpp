#include "gmcommandmanager.h"
#include "propertymodule.h"
#include "itemmodule.h"
#include "locallogjob.h"
#include "i18n.h"
#include "entityplayer.h"

/// 执行GM命令
void CGMCommandManager::excuteCommand(std::string strCommandName, StrTokens& tTokens, CEntityPlayer* pPlayer)
{
	CommandHash::iterator it = mCommandHash.find(strCommandName);
	if (it != mCommandHash.end())
	{
		it->second(strCommandName, tTokens, pPlayer);
	}
	else
	{
		std::string str = getAllCommnadWith(strCommandName);
		//SendDebugMsg2ChatDlg(pPlayer, str);
	}
}

/// 得到命令的帮助字符串
std::string CGMCommandManager::getCommnadHelpString(std::string strCommandName)
{
	CommandHelpHash::iterator it = mCommandHelpHash.find(strCommandName);
	if (it == mCommandHelpHash.end())
	{
		return std::string("");
	}

	return it->second;
}

/// 得到以strName开头的所以命令
std::string CGMCommandManager::getAllCommnadWith(std::string strName)
{
	std::string strResult;
	CommandHelpHash::iterator it = mCommandHelpHash.begin();
	for (; it != mCommandHelpHash.end(); ++it)
	{
		if (it->first.find(strName) == 0)
		{
			strResult += it->first;
			strResult += "    ";
		}
	}

	return strResult;
}

void CGMCommandManager::InitCommand()
{
	COMMAND_HANDLER_ADD(help, "帮助命令，用法：\\help 完整命令名字。\ab,如果没有名字为ab的GM命令，就会列出所有已ab开头的命令");
	COMMAND_HANDLER_ADD(exp, "获得经验，用法：\\exp 获得经验数。");
	COMMAND_HANDLER_ADD(ii, "获得道具，用法：\\ii 道具ID 道具数目，获得对应的道具，包括货币。");
	COMMAND_HANDLER_ADD(setlevel, "获得道具，用法：\\setlevel 等级，设置玩家等级。");
}

COMMAND_HANDLER_IMPL(help)
{
	MYTH_ASSERT_INFO(tTokens.size() >= 1, return, 
		"help command parameter number invalid, %d", tTokens.size());

	CGMCommandManager& rCmdManager = CPropertyModule::Inst()->getGMCmdManager();
	std::string strHelpString = rCmdManager.getCommnadHelpString(tTokens[0]);
	if (strHelpString == "")
	{
		return;
	}

	char cCharName[8092] = { 0 };
	CI18N::AnsiToUtf8(const_cast<char*>(strHelpString.c_str()), cCharName, sizeof(cCharName) - 1);
	std::string str(cCharName);
	//SendDebugMsg2ChatDlg(pPlayer, str);
}

COMMAND_HANDLER_IMPL(exp)
{
	MYTH_ASSERT_INFO(tTokens.size() >= 1, return,
		"exp command parameter number invalid, %d", tTokens.size());

	int nExp = atoi(tTokens[0].c_str());
	pPlayer->getPropertyUnit().obtainExp(nExp);
}

COMMAND_HANDLER_IMPL(ii)
{
	MYTH_ASSERT_INFO(tTokens.size() >= 2, return,
		"ii command parameter number invalid, %d", tTokens.size());

	int nItemID = atoi(tTokens[0].c_str());
	int nItemNum = atoi(tTokens[1].c_str());
	pPlayer->getItemUnit().insertItem(&nItemID, &nItemNum, 1);
}

COMMAND_HANDLER_IMPL(setlevel)
{
	MYTH_ASSERT_INFO(tTokens.size() >= 1, return,
		"setlevel command parameter number invalid, %d", tTokens.size());

	int nOldPlayer = pPlayer->getLevel();
	int nNewLevel = atoi(tTokens[0].c_str());

	pPlayer->setLevel(nNewLevel);
	pPlayer->getPropertyUnit().onLevelUp(nOldPlayer);
}