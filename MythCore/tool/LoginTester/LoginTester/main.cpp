#include "logintester.h"
int main()
{
	CLoginTester::CreateInst();
	if (!CLoginTester::Inst()->init())
	{
		return -1;
	}
	CLoginTester::Inst()->setServerIP("127.0.0.1");
	CLoginTester::Inst()->setServerPort(6688);
	CLoginTester::Inst()->run();
}