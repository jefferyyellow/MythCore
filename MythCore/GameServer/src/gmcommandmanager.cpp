#include "gmcommandmanager.h"
#include "propertymodule.h"
#include "itemmodule.h"
#include "locallogjob.h"
#include "i18n.h"
#include "entityplayer.h"
#include "scenejob.h"
#include "itemmodule.hxx.pb.h"

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