#include "PreCompile.h"
#include "StudyGameServerCore.h"
#include <GameServerNet\TCPSession.h>
#include <GameServerMessage\Messages.h>
#include <GameServerCore\GameServerSectionManager.h>
#include "ServerDispatcher.h"
#include "NoneFightZone.h"
#include "FightZone.h"
#include "ContentsEnum.h"
#include "ContentsSystemEnum.h"
#include "ContentsUserData.h"
#include "ContentsPlayerData.h"
#include "Player.h"
#include "Portal.h"
#include "ThreadHandlerPlayerUpdateMessageUDP.h"
#include "ContentsGlobalValue.h"

StudyGameServerCore::StudyGameServerCore()
{
}

StudyGameServerCore::~StudyGameServerCore()
{
}

void StudyGameServerCore::UserStart()
{
	DispatcherRegistration();

	// 이게 tcp 처리하는것.
	// 바인드 시킬수 있죠?
	SetAcceptCallBack(StudyGameServerCore::AcceptEvent);

	// 기본 전략을 강조한다음.
	// 그 전략을 수정할수 있는 방법을 제공하는 수밖에 없어.

	InitUDP(8, StudyGameServerCore::UDPRecvEvent);

	GameServerSectionManager::GetInst()->Init(3, "SectionThread");

	std::shared_ptr<NoneFightZone> Town = GameServerSectionManager::GetInst()->CreateSection<NoneFightZone>(0, ESectionType::NONEFIGHT);
	Town->SetName("PlayLevel");

	std::shared_ptr<FightZone> Field = GameServerSectionManager::GetInst()->CreateSection<FightZone>(1, ESectionType::FIGHTZONE1);
	Field->SetName("FightZone0");

	std::shared_ptr<Portal> ToField = Town->CreateActor<Portal>(FVector4(2500.0f, 300.0f, -300.0f), FVector4(100.0f, 100.0f, 100.0f));
	ToField->SetName("Portal");
	
	std::shared_ptr<Portal> ToTown = Field->CreateActor<Portal>(FVector4(-1700.0f, 640.0f, -300.0f), FVector4(100.0f, 100.0f, 100.0f));
	ToTown->SetName("Portal");

	ToTown->LinkSection = Town.get();
	ToField->LinkSection = Field.get();
}

void StudyGameServerCore::AcceptEvent(std::shared_ptr<TCPSession> _User)
{
	_User->SetRecvCallBack(RecvEvent);
	_User->SetCloseCallBack(CloseEvent);

	std::string LogText = std::to_string(static_cast<int>(_User->GetSocket()));
	GameServerDebug::LogInfo(LogText + "Connect.");
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
	// std::string MessageCode = std::to_string(Converter.GetMessageIdUint());
	// GameServerDebug::LogInfo(MessageCode + " Message In");
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
	// 커넥션이 끊겨서 종료되었으니까.
	std::string LogText = std::to_string(static_cast<int>(_S->GetSocket()));
	GameServerDebug::LogInfo(LogText + "Connect End.");

	std::shared_ptr<ContentsUserData> UserData = _S->GetLink<ContentsUserData>(EDataIndex::USERDATA);
	// 로그인은 끝냈다.

	std::shared_ptr<ContentsPlayerData> PlayerData = _S->GetLink<ContentsPlayerData>(EDataIndex::PLAYABLE);

	if (nullptr == PlayerData)
	{
		_S->ClearLinkObject();
		return;
	}

	//if (nullptr != UserData)
	//{
	//	ContentsGlobalValue::UnRegistPlayable(UserData->SelectData.Index);
	//}

	// ThreadHandlerSelectCharacterMessage::

	Player* ConnectionPlayer = PlayerData->ConnectPlayer;

	ConnectionPlayer->Death();


	_S->ClearLinkObject();


	// 이녀석은 섹션까지 집입해서 플레이를 하던 유저다.

	// 여기로 왔을때 세션과 연결이 끊기지 않은 상태입니다.

	// 여기는 아직 연결이 유지되고 있는데.

	// 클로즈가 와도
	// 
	// 정리하는 로직은 우리가 짜야 합니다.

	// _S->GetLink

	//_S->GetLink

}


void StudyGameServerCore::UDPRecvEvent(std::shared_ptr<UDPSession> _S, const std::vector<unsigned char>& _Value, IPEndPoint& EndPoint)
{
	// GameServerDebug::LogInfo("UDP");

	MessageConverter Converter = MessageConverter(_Value);

	if (false == Converter.IsVailed())
	{
		GameServerDebug::AssertDebugMsg("Convert Error");
		return;
	}

	int Id = Converter.GetMessageIdUint();

	if (static_cast<int>(MessageId::PlayerUpdate) != Id)
	{
		GameServerDebug::AssertDebugMsg("UDP Only PlayerUpdate Message");
		return;
	}

	// 이 오브젝트 번째의 액터가
	std::shared_ptr<PlayerUpdateMessage> ConvertMessage = std::dynamic_pointer_cast<PlayerUpdateMessage>(Converter.GetServerMessage());
	if (nullptr == ConvertMessage)
	{
		GameServerDebug::LogError("ConvertError");
		return;
	}

	std::shared_ptr<ThreadHandlerPlayerUpdateMessageUDP> Cmd = std::make_shared<ThreadHandlerPlayerUpdateMessageUDP>();
	Cmd->Point = EndPoint;
	Cmd->Init(ConvertMessage, _S);
	Cmd->Start();


	//// n초동안 연락이 완전히 두절된 커넥션도 좀비 커넥션이라고 보고 잘라내야 한다.
	//MessageHandler<UDPSession> Handler;
	//// std::string MessageCode = std::to_string(Converter.GetMessageIdUint());
	//// GameServerDebug::LogInfo(MessageCode + " Message In");
	//if (false == UDPDis.GetHandler(Converter.GetMessageIdUint(), Handler))
	//{
	//	// 여기서는 메세지를 잘못처리한 시간을 
	//	GameServerDebug::AssertDebugMsg("Message Error");
	//	return;
	//}
	//Handler(_S, Converter.GetServerMessage());
}

