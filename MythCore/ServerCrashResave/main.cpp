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

	// �ͷ�protobuf��lib��msgռ�õ��ڴ�
	::google::protobuf::ShutdownProtobufLibrary();
}