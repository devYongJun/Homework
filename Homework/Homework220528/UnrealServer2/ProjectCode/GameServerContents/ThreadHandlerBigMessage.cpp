#include "PreCompile.h"
#include "ThreadHandlerBigMessage.h"
#include <GameServerBase/GameServerDebug.h>
#include <GameServerBase/GameServerString.h>
#include <GameServerNet/TCPListener.h>
//#include "DBQueue.h"
//#include "NetQueue.h"

ThreadHandlerBigMessage::ThreadHandlerBigMessage()
{

}

ThreadHandlerBigMessage::~ThreadHandlerBigMessage()
{

}

void ThreadHandlerBigMessage::Start()
{
	if (nullptr == Session_)
	{
		GameServerDebug::LogError("Login TCPSession Error");
	}

	Message_;
}
