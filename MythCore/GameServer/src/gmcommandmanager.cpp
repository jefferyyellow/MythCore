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
/// �㲥����Ӱ��ͽ��
void CGMCommandManager::broadcastCommandResult(CEntityPlayer* pPlayer, char* pResult)
{
	CChatNotify tChatNotify;
	tChatNotify.set_playerid(pPlayer->getRoleID());
	tChatNotify.set_playername(pPlayer->getName());
	tChatNotify.set_channel(emChatChannel_World);
	tChatNotify.set_content(pResult);
	CSceneJob::Inst()->send2AllPlayer(ID_S2C_NOTIFY_CHAT, &tChatNotify);
}

/// ֪ͨ�������Ӱ��ͽ��
void CGMCommandManager::sendCommandResult(CEntityPlayer* pPlayer, char* pResult)
{
	CChatNotify tChatNotify;
	tChatNotify.set_playerid(pPlayer->getRoleID());
	tChatNotify.set_playername(pPlayer->getName());
	tChatNotify.set_channel(emChatChannel_World);
	tChatNotify.set_content(pResult);

	CSceneJob::Inst()->send2Player(pPlayer, ID_S2C_NOTIFY_CHAT, &tChatNotify);
}

/// ִ��GM����
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

/// �õ�����İ����ַ���
std::string CGMCommandManager::getCommnadHelpString(std::string strCommandName)
{
	CommandHelpHash::iterator it = mCommandHelpHash.find(strCommandName);
	if (it == mCommandHelpHash.end())
	{
		return std::string("");
	}

	return it->second;
}

/// �õ���strName��ͷ����������
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
	COMMAND_HANDLER_ADD(help, "��������÷���\\help �����������֡�\ab,���û������Ϊab��GM����ͻ��г�������ab��ͷ������");
	COMMAND_HANDLER_ADD(exp, "��þ��飬�÷���\\exp ��þ�������");
	COMMAND_HANDLER_ADD(ii, "��õ��ߣ��÷���\\ii ����ID ������Ŀ����ö�Ӧ�ĵ��ߣ��������ҡ�");
	COMMAND_HANDLER_ADD(removeitem, "ɾ�����ߣ��÷���\\removeitem ����ID ������Ŀ��ɾ����Ӧ�ĵ��ߣ���������,������ĿΪ-1��ʾɾ�����е���(����)��");
	COMMAND_HANDLER_ADD(clearbag, "��ձ������÷���\\clearbag����ձ�����ע�⣺���������ң�ֻ������ߡ�");
	COMMAND_HANDLER_ADD(setlevel, "��õ��ߣ��÷���\\setlevel �ȼ���������ҵȼ���");
	COMMAND_HANDLER_ADD(settime, "����ʱ�䣬�÷���\\settime [20200803] [11:44:55]�����õ�ǰ�ķ�����ʱ�䡣");
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

// ����ʱ��
// ��һ�������û�в���������ǰ����Ϸʱ������Ϊϵͳ��ʵʱ��
// �ڶ��������1������(����ַ������У���ʾʱ�䣬���û�б�ʾ����)��
//					1. 20201223 �޸ĵ�ǰ����
//					2. 20:48:34 �޸ĵ�ǰ��ʱ��
// �����������2������,��һ��������ʾ���ڣ��ڶ���������ʾʱ��
// ���ӣ�
// settime 2����ʾ����Ϸʱ������������䣬�ĳɱ���2��
// settime 0803����ʾ��Ϸʱ�������������,�ĳɵ����8��3��
// settime 20230808����ʾ��Ϸʱ�������������,�ĳ�2023��8��8��
// settime 14:43����ʾ��Ϸʱ������������䣬�ĳ�14��43��
// settime 14:43:58 ��ʾ��Ϸʱ������������䣬�ĳ�14��43��58��
// settime 20230808 14:43:58 �ĳ�2023��8��8��14��43��58��
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
		// ���������ð�ŷָ����ַ���
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
			// ���ʱ��
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