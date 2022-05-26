#include "PreCompile.h"
#include "ThreadHandlerAttackMessage.h"
#include <GameServerBase/GameServerDebug.h>
#include <GameServerBase/GameServerString.h>
#include <GameServerNet/TCPListener.h>


void ThreadHandlerAttackMessage::Start()
{
	if (nullptr == Session_)
	{
		GameServerDebug::LogError("Login TCPSession Error");
	}

	FVector4 pos = Message_->Pos;
	int radius = Message_->Radius;

	GameServerSerializer Data;
	Message_->Serialize(Data);
	ActorWork(Message_->ThreadIndex, Message_->SectionIndex, Message_->ObjectIndex, Message_);
}