#include "PreCompile.h"
#include "ThreadHandlerJoinMessage.h"
#include <GameServerBase/GameServerDebug.h>
#include <GameServerBase/GameServerString.h>
#include "GameServerUser.h"
#include <GameServerNet\DBConnecter.h>
#include "UserTable.h"
#include <GameServerCore\DBQueue.h>
#include <functional>

ThreadHandlerJoinMessage::ThreadHandlerJoinMessage()
{

}

ThreadHandlerJoinMessage::~ThreadHandlerJoinMessage()
{

}

void ThreadHandlerJoinMessage::Start()
{
	if (nullptr == TCPSession_)
	{
		GameServerDebug::LogError("Join TCPSession Error");
	}

	JoinResult_.Code = EGameServerCode::JoinError;
	DBWork(&ThreadHandlerJoinMessage::DBCheck);
}

void ThreadHandlerJoinMessage::DBCheck()
{
	std::string Name = GameServerThread::GetName();
	UserTable_InsertUserInfo InsertQuery(Message_->ID, Message_->PW);
	if (false == InsertQuery.DoQuery())
	{
		JoinResult_.Code = EGameServerCode::JoinError;
	}
	else
	{
		JoinResult_.Code = EGameServerCode::OK;
	}
	NetWork(&ThreadHandlerJoinMessage::ResultSend);
}

void ThreadHandlerJoinMessage::ResultSend()
{
	GameServerSerializer Sr;
	JoinResult_.Serialize(Sr);
	TCPSession_->Send(Sr.GetData());

	GameServerDebug::LogInfo("Join Result Out");
}