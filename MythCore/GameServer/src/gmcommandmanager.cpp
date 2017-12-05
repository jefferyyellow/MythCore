#include "gmcommandmanager.h"
#include "propertymodule.h"
#include "itemmodule.h"
#include "locallogjob.h"
#include "i18n.h"
#include "entityplayer.h"

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

void CGMCommandManager::Init()
{
	COMMAND_HANDLER_ADD(help, "��������÷���\\help �����������֡�\ab,���û������Ϊab��GM����ͻ��г�������ab��ͷ������");
	COMMAND_HANDLER_ADD(exp, "��þ��飬�÷���\\exp ��þ�������");
	//COMMAND_HANDLER_ADD(money, "��ý�ң��÷���\\money ����������������Ϊ��ֵ����Ϊʧȥָ�������Ľ�ҡ�");
	//COMMAND_HANDLER_ADD(diamond, "�����ʯ���÷���\\diamond ��ʯ�������õ�ָ����������ʯ��������VIP���飬���Ϊ��ֵ����Ϊʧȥָ����������ʯ��");
}

COMMAND_HANDLER_IMPL(help)
{
	MYTH_ASSERT_INFO(tTokens.size() < 1, return, 
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
	MYTH_ASSERT_INFO(tTokens.size() < 1, return,
		"exp command parameter number invalid, %d", tTokens.size());

	int nExp = atoi(tTokens[0].c_str());
	pPlayer->GetPropertyUnit().obtainExp(nExp);
}

//COMMAND_HANDLER_IMPL(money)
//{
//	MYTH_ASSERT_INFO(tTokens.size() < 1, return,
//		"money command parameter number invalid, %d", tTokens.size());
//
//	int nMoney = atoi(tTokens[0].c_str());
//	if (nMoney > 0)
//	{
//		pPlayer->GetItemUnit().obtainMoney(nMoney);
//	}
//	else
//	{
//		pPlayer->GetItemUnit().consumeMoney(-nMoney);
//	}
//}
//
//COMMAND_HANDLER_IMPL(diamond)
//{
//	MYTH_ASSERT_INFO(tTokens.size() < 1, return,
//		"diamond command parameter number invalid, %d", tTokens.size());
//
//	int nDiamond = atoi(tTokens[0].c_str());
//	if (nDiamond > 0)
//	{
//		pPlayer->GetItemUnit().obtainDiamond(nDiamond);
//	}
//	else
//	{
//		pPlayer->GetItemUnit().consumeDiamond(-nDiamond);
//	}
//}