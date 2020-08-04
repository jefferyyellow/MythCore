#include "gmcommandmanager.h"
#include "propertymodule.h"
#include "itemmodule.h"
#include "locallogjob.h"
#include "i18n.h"
#include "entityplayer.h"
#include "scenejob.h"
#include "itemmodule.hxx.pb.h"
#include "gameserver.h"
#include "chatmodule.hxx.pb.h"
#include "chatmodule.h"
/// 广播命令影响和结果
void CGMCommandManager::broadcastCommandResult(CEntityPlayer* pPlayer, char* pResult)
{
	CChatNotify tChatNotify;
	tChatNotify.set_playerid(pPlayer->getRoleID());
	tChatNotify.set_playername(pPlayer->getName());
	tChatNotify.set_channel(emChatChannel_World);
	tChatNotify.set_content(pResult);
	CSceneJob::Inst()->send2AllPlayer(ID_S2C_NOTIFY_CHAT, &tChatNotify);
}

/// 通知玩家命令影响和结果
void CGMCommandManager::sendCommandResult(CEntityPlayer* pPlayer, char* pResult)
{
	CChatNotify tChatNotify;
	tChatNotify.set_playerid(pPlayer->getRoleID());
	tChatNotify.set_playername(pPlayer->getName());
	tChatNotify.set_channel(emChatChannel_World);
	tChatNotify.set_content(pResult);

	CSceneJob::Inst()->send2Player(pPlayer, ID_S2C_NOTIFY_CHAT, &tChatNotify);
}

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
	COMMAND_HANDLER_ADD(removeitem, "删除道具，用法：\\removeitem 道具ID 道具数目，删除对应的道具，包括货币,道具数目为-1表示删除所有道具(货币)。");
	COMMAND_HANDLER_ADD(clearbag, "清空背包，用法：\\clearbag，清空背包，注意：不包括货币，只清除道具。");
	COMMAND_HANDLER_ADD(setlevel, "获得道具，用法：\\setlevel 等级，设置玩家等级。");
	COMMAND_HANDLER_ADD(settime, "设置时间，用法：\\settime [20200803] [11:44:55]，设置当前的服务器时间。");
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

COMMAND_HANDLER_IMPL(removeitem)
{
	MYTH_ASSERT_INFO(tTokens.size() >= 2, return,
		"removeitem command parameter number invalid, %d", tTokens.size());
	int nItemID = atoi(tTokens[0].c_str());
	int nItemNum = atoi(tTokens[1].c_str());
	if (nItemNum < 0)
	{
		int nAllItemNum = pPlayer->getItemUnit().hasItem(nItemID);
		pPlayer->getItemUnit().removeItemByID(nItemID, nAllItemNum);
	}
	else
	{
		pPlayer->getItemUnit().removeItem(nItemID, nItemNum);
	}
}

COMMAND_HANDLER_IMPL(clearbag)
{
	CItemBox& rBag = pPlayer->getItemUnit().getBag();
	for (int i = 0; rBag.getSize(); ++ i)
	{
		CItemObject* pItemObject = rBag.getItem(i);
		if (NULL == pItemObject)
		{
			continue;
		}

		CRemoveItemNotify tRemoveItemNotify;
		tRemoveItemNotify.set_index(i);
		tRemoveItemNotify.set_number(pItemObject->GetItemNum());
		CSceneJob::Inst()->send2Player(pPlayer, ID_S2C_NOTIYF_REMOVE_ITEM, &tRemoveItemNotify);
	}

	rBag.clear();
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

// 设置时间
// 第一种情况：没有参数，将当前的游戏时间设置为系统真实时间
// 第二种情况：1个参数(如果字符串中有：表示时间，如果没有表示日期)，
//					1. 20201223 修改当前日期
//					2. 20:48:34 修改当前的时间
// 第三种情况：2个参数,第一个参数表示日期，第二个参数表示时间
// 例子：
// settime 2：表示把游戏时间的其他都不变，改成本月2号
// settime 0803：表示游戏时间的其他都不变,改成当年的8月3号
// settime 20230808：表示游戏时间的其他都不变,改成2023年8月8号
// settime 14:43：表示游戏时间的其他都不变，改成14点43分
// settime 14:43:58 表示游戏时间的其他都不变，改成14点43分58秒
// settime 20230808 14:43:58 改成2023年8月8号14点43分58秒
COMMAND_HANDLER_IMPL(settime)
{
	tm& rTmNow = CSceneJob::Inst()->getTmNow();
	tm  tTmSetTime = rTmNow;

	StrTokens tTimeTokens;
	if (tTokens.size() >= 2)
	{
		int nDate = atoi(tTokens[0].c_str());
		int nTmpYear = nDate / 10000;
		int nTmpMonth = nDate % 10000 / 100;
		int nTmpDay = nDate % 100;
		if (nTmpYear != 0)
		{
			tTmSetTime.tm_year = nTmpYear - 1900;
		}

		if (nTmpMonth != 0)
		{
			tTmSetTime.tm_mon = nTmpMonth - 1;
		}

		tTmSetTime.tm_mday = nTmpDay;

		tTimeTokens = strSplit(tTokens[1].c_str(), ":");
	}
	else if (tTokens.size() >= 1)
	{
		tTimeTokens = strSplit(tTokens[0].c_str(), ":");
		// 如果不是以冒号分隔的字符串
		if (tTimeTokens.size() <= 1)
		{
			int nDate = atoi(tTokens[0].c_str());
			int nTmpYear = nDate / 10000;
			int nTmpMonth = nDate % 10000 / 100;
			int nTmpDay = nDate % 100;
			if (nTmpYear != 0)
			{
				tTmSetTime.tm_year = nTmpYear - 1900;
			}

			if (nTmpMonth != 0)
			{
				tTmSetTime.tm_mon = nTmpMonth - 1;
			}

			tTmSetTime.tm_mday = nTmpDay;
			// 清掉时间
			tTimeTokens.clear();
		}
	}
	else
	{
		time_t tRealTime = time(NULL);
#ifdef MYTH_OS_WINDOWS
		localtime_s(&tTmSetTime, &tRealTime);
#else
		localtime_r(&tRealTime, &tTmSetTime);
#endif // MYTH_OS_WINDOWS
	}

	if (tTimeTokens.size() >= 1)
	{
		tTmSetTime.tm_hour = atoi(tTimeTokens[0].c_str());
	}
	if (tTimeTokens.size() >= 2)
	{
		tTmSetTime.tm_min = atoi(tTimeTokens[1].c_str());
	}
	if (tTimeTokens.size() >= 3)
	{
		tTmSetTime.tm_sec = atoi(tTimeTokens[2].c_str());
	}

	time_t tGameTime = mktime(&tTmSetTime);
	CGameServer::Inst()->SetGameTimeOffset(tGameTime - time(NULL));

	char acBuffer[STR_LENGTH_128] = {0};
#ifdef MYTH_OS_WINDOWS
	asctime_s(acBuffer, sizeof(acBuffer) - 1, &tTmSetTime);
#else
	asctime_r(&tTmSetTime, acBuffer);
#endif
	broadcastCommandResult(pPlayer, acBuffer);
}