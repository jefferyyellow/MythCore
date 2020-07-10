
//#include <wchar.h>
//#include "logdisplayer.h"
//#include "log.h"
//#include "logmanager.h"
//#include "fileutility.h"
//#include "timemanager.h"
//#include "performance.h"
//#include "commontype.h"
//#include "i18n.h"
//#include <vector>
//#include <list>
//#include "bit_set.h"
////#include <hash_map>
//#include "blockmemory.h"
//#include "memory.h"
//#include "iterator.h"
//#include "array.h"

//#include "list.h"
//#include "slist.h"
//#include "deque.h"
//#include "vector.h"
//#include "selectmodel.h"
//#include "threadpool.h"
//#include "thread.h"
//#include "logmanager.h"
//#include "log.h"
//#include "logdisplayer.h"
//#include "simplelock.h"
//using namespace Myth;
//#include <stdio.h>
//using namespace std;
//
//#define SERVER_PERCENT_FLOAT 10000.0f
//#define SERVER_MILLIONTH	0.0001f
////using namespace stdext;
//
////#include <type_traits>
//
//void logdebugfunc()
//{
//	CLogManager* pLogManger = CLogManager::CreateInst();
//	CStdDisplayer tStdDisplayer;
//	const char* pLog="Hello world!\n";
//	tStdDisplayer.DisplayLog(const_cast<char*>(pLog));
//
//	const char* tRollFileName = "tt.log";
//	const char* tRollFileMessage = "Yes,you should get ready for your arrive";
//	char acBuffer[256] = {0};
//	CRollFileDisplayer tRollFileDisPlayer(const_cast<char*>(tRollFileName), 1024, 10);
//	for(int i = 0; i < 101; ++ i)
//	{
//		snprintf(acBuffer, sizeof(acBuffer) - 1, "%s_____%d\n", tRollFileMessage, i);
//		tRollFileDisPlayer.DisplayLog(acBuffer);
//	}
//
//	CDebuggerOutputDisplayer tDebugDisplayer;
//	tDebugDisplayer.DisplayLog(const_cast<char*>(tRollFileMessage));
//	CLog mInfoLog;
//	CLogManager::Inst()->SetInfoLog(&mInfoLog);
//	CLog mDebugLog;
//	CLogManager::Inst()->AddDebugLog(&mDebugLog, "default");
//
//	CStdDisplayer tDisPlayer;
//	mDebugLog.AddDisplayer(&tDisPlayer);
//
//	CRollFileDisplayer tRollFileDisplayer(const_cast<char*>(tRollFileName), 1024, 10);
//	mDebugLog.AddDisplayer(&tRollFileDisplayer);
//
//	CDebuggerOutputDisplayer tOutputDisplayer;
//	mDebugLog.AddDisplayer(&tOutputDisplayer);
//
//	LOG_DEBUG("default", "%s", "Hello World!!!\n");
//
//
//	CLog mProDebugLog;
//	CLogManager::Inst()->AddDebugLog(&mProDebugLog, "pro");
//
//
//	CStdDisplayer tProPlayer;
//	mProDebugLog.AddDisplayer(&tProPlayer);
//
//	const char* pProLogName = "pro.log";
//	CRollFileDisplayer tProFileDisplayer(const_cast<char*>(pProLogName), 1024, 10);
//	mProDebugLog.AddDisplayer(&tProFileDisplayer);
//
//	CDebuggerOutputDisplayer tProOutputDisplayer;
//	mProDebugLog.AddDisplayer(&tProOutputDisplayer);
//
//	LOG_DEBUG("pro", "%s", "Pro log message is here!\n");
//
//	CLogManager::DestroyInst();
//}
//
//void filedebugfunc()
//{
//#ifdef MYTH_OS_WINDOWS
//	char tFilePath[STRING_LENGTH_256] = "F:\\MythCore\\MythCore\\Debug\\pro.log";
//#else
//	char tFilePath[STRING_LENGTH_256] = "/home/huangjh/MythCore/MythCore/MythCore/pro.log";
//#endif
//	char tFileNameExt[STRING_LENGTH_256] = {0};
//	CFileUtility::GetFileName(tFilePath, tFileNameExt, sizeof(tFileNameExt));
//	printf("File name: %s\n", tFileNameExt);
//
//	char tPathName[STRING_LENGTH_256] = {0};
//	CFileUtility::GetPathName(tFilePath, tPathName, sizeof(tPathName));
//	printf("PathName: %s\n", tPathName);
//
//	char tExtName[STRING_LENGTH_256] = {0};
//	CFileUtility::GetExtension(tFilePath, tExtName, sizeof(tFilePath));
//	printf("Ext Name: %s\n", tExtName);
//
//	char tFileName[STRING_LENGTH_256] = {0};
//	CFileUtility::GetFileNameWithoutExtension(tFilePath, tFileName, sizeof(tFileName));
//	printf("File Name Without Extension: %s\n", tFileName);
//
//	bool bFileExist = CFileUtility::CheckFileExist(tFilePath);
//	if (bFileExist)
//	{
//		printf("File exist!\n");
//	}
//	else
//	{
//		printf("File not exist!!\n");
//	}
//
//	bool bDirectory = CFileUtility::IsDirectory(tFilePath);
//	if (bDirectory)
//	{
//		printf("File is directory!\n");
//	}
//	else
//	{
//		printf("File is not directory!\n");
//	}
//
//	int nFileSize = CFileUtility::GetFileSize(tFilePath);
//	printf("File size: %d\n", nFileSize);
//
//	bool bCreateFile = CFileUtility::CreateEmptyFile("passwd");
//	if (bCreateFile)
//	{
//		printf("Create file success!\n");
//	}
//	else
//	{
//		printf("Create file failure!\n");
//	}
//
//	bool bDeleteFile = CFileUtility::DeleteFile("passwd");
//	if (bDeleteFile)
//	{
//		printf("Delete file success!\n");
//	}
//	else
//	{
//		printf("Delete file failure!\n");
//	}
//
//	bool bCreateDir = CFileUtility::CreateDir("abc");
//	if (bCreateDir)
//	{
//		printf("Create directory success!\n");
//	}
//	else
//	{
//		printf("Create directory failure!\n");
//	}
//
//	bool bDeleteDir = CFileUtility::DeleteDir("abc");
//	if (bDeleteDir)
//	{
//		printf("Delete directory success!\n");
//	}
//	else
//	{
//		printf("Delete directory failure!\n");
//	}
//
////	bool bCreateDirTree = CFileUtility::CreateDirTree("hjh/hjh2/hjh3");
////	if (bCreateDirTree)
////	{
////		printf("Create directory tree success!\n");
////	}
////	else
////	{
////		printf("Create directory tree failure!\n");
////	}
//
//	bool bDeleteDirTree = CFileUtility::DeleteDirTree("hjh");
//	if (bDeleteDirTree)
//	{
//		printf("Delete directory tree success!\n");
//	}
//	else
//	{
//		printf("Delete directory tree failure!\n");
//	}
//}
//#define MI 0.0001
//#define	PE 10000
//
//void timedebugfunc3()
//{
//	PERFOR_TIMER_BEFORE(timedebugfunc3);
//	double i = 1000000;
//	double j = 0;
//	for (int i = 0; i < 100; ++ i)
//	{
//		j = i  / 10000;
//	}
//	PERFOR_TIMER_AFTER(timedebugfunc3);
//}
//
//void timedebugfunc4()
//{
//	PERFOR_TIMER_BEFORE(timedebugfunc4);
//	double i = 1000000;
//	double j = 0;
//	for (int i = 0; i < 100000; ++ i)
//	{
//		j = i * MI;
//	}
//	PERFOR_TIMER_AFTER(timedebugfunc4);
//}
//
//void timedebugfunc2()
//{
//	PERFOR_TIMER_BEFORE(timedebugfunc2);	
//
//	double i = 1000000000;
//	double j = 0;
//	for (int i = 0; i < 10000000; ++ i)
//	{
//		j = i * MI;
//	}
//	for (int i = 0; i < 10; ++ i)
//	{
//		timedebugfunc3();
//	}
//
//	for (int i = 0; i < 44; ++ i)
//	{
//		timedebugfunc4();
//	}
//	PERFOR_TIMER_AFTER(timedebugfunc2);
//}
//
//void autotimer()
//{
//	PERFOR_AUTO_TIMER(autotimer);
//	for (int i = 0; i < 44; ++ i)
//	{
//		timedebugfunc4();
//	}
//}
//
//void timedebugfunc()
//{
//	CTimeManager* pTime = CTimeManager::CreateInst();
//	CPerformance* pPerformance = CPerformance::CreateInst();
//	PERFOR_TIMER_BEFORE(timedebugfunc);
//	sint64 nDeviation = CClockTime::CalcDeviation();
//	printf("Deviation: %ld\n", nDeviation);
//
//	for (int i = 0; i < 30; ++ i)
//	{
//		timedebugfunc2();
//	}
//
//	PERFOR_TIMER_AFTER(timedebugfunc);
//	CPerformance::Inst()->PrintResult();
//
//	autotimer();
//	CTimeManager::DestroyInst();
//	CPerformance::DestroyInst();
//}
//
//void i18ndebugfunc()
//{
//	char tUtf8Buff[STRING_LENGTH_32]={0};
//	char tAsciBuff[STRING_LENGTH_32] = {0};
//	wchar_t tUniBuff[STRING_LENGTH_32] = {0};
//
//	const char* pName= "张二超";
//	CI18N::AnsiToUtf8(const_cast<char*>(pName), tUtf8Buff, sizeof(tUtf8Buff));
//	printf("Ansi: %s\n", pName);
//	printf("Utf8: %s\n", tUtf8Buff);
//
//	CI18N::Utf8ToAnsi(tUtf8Buff, tAsciBuff, sizeof(tAsciBuff));
//	printf("Ansi: %s\n", tAsciBuff);
//
//	CI18N::AnsiToUnicode(const_cast<char*>(pName), tUniBuff, STRING_LENGTH_32);
//	printf("Unicode: %s\n", tUniBuff);
//
//
//	const wchar_t* pUniName=L"张二超";
//	CI18N::UnicodeToUtf8(const_cast<wchar_t*>(pUniName), tUtf8Buff, sizeof(tUtf8Buff));
//	printf("Unicode: %s\n", pUniName);
//	printf("Utf8: %s\n", tUtf8Buff);
//	
//	CI18N::Utf8ToUnicode(tUtf8Buff, tUniBuff, STRING_LENGTH_32);
//	printf("Unicode: %s\n", tUniBuff);
//
//	CI18N::UnicodeToAnsi(const_cast<wchar_t*>(pUniName), tAsciBuff, sizeof(tAsciBuff));
//	printf("Ansi: %s\n", tAsciBuff);
//}
//
//void testBitSet()
//{
//
//	CBitSet<10> tBitSet;
//	//tBitSet.setBit(3);
//	//tBitSet.clearBit(3);
//	//tBitSet.setAllBit();
//	for (int i = 0; i < 5; ++ i)
//	{
//		tBitSet.setBit(i);
//	}
//
//	CBitSet<10> tNewSet;
//	tNewSet = tBitSet;
//	CBitSet<10> tNewSet2(tBitSet);
//	CBitSet<10> tNewSet3(true);
//
//	int tNewSet3Size = tNewSet3.size();
//
//	for (int i = 0; i < 10; ++ i)
//	{
//		if (tBitSet.getBit(i))
//		{
//			printf("%d: true\n", i);
//		}
//		else
//		{
//			printf("%d: false\n", i);
//		}
//	}
//
//	tNewSet.clearAllBit();
//	bool bAllBitSet = tNewSet.checkAllBitSet();
//	tNewSet.setAllBit();
//	bAllBitSet = tNewSet.checkAllBitSet();
//	tNewSet.clearAllBit();
//	bAllBitSet = tNewSet.checkAllBitClear();
//	tNewSet &= tNewSet2;
//	tNewSet != tNewSet2;
//	tNewSet ^= tNewSet2;
//
//	tNewSet = ~tNewSet2;
//	tNewSet = tNewSet & tNewSet2;
//	tNewSet = tNewSet | tNewSet2;
//	tNewSet = tNewSet ^ tNewSet2;
//
//}
//
//struct A
//{
//	int i; 
//};
//
//class CTestPod
//{
//public:
//	CTestPod()
//	{
//		mData = 0;
//	}
//	virtual ~CTestPod()
//	{
//		mData = 0;
//	}
//
//public:
//	void	SetData(int nData){ mData = nData; }
//
//protected:
//private:
//	int		mData;
//};
//void testtypetrait()
//{
//
//	PERFOR_TIMER_BEFORE(Devide);
//	for (int i = 1; i <= 10000; ++ i)
//	{
//		float j = i / SERVER_PERCENT_FLOAT;
//	}
//	PERFOR_TIMER_AFTER(Devide);
//
//	PERFOR_TIMER_BEFORE(Multiply);
//	for (int i = 1; i <= 10000; ++ i)
//	{
//		float j = i * SERVER_MILLIONTH;
//	}
//	PERFOR_TIMER_AFTER(Multiply);
//
//	CPerformance::Inst()->PrintResult();
//	CTimeManager::DestroyInst();
//	CPerformance::DestroyInst();
//
//	CBlockMemory<CTestPod, 5, 1> tMemory;
//	CTestPod* tpPod[7];
//	for (int i = 0; i < 7; ++ i)
//	{
//		tpPod[i] = tMemory.allocate();
//	}
//
//	for (int i = 0; i < 7; ++ i)
//	{
//		tMemory.free(tpPod[i]);
//	}
//
//	tMemory.checkMemoryLeak();
//	//bool bTemp = std::has_trivial_destructor<CTestPod>::value;
//	//bTemp = std::has_virtual_destructor<CTestPod>::value;
//
//	CTestPod temp;
//	void* p =&temp;
//	comconstruct<CTestPod, true>::construct(p);
//	comconstruct<CTestPod, false>::construct(p);
//}
//
//void testarray()
//{
//	int t = 3;
//	Myth::CArray<int, 10> array(3);
//	array.push_back(4);
//	Myth::CArray<int, 10>::iterator iter = array.begin();
//	Myth::CArray<int, 10>::iterator eiter = array.end();
//	for (; iter != eiter; ++iter)
//	{
//		printf("%d\t", *iter);
//	}
//
//	printf("\n");
//	for (unsigned int i = 0; i < array.size(); ++i)
//	{
//		printf("%d\t", array[i]);
//	}
//	printf("\n");
//	for (unsigned int i = 0; i < array.size(); ++i)
//	{
//		printf("%d\t", array.at(i));
//	}
//	printf("\n");
//	int a[3] = { 1, 2, 3 };
//	array.assign(a, a + 3);
//	for (unsigned int i = 0; i < array.size(); ++i)
//	{
//		printf("%d\t", array.at(i));
//	}
//	printf("\n");
//	Myth::CArray<int, 10>::iterator it = array.begin();
//	for (; it != array.end(); ++it)
//	{
//		printf("%d\t", *it);
//	}
//	printf("\n");
//	Myth::CArray<int, 10>::const_iterator constit = array.begin();
//	for (; constit != array.end(); ++constit)
//	{
//		printf("%d\t", *constit);
//	}
//
//	printf("\n");
//	array.assign(3, 4100);
//	for (unsigned int i = 0; i < array.size(); ++i)
//	{
//		printf("%d\t", array.at(i));
//	}
//	printf("\n");
//	array.push_back(3);
//	array.push_back(5);
//
//	for (unsigned int i = 0; i < array.size(); ++i)
//	{
//		printf("%d\t", array.at(i));
//	}
//	printf("\n");
//	array.insert(array.begin(), 3);
//	array.insert(array.begin(), 3);
//	for (unsigned int i = 0; i < array.size(); ++i)
//	{
//		printf("%d\t", array.at(i));
//	}
//	printf("\n");
//	array.insert(array.begin(), 2, 5);
//	for (unsigned int i = 0; i < array.size(); ++i)
//	{
//		printf("%d\t", array.at(i));
//	}
//
//	printf("\n");
//	array.erase(array.begin());
//	for (unsigned int i = 0; i < array.size(); ++i)
//	{
//		printf("%d\t", array.at(i));
//	}
//	printf("\n");
//
//	array.erase(array.begin(), array.end());
//	for (unsigned int i = 0; i < array.size(); ++i)
//	{
//		printf("%d\t", array.at(i));
//	}
//
//	Myth::CArray<CTestPod, 10> arrPod(3);
//	CTestPod tPod;
//	tPod.SetData(6);
//	arrPod.push_back(tPod);
//	Myth::CArray<CTestPod, 10> arrPod2 = arrPod;
//
//}
//
//void testList()
//{
//	Myth::CList<int> list(3, 3);
//	Myth::CList<int>::iterator it = list.begin();
//	Myth::CList<int>::iterator end = list.end();
//	for (; it != end; ++it)
//	{
//		printf("%d\t", *it);
//	}
//
//	printf("\n");
//	Myth::CList<int> listsecond(list);
//	it = listsecond.begin();
//	end = listsecond.end();
//	for (; it != end; ++it)
//	{
//		printf("%d\t", *it);
//	}
//
//	printf("\n");
//	Myth::CList<int> listthird;
//	listthird = list;
//	it = listthird.begin();
//	end = listthird.end();
//	for (; it != end; ++it)
//	{
//		printf("%d\t", *it);
//	}
//
//	printf("\n");
//	listthird = list;
//	it = listthird.begin();
//	end = listthird.end();
//	for (; it != end; ++it)
//	{
//		printf("%d\t", *it);
//	}
//
//	printf("\n");
//	listthird.assign(6, 6);
//	it = listthird.begin();
//	end = listthird.end();
//	for (; it != end; ++it)
//	{
//		printf("%d\t", *it);
//	}
//
//	printf("\n");
//	listthird.assign(3, 3);
//	it = listthird.begin();
//	end = listthird.end();
//	for (; it != end; ++it)
//	{
//		printf("%d\t", *it);
//	}
//
//	int a[4] = { 6, 7, 8, 9 };
//	printf("\n");
//	listthird.assign(&a[0], &a[4]);
//	it = listthird.begin();
//	end = listthird.end();
//	for (; it != end; ++it)
//	{
//		printf("%d\t", *it);
//	}
//
//	printf("\n");
//	Myth::CList<int>::reverse_iterator rit = listthird.rbegin();
//	Myth::CList<int>::reverse_iterator rEnd = listthird.rend();
//	for (; rit != rEnd; ++rit)
//	{
//		printf("%d\t", *rit);
//	}
//
//	printf("\n");
//	listthird.push_back(10);
//	listthird.push_back(11);
//	it = listthird.begin();
//	end = listthird.end();
//	for (; it != end; ++it)
//	{
//		printf("%d\t", *it);
//	}
//
//	printf("\n");
//	listthird.push_front(12);
//	listthird.push_front(13);
//	listthird.push_front(14);
//	it = listthird.begin();
//	end = listthird.end();
//	for (; it != end; ++it)
//	{
//		printf("%d\t", *it);
//	}
//
//	printf("\n");
//	listthird.pop_back();
//	listthird.pop_back();
//	it = listthird.begin();
//	end = listthird.end();
//	for (; it != end; ++it)
//	{
//		printf("%d\t", *it);
//	}
//
//	printf("\n");
//	listthird.pop_front();
//	listthird.pop_front();
//	listthird.pop_front();
//	it = listthird.begin();
//	end = listthird.end();
//	for (; it != end; ++it)
//	{
//		printf("%d\t", *it);
//	}
//
//	printf("\n");
//	listthird.insert(listthird.begin(), 11);
//	listthird.insert(listthird.begin(), 12);
//	it = listthird.begin();
//	end = listthird.end();
//	for (; it != end; ++it)
//	{
//		printf("%d\t", *it);
//	}
//
//	printf("\n");
//	listthird.insert(listthird.end(), 11);
//	listthird.insert(listthird.end(), 12);
//	it = listthird.begin();
//	end = listthird.end();
//	for (; it != end; ++it)
//	{
//		printf("%d\t", *it);
//	}
//
//	printf("\n");
//	listthird.insert(listthird.end(), 3, 13);
//	it = listthird.begin();
//	end = listthird.end();
//	for (; it != end; ++it)
//	{
//		printf("%d\t", *it);
//	}
//
//	printf("\n");
//	listthird.insert(listthird.end(), &a[0], &a[4]);
//	it = listthird.begin();
//	end = listthird.end();
//	for (; it != end; ++it)
//	{
//		printf("%d\t", *it);
//	}
//
//	printf("\n");
//	listthird.erase(listthird.begin());
//	listthird.erase(listthird.begin());
//	it = listthird.begin();
//	end = listthird.end();
//	for (; it != end; ++it)
//	{
//		printf("%d\t", *it);
//	}
//
//	printf("\n");
//	listthird.remove(7);
//	it = listthird.begin();
//	end = listthird.end();
//	for (; it != end; ++it)
//	{
//		printf("%d\t", *it);
//	}
//
//	printf("\n");
//	listthird.push_front(6);
//	listthird.push_back(9);
//	it = listthird.begin();
//	end = listthird.end();
//	for (; it != end; ++it)
//	{
//		printf("%d\t", *it);
//	}
//
//	printf("\n");
//	listthird.unique();
//	it = listthird.begin();
//	end = listthird.end();
//	for (; it != end; ++it)
//	{
//		printf("%d\t", *it);
//	}
//
//	printf("\n");
//	listthird.reverse();
//	it = listthird.begin();
//	end = listthird.end();
//	for (; it != end; ++it)
//	{
//		printf("%d\t", *it);
//	}
//}
//
//void testSlist()
//{
//	Myth::CSlist<int, 3, 3> slist(3, 3);
//	Myth::CSlist<int, 3, 3>::iterator it = slist.begin();
//	Myth::CSlist<int, 3, 3>::iterator itEnd = slist.end();
//	for (; it != itEnd; ++it)
//	{
//		printf("%d\t", *it);
//	}
//	printf("\n");
//
//	int n[5] = { 0, 2, 4, 6, 8 };
//	Myth::CSlist<int, 3, 3> slist2(&n[0], &n[5]);
//	it = slist2.begin();
//	itEnd = slist2.end();
//	for (; it != itEnd; ++it)
//	{
//		printf("%d\t", *it);
//	}
//	printf("\n");
//
//	Myth::CSlist<int, 3, 3> slist3(slist2);
//	it = slist3.begin();
//	itEnd = slist3.end();
//	for (; it != itEnd; ++it)
//	{
//		printf("%d\t", *it);
//	}
//	printf("\n");
//
//	Myth::CSlist<int, 3, 3> slist4 = slist2;
//	it = slist4.begin();
//	itEnd = slist4.end();
//	for (; it != itEnd; ++it)
//	{
//		printf("%d\t", *it);
//	}
//	printf("\n");
//
//	printf("Size %d\t", slist4.size());
//	printf("\n");
//
//
//	Myth::CSlist<int, 3, 3>::const_iterator constit = slist4.begin();
//	Myth::CSlist<int, 3, 3>::const_iterator constEnd = slist4.end();
//	for (; constit != constEnd; ++constit)
//	{
//		printf("%d\t", *constit);
//	}
//	printf("\n");
//
//	Myth::CSlist<int, 3, 3> slist5;
//	if (slist5.empty())
//	{
//		printf("slist5 empty");
//	}
//	else
//	{
//		printf("slist5 noempty");
//	}
//	printf("\n");
//	if (slist4.empty())
//	{
//		printf("slist4 empty");
//	}
//	else
//	{
//		printf("slist4 noempty");
//	}
//	printf("\n");
//
//	while (!slist4.empty())
//	{
//		printf("%d\t", *slist4.begin());
//		slist4.pop_front();
//	}
//
//	printf("\n");
//
//	Myth::CSlist<int, 3, 3> slist6;
//	slist6.push_front(1);
//	slist6.push_front(2);
//	slist6.push_front(3);
//	slist6.push_front(3);
//	slist6.push_front(4);
//	slist6.push_front(5);
//	slist6.push_front(5);
//	slist6.push_front(6);
//	slist6.push_front(6);
//	it = slist6.begin();
//	itEnd = slist6.end();
//	for (; it != itEnd; ++it)
//	{
//		printf("%d\t", *it);
//	}
//	printf("\n");
//
//	slist6.reverse();
//	it = slist6.begin();
//	itEnd = slist6.end();
//	for (; it != itEnd; ++it)
//	{
//		printf("%d\t", *it);
//	}
//	printf("\n");
//
//
//	slist6.remove(3);
//	it = slist6.begin();
//	itEnd = slist6.end();
//	for (; it != itEnd; ++it)
//	{
//		printf("%d\t", *it);
//	}
//	printf("\n");
//
//	slist6.unique();
//	it = slist6.begin();
//	itEnd = slist6.end();
//	for (; it != itEnd; ++it)
//	{
//		printf("%d\t", *it);
//	}
//
//	printf("\n");
//
//	it = slist6.begin();
//	itEnd = slist6.end();
//	for (; it != itEnd;)
//	{
//		if (*it == 5)
//		{
//			it = slist6.erase(it);
//		}
//		else
//		{
//			++it;
//		}
//	}
//
//	it = slist6.begin();
//	itEnd = slist6.end();
//	for (; it != itEnd; ++it)
//	{
//		printf("%d\t", *it);
//	}
//	printf("\n");
//
//	slist6.erase(slist6.begin(), slist6.end());
//	it = slist6.begin();
//	itEnd = slist6.end();
//	for (; it != itEnd; ++it)
//	{
//		printf("%d\t", *it);
//	}
//	printf("\n");
//
//	if (slist6.empty())
//	{
//		printf("slist6 empty");
//	}
//	else
//	{
//		printf("slist6 noempty");
//	}
//	printf("\n");
//}

