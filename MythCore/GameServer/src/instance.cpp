#include "instance.h"
/// ------------------- CSingleInstance -----------------------
/// ��ʼ��
void CSingleInstance::init()
{

}

/// ����
void CSingleInstance::create()
{

}

/// ����
void CSingleInstance::end()
{

}

/// ����
void CSingleInstance::destroy()
{

}

/// ����
void CSingleInstance::givePrize()
{

}


/// ��ҽ���
void CSingleInstance::playerEnter(CEntityPlayer& rPlayer)
{

}


/// ------------------- CMultipleInstance -----------------------
/// ��ʼ��
void CMultipleInstance::init()
{

}

/// ����
void CMultipleInstance::create()
{

}

/// ����
void CMultipleInstance::end()
{

}

/// ����
void CMultipleInstance::destroy()
{

}

/// ����
void CMultipleInstance::givePrize()
{

}

/// ��ҽ���
void CMultipleInstance::playerEnter(CEntityPlayer& rPlayer)
{

}


/// ------------------- CInstance -----------------------
/// ����
void CInstance::create()
{
	switch (mType)
	{
		case emInstance_Common:
		{
			mSingle.init();
			mSingle.create();
			break;
		}
		case emInstance_Team:
		{
			mMultiple.init();
			mMultiple.create();
			break;
		}
	}
}

/// ����
void CInstance::end()
{
	switch (mType)
	{
		case emInstance_Common:
		{
			mSingle.end();
			break;
		}
		case emInstance_Team:
		{
			mMultiple.end();
			break;
		}
	}
}

/// ����
void CInstance::destroy()
{
	switch (mType)
	{
		case emInstance_Common:
		{
			mSingle.destroy();
			break;
		}
		case emInstance_Team:
		{
			mMultiple.destroy();
			break;
		}
	}
}

/// ����
void CInstance::givePrize()
{
	switch (mType)
	{
		case emInstance_Common:
		{
			mSingle.givePrize();
			break;
		}
		case emInstance_Team:
		{
			mMultiple.givePrize();
			break;
		}
	}
}

/// ��ҽ���
void CInstance::playerEnter(CEntityPlayer& rPlayer)
{
	switch (mType)
	{
		case emInstance_Common:
		{
			mSingle.playerEnter(rPlayer);
			break;
		}
		case emInstance_Team:
		{
			mMultiple.playerEnter(rPlayer);
			break;
		}
	}
}
