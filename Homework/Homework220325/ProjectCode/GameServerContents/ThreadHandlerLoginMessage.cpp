#include "PreCompile.h"
#include "ThreadHandlerLoginMessage.h"
#include <GameServerBase/GameServerDebug.h>
#include <GameServerBase/GameServerString.h>
#include "GameServerUser.h"
#include <GameServerNet\DBConnecter.h>
#include "UserTable.h"
#include <GameServerCore\DBQueue.h>
#include <functional>

// DB에 접속하는일
// 검증하는일
// 마지막으로 결과 패킷을 보내는 일까지
// 전부다 이안에서 벌어져야 하고
// 절대로 외부로 새어나가서는 안됩니다.

// 모든 이렇게 하겠지라고 착각하면 큰일납니다.
// 외우는 것이 아니라 코드를 짠사람의 의도를 파악해라.
// 보통 경력자한테 원하는 겁니다.
ThreadHandlerLoginMessage::ThreadHandlerLoginMessage()
{

}

ThreadHandlerLoginMessage::~ThreadHandlerLoginMessage()
{

}

void ThreadHandlerLoginMessage::Start()
{
	if (nullptr == TCPSession_)
	{
		GameServerDebug::LogError("Login TCPSession Error");
	}

	LoginResult_.Code = EGameServerCode::LoginError;
	DBWork(&ThreadHandlerLoginMessage::DBCheck);
}

void ThreadHandlerLoginMessage::DBCheck(/*GameEngineThread* _Thread*/)
{
	//std::string Name = GameServerThread::GetName();

	//UserTable_SelectIDToUserInfo SelectQuery(LoginMessage_->ID);
	//SelectQuery.DoQuery();

	//if (nullptr == SelectQuery.RowData)
	//{
	//	LoginResult_.Code = EGameServerCode::LoginError;
	//}
	//else 
	//{
	//	LoginResult_.Code = EGameServerCode::OK;
	//}

	//UserTable_InsertUserInfo Query = UserTable_InsertUserInfo("kk", "kk");
	//if (false == Query.DoQuery())
	//{
	//	int a = 0;
	//}


	//// INSERT INTO `userver2`.`user` (`ID`, `PW`) VALUES('c', 'c');

	//NetQueue::Queue(std::bind(&ThreadHandlerLoginMessage::ResultSend, shared_from_this()));

}

void ThreadHandlerLoginMessage::ResultSend()
{
	/*std::shared_ptr<GameServerUser> NewUser = std::make_shared<GameServerUser>();
	GameServerString::UTF8ToAnsi(LoginMessage_->ID, NewUser->ID);*/

	//TCPSession_->SetLink(NewUser);

	//GameServerSerializer Sr;
	//LoginResult_.Serialize(Sr);
	//TCPSession_->Send(Sr.GetData());

	//GameServerDebug::LogInfo("Login Result Out");

	// std::sheard_ptr<Section> Finds = GetALLSection();
	// Finds->PushSection(TCPSession_, 30)
}