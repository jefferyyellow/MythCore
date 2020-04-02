#include "commontype.h"
#include "sharememory.h"
#include "objpool.h"
#include "ServerResave.h"

int main()
{
	CServerResave::createInst();
	bool bResult = CServerResave::Inst()->initAll();
	if (!bResult)
	{
		CServerResave::Inst()->exit();
		CServerResave::destroyInst();
		return 0;
	}

	CServerResave::Inst()->run();
	CServerResave::Inst()->exit();
	CServerResave::destroyInst();

	// 释放protobuf中lib和msg占用的内存
	::google::protobuf::ShutdownProtobufLibrary();
}