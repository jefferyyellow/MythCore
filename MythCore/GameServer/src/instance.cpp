#include "instance.h"
/// ------------------- CSingleInstance -----------------------
/// 初始化
void CSingleInstance::init()
{

}

/// 创建
void CSingleInstance::create()
{

}

/// 结束
void CSingleInstance::end()
{

}

/// 销毁
void CSingleInstance::destroy()
{

}

/// 发奖
void CSingleInstance::givePrize()
{

}


/// 玩家进入
void CSingleInstance::playerEnter(CEntityPlayer& rPlayer)
{

}


/// ------------------- CMultipleInstance -----------------------
/// 初始化
void CMultipleInstance::init()
{

}

/// 创建
void CMultipleInstance::create()
{

}

/// 结束
void CMultipleInstance::end()
{

}

/// 销毁
void CMultipleInstance::destroy()
{

}

/// 发奖
void CMultipleInstance::givePrize()
{

}

/// 玩家进入
void CMultipleInstance::playerEnter(CEntityPlayer& rPlayer)
{

}


/// ------------------- CInstance -----------------------
/// 创建
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

/// 结束
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

/// 销毁
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

/// 发奖
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

/// 玩家进入
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
