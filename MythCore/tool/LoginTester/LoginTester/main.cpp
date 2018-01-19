#include "logintester.h"
int main()
{
	CLoginTester tLoginTester;
	if (!tLoginTester.init())
	{
		return -1;
	}
	tLoginTester.setServerIP("127.0.0.1");
	tLoginTester.setServerPort(6688);
	tLoginTester.run();;

}