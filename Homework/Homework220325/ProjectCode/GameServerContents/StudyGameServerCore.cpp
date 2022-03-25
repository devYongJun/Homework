#include "PreCompile.h"
#include "StudyGameServerCore.h"
#include <GameServerNet\TCPSession.h>
#include <GameServerMessage\Messages.h>
#include "ServerDispatcher.h"

StudyGameServerCore::StudyGameServerCore()
{
}

StudyGameServerCore::~StudyGameServerCore()
{
}

void StudyGameServerCore::UserStart()
{
	DispatcherRegistration();
	// 바인드 시킬수 있죠?
	SetAcceptCallBack(StudyGameServerCore::AcceptEvent);

	// 고정형 맵
	// AddSection("키를 만들어야 한다.", 30);

	// 현재 섹션의 인원수 등을 파악해서 몇명에서 늘릴지
	
	// Section* Ptr = GetSection();
	// Ptr->UserCount();
	// AddSection("키를 만들어야 한다.", 30);
	// AddSection<Lobby>("로비", -1);
	// AddSection<PlayRoom>("로비", 30);

}

void StudyGameServerCore::AcceptEvent(std::shared_ptr<TCPSession> _User)
{
	_User->SetRecvCallBack(RecvEvent);

	_User->SetCloseCallBack(CloseEvent);

	std::string LogText = std::to_string(static_cast<int>(_User->GetSocket()));
	GameServerDebug::LogInfo(LogText + "Connect.");
	// FindSection("로비");
}


void StudyGameServerCore::RecvEvent(std::shared_ptr<TCPSession> _S, const std::vector<unsigned char>& _Value)
{
	MessageConverter Converter = MessageConverter(_Value);

	if (false == Converter.IsVailed())
	{
		GameServerDebug::AssertDebugMsg("Convert Error");
		return;
	}

	// n초동안 연락이 완전히 두절된 커넥션도 좀비 커넥션이라고 보고 잘라내야 한다.
	MessageHandler<TCPSession> Handler;
	std::string MessageCode = std::to_string(Converter.GetMessageIdUint());
	GameServerDebug::LogInfo(MessageCode + " Message In");
	if (false == Dis.GetHandler(Converter.GetMessageIdUint(), Handler))
	{
		// 여기서는 메세지를 잘못처리한 시간을 
		GameServerDebug::AssertDebugMsg("Message Error");
		return;
	}
	Handler(_S, Converter.GetServerMessage());

}

void StudyGameServerCore::CloseEvent(std::shared_ptr<TCPSession> _S)
{
	std::string LogText = std::to_string(static_cast<int>(_S->GetSocket()));
	GameServerDebug::LogInfo(LogText + "Connect End.");
}