//class CSizeA
//{
//public:
//	virtual void funcA()
//	{
//		printf("funcA");
//	}
//	char c;
//	int   val;
//	short sh;
//};
//
//class CSizeB : public CSizeA
//{
//public:
//	void func1(void);
//	virtual void funcB(void)
//	{
//		printf("funcB");
//	}
//
//	int i;
//};
//
//class CSizeC : public CSizeB
//{
//public:
//	int t;
//};
//
//using namespace std;
//#include <string>
//#include <map>
//
//class CPerfInfo
//{
//public:
//	CPerfInfo()
//		: mPerfLog(1), mTotalCalled(0), mCostTime(0), mMostCostTime(0), mLestCostTime(1000)
//	{}
//	~CPerfInfo()
//	{}
//
//public:
//
//	int	mPerfLog;		// 是否统计本信息。为了以后控制统计项制定.默认统计
//	int	mTotalCalled;	// 被调用的总次数
//	int	mCostTime;		// 总共花费的时间
//	int mMostCostTime;	// 最长消耗时间
//	int mLestCostTime;	// 最短消耗时间
//};
//
//
//
///// 性能优化，不直接使用string作为map的key,封装CPerfIndex作为key
//class CPerfIndex
//{
//public:
//	CPerfIndex( unsigned int uiHashCode, const char* pszName ) : 
//	  mHashCode( uiHashCode ), mName( pszName ) {}
//	~CPerfIndex(){}
//
//	unsigned int	mHashCode;		// 作为优先比较关键字
//	std::string		mName;			// perf统计的信息
//};
//
//
//
//class CLessPrefIndex
//{
//public:
//
//	bool operator()(const CPerfIndex& __x, const CPerfIndex& __y) const
//	{
//		return ( __x.mHashCode < __y.mHashCode )
//			|| ( __x.mHashCode == __y.mHashCode && __x.mName < __y.mName );
//	}
//};
//
//typedef std::map<CPerfIndex, CPerfInfo, CLessPrefIndex> PerfMap;
//typedef PerfMap::iterator PerfMapIterator;
//class CPerfStat
//{
//public:
//	static PerfMap msPerfMap;
//	/**
//	* DJB Hash Function
//	* An algorithm produced by Professor Daniel J. Bernstein and shown first to the
//	* world on the usenet newsgroup comp.lang.c. It is one of the most efficient
//	* hash functions ever published.
//	*
//	* @param str    需要计算hash值的字符串，必须以ASCII 0结尾
//	*
//	* @return 字符串对应的hash值
//	*/
//	static inline unsigned int DJBHash(const char* str)
//	{
//		unsigned int hash = 5381;
//
//		for(const unsigned char* p = reinterpret_cast<const unsigned char*> (str); 0 != *p; ++p)
//		{
//			hash = ((hash << 5) + hash) + *p;
//		}
//		return (hash & 0x7FFFFFFF);
//	}
//
//
//	static inline CPerfInfo& GetPerfInfo(const char* name)
//	{
//		unsigned int hashCode = DJBHash(name);
//		CPerfIndex perfIndex(hashCode, name);
//		return msPerfMap[perfIndex];
//	}
//
//	static inline CPerfInfo& GetPerfInfo(unsigned int hashCode, const char* name)
//	{
//		CPerfIndex perfIndex(hashCode, name);
//		return msPerfMap[perfIndex];
//	}
//
//	// 记录统计信息到日志文件
//	static void LogPerfInfo(PerfMap& rPerfMap, const char* pKey);
//
//};
//
//class CTestInit
//{
//public:
//	CTestInit()
//		:a(0), b(0), c(0), d(0), e(0), f(0), g(0)
//	{
//		//init();
//	}
//
//	void init()
//	{
//		a = 0;
//		b = 0;
//		c = 0;
//		d = 0;
//		e = 0;
//		f = 0;
//		g = 0;
//	}
//
//private:
//	int a;
//	int b;
//	int c;
//	int d;
//	int e;
//	int f;
//	int g;
//};
//void testHashMap()
//{
//	//typedef hash_map<int, int>  HASH_MAP;
//	//HASH_MAP tMap;
//	//for (int i = 0; i <= 2000; ++i)
//	//{
//	//	tMap[i] = i;
//	//}
//	//HASH_MAP::iterator it;
//
//	CTimeManager* pTime = CTimeManager::createInst();
//	CPerformance* pPerformance = CPerformance::createInst();
//	PERFOR_TIMER_BEFORE(testHashMap);
//
//	for (int i = 0; i < 1000000; ++ i)
//	{
//		CTestInit a;
//	}
//
//	PERFOR_TIMER_AFTER(testHashMap);
//	CPerformance::Inst()->PrintResult();
//
//	CTimeManager::destroyInst();
//	CPerformance::destroyInst();
//}
//
//void testVector()
//{
//	Myth::CVector<int, 4, 4> vector(10u, 10);
//	Myth::CVector<int, 4, 4>::iterator it = vector.begin();
//	Myth::CVector<int, 4, 4>::iterator itend = vector.end();
//	for (; it != itend; ++it)
//	{
//		printf("%d		", *it);
//	}
//
//	printf("\n");
//	for (int i = 0; i < vector.size(); ++i)
//	{
//		printf("%d		", vector[i]);
//	}
//	for (int i = 0; i < 20; i++)
//	{
//		vector.assign(20, 20);
//	}
//	printf("\n");
//	for (int i = 0; i < vector.size(); ++i)
//	{
//		printf("%d		", vector[i]);
//	}
//
//	Myth::CVector<int, 4, 4> vector2(2u, 2);
//	for (int i = 0; i < 8; ++i)
//	{
//		vector2.push_back(i);
//	}
//
//	printf("\n");
//	for (int i = 0; i < vector2.size(); ++i)
//	{
//		printf("%d		", vector2[i]);
//	}
//
//}
//
//void testSelectModel()
//{
//	Myth::CSelectModel selectModel;
//	selectModel.setMaxFd(0);
//}
//
//#ifdef MYTH_OS_WINDOWS
//#include <Windows.h>
//// CriticalSection锁
//class CCSLock
//{
//public:
//	CCSLock(void){ ::InitializeCriticalSection(&m_CriticalSection); }
//	~CCSLock(void){ ::DeleteCriticalSection(&m_CriticalSection); }
//
//
//	void Lock(){ ::EnterCriticalSection(&m_CriticalSection); }
//	void UnLock(){ ::LeaveCriticalSection(&m_CriticalSection); }
//
//private:
//	CRITICAL_SECTION m_CriticalSection;
//};
//void testCriticalSection()
//{
//
//	CTimeManager* pTime = CTimeManager::CreateInst();
//	CPerformance* pPerformance = CPerformance::CreateInst();
//	PERFOR_TIMER_BEFORE(timedebugfunc);
//
//	CCSLock cs;
//	for (int i = 0; i < 160000000; ++i)
//	{
//		cs.Lock();
//		cs.UnLock();
//	}
//
//	PERFOR_TIMER_AFTER(timedebugfunc);
//	CPerformance::Inst()->PrintResult();
//
//	CTimeManager::DestroyInst();
//	CPerformance::DestroyInst();
//	
//
//}
//#endif
//PerfMap CPerfStat::msPerfMap;
//
//Myth::CSimpleLock cs;
//class CJob : public Myth::IJob
//{
//public:
//	virtual void doing(int uParam)
//	{
//		cs.lock();
//		//printf("Thread Serial Num :%d, %d\n", uParam, mNum);
//		//LOG_DEBUG("pro", "Thread Serial Num :%d, %d\n", uParam, getJobID());
//		cs.unlock();
//	}
//};
#include "mysqldatabase.h"
#include "mysqlqueryresult.h"
#include "tinyxml2.h"
#include "logmanager.h"
#include "logdisplayer.h"
#include <vector>
#include <string>
using namespace std;

using namespace Myth;
using namespace tinyxml2;
#	define LOG_INFO(fmt, ... )				CLogManager::Inst()->LogInfoMessageFormat(fmt, ##__VA_ARGS__ )
#	define LOG_WARN(fmt, ... )				CLogManager::Inst()->LogWarnMessageFormat(fmt, ##__VA_ARGS__ )
#	define LOG_ERROR(fmt, ... )				CLogManager::Inst()->LogErrorMessageFormat(fmt, ##__VA_ARGS__ )
#	define LOG_DEBUG(logname, fmt, ... )	CLogManager::Inst()->LogDebugMessageFormat(logname, fmt, ##__VA_ARGS__ )

typedef std::vector<std::string> Tokens;
Tokens StrSplit(const std::string &src, const std::string &sep)
{
	Tokens r;
	std::string s;
	for (std::string::const_iterator i = src.begin(); i != src.end(); i++)
	{
		if (sep.find(*i) != std::string::npos)
		{
			if (s.length()) r.push_back(s);
			s = "";
		}
		else
		{
			s += *i;
		}
	}
	if (s.length()) r.push_back(s);
	return r;
}


class CServerInfo
{
public:
	string	strServerID;
	string	strServerName;
	string	strIP;
	string	strDataBase;
};

typedef std::vector<CServerInfo> SERVER_LIST;
SERVER_LIST gServerList;
void LoadConfig()
{
	FILE* fp = fopen("realInfo.txt", "r");
	char acBuff[10000] = {0};
	while (!feof(fp))
	{
		fgets(acBuff, 10000, fp);
		Tokens tToken = StrSplit(acBuff, "\t" );

		CServerInfo tInfo;
		tInfo.strServerID = tToken[0];
		tInfo.strServerName = tToken[1];
		tInfo.strIP = tToken[2];
		tInfo.strDataBase = tToken[3];
		gServerList.push_back(tInfo);
	}
	fclose(fp);
}


void PrintfMysqlResult(CMysqlQueryResult& rResult)
{
	char acBuffer[100000];
	int nResultLen = 100000;
	int nLength = 0;

	for (int i = 0; i < rResult.getRowCount(); ++i)
	{

		nResultLen = 0;
		nLength = 0;
		for (int j = 0; j < rResult.getFieldCount(); ++j)
		{
			if (j == 0 || j == 1)
			{
				nLength = snprintf((char*)&(acBuffer[nResultLen]), sizeof(acBuffer) - nResultLen - 1, "%-30s", rResult.getFileValue(j));
				nResultLen += nLength;
				//printf("%-30s", tResult.getFileValue(j));
			}
			else
			{
				nLength = snprintf((char*)&(acBuffer[nResultLen]), sizeof(acBuffer) - nResultLen - 1, "%-10s", rResult.getFileValue(j));
				nResultLen += nLength;
			}
		}

		LOG_INFO(acBuffer);
		rResult.nextRow();
	}

}

int main(int argc, char** argv)
{
//	typedef		int				value_type;
//	typedef const value_type*	const_pointer;
//	typedef const value_type&	const_reference;
//	typedef	const value_type*	const_iterator;	
//
//	//CTimeManager* pTime = CTimeManager::CreateInst();
//	//CPerformance* pPerformance = CPerformance::CreateInst();
//
//	//srand(time(NULL));
//
//	//PERFOR_TIMER_BEFORE(vector);
//	//std::vector<int> tvec;
//	//const int nCountMax = 10000;
//	//for (int i = 0; i < nCountMax; ++ i)
//	//{
//	//	int nRand= rand();
//	//	tvec.push_back(nRand);
//	//}
//
//	//for (int i = 0; i < nCountMax; ++ i)
//	//{
//	//	int nRand= rand();
//	//	for (int j = 0; j < tvec.size(); ++ j)
//	//	{
//	//		if (nRand == tvec[j])
//	//		{
//	//			break;
//	//		}
//	//	}
//	//}
//	//PERFOR_TIMER_AFTER(vector);
//
//	//PERFOR_TIMER_BEFORE(hash_map);
//	//stdext::hash_map<int, int> tMap;
//	//for (int i = 0; i < nCountMax; ++ i)
//	//{
//	//	int nRand= rand();
//	//	tMap[nRand] = i;
//	//}
//
//	//for (int i = 0; i < nCountMax; ++ i)
//	//{
//	//	int nRand= rand();
//	//	int j = tMap[nRand];
//	//}
//	//PERFOR_TIMER_AFTER(hash_map);
//	//CPerformance::Inst()->PrintResult();
//	//CTimeManager::DestroyInst();
//	//CPerformance::DestroyInst();
//	//int nMax = rand()%7;
//
//	//bool bPerNodePod = std::is_pod<CPerforNode>::value;
//	//bool bTestPod = std::is_pod<CTestPod>::value;
//	//logdebugfunc();
//	//filedebugfunc();
//	//timedebugfunc();
//	//i18ndebugfunc();
//	//std::vector<int> dd;
//	//dd.push_back(1);
//	//dd.push_back(2);
//	//dd.push_back(3);
//	//for (auto &i:dd)
//	//{
//	//	Print("%d\n", i);
//	//}
//
//	//testBitSet();
//	//testtypetrait();
//
//	testarray();
//	testList();
//	testSlist();
//	std::vector<int> arr;
//	arr.assign(3,3);
//
//	std::list<int> list(3,0);
//	int size = sizeof(CSizeA);
//	int size2 = sizeof(CSizeB);
//
//	testHashMap();
//	testVector();
//	testCriticalSection();

	//CLogManager* pLogManger = CLogManager::CreateInst();
	//CLog mProDebugLog;
	//CLogManager::Inst()->AddDebugLog(&mProDebugLog, "pro");
	//
	//
	//CStdDisplayer tProPlayer;
	//mProDebugLog.AddDisplayer(&tProPlayer);
	//
	//const char* pProLogName = "pro.log";
	//CRollFileDisplayer tProFileDisplayer(const_cast<char*>(pProLogName), 1024000, 10);
	//mProDebugLog.AddDisplayer(&tProFileDisplayer);
	
//	LOG_DEBUG("pro", "%s", "Pro log message is here!\n");


//	Myth::CThreadPool threadpool(4);
//	CJob tJob1;
//	tJob1.setJobID(1);
//	threadpool.pushBackJob(&tJob1);
//
//	CJob tJob2;
//	tJob2.setJobID(2);
//	threadpool.pushBackJob(&tJob2);
//
//	CJob tJob3;
//	tJob3.setJobID(3);
//	threadpool.pushBackJob(&tJob3);
//
//	CJob tJob4;
//	tJob4.setJobID(4);
//	threadpool.pushBackJob(&tJob4);
//
//	CJob tJob5;
//	tJob5.setJobID(5);
//	threadpool.pushBackJob(&tJob5);
//
//	CJob tJob6;
//	tJob6.setJobID(6);
//	threadpool.pushBackJob(&tJob6);
//
//	int i = 0;
//	while (true)
//	{
//		//printf("begin next\n");
//		cs.lock();
////		LOG_DEBUG("pro", "begin next\n");
//		cs.unlock();
//		threadpool.run();
//#ifdef MYTH_OS_WINDOWS
//		Sleep(100);
//#else
//		struct timespec tv;
//		tv.tv_sec = 5;
//		tv.tv_nsec = 0;
//
//		nanosleep(&tv, NULL);
//#endif
//		++i;
//		if (i > 50)
//		{
//			break;
//		}
//	}
//
//	CLogManager::DestroyInst();
	//CMysqlDataBase	mDataBase;
	//int nResult = mDataBase.connectMysqlServer("192.168.10.13', "root", "webgame", "mxwHDB", 3306, NULL);
	LoadConfig();
	CLogManager::createInst();

	// 给信息日志加文件displayer
	CRollFileDisplayer* pInfoFileDisplayer = new CRollFileDisplayer(const_cast<char*>("testinfo.log"), 1024000, 10);
	CLogManager::Inst()->GetInfoLog().AddDisplayer(pInfoFileDisplayer);


	for (int i = 0; i < gServerList.size(); ++ i)
	{
		CServerInfo& rInfo = gServerList[i];

		CMysqlDataBase mDataBase;

		int nResult = mDataBase.connectMysqlServer(rInfo.strIP.c_str(), "mxw2-server", "FMxxfh7brodkhx", rInfo.strDataBase.c_str(), 3306, NULL);
		if (0 != nResult)
		{
			return 0;
		}


		CMysqlQueryResult tResult(&mDataBase, false);

		nResult = mDataBase.query("desc UMS_ROLE;", tResult);
		PrintfMysqlResult(tResult);
		LOG_INFO("\n\n\n");

		nResult = mDataBase.query("desc UMS_ROLE_INFO;", tResult);
		PrintfMysqlResult(tResult);
		LOG_INFO("\n\n\n");
	}



}

