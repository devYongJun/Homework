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

	// �̰� tcp ó���ϴ°�.
	// ���ε� ��ų�� ����?
	SetAcceptCallBack(StudyGameServerCore::AcceptEvent);

	// �⺻ ������ �����Ѵ���.
	// �� ������ �����Ҽ� �ִ� ����� �����ϴ� ���ۿ� ����.

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

	// n�ʵ��� ������ ������ ������ Ŀ�ؼǵ� ���� Ŀ�ؼ��̶�� ���� �߶󳻾� �Ѵ�.
	MessageHandler<TCPSession> Handler;
	// std::string MessageCode = std::to_string(Converter.GetMessageIdUint());
	// GameServerDebug::LogInfo(MessageCode + " Message In");
	if (false == Dis.GetHandler(Converter.GetMessageIdUint(), Handler))
	{
		// ���⼭�� �޼����� �߸�ó���� �ð��� 
		GameServerDebug::AssertDebugMsg("Message Error");
		return;
	}
	Handler(_S, Converter.GetServerMessage());

}

void StudyGameServerCore::CloseEvent(std::shared_ptr<TCPSession> _S)
{
	// Ŀ�ؼ��� ���ܼ� ����Ǿ����ϱ�.
	std::string LogText = std::to_string(static_cast<int>(_S->GetSocket()));
	GameServerDebug::LogInfo(LogText + "Connect End.");

	std::shared_ptr<ContentsUserData> UserData = _S->GetLink<ContentsUserData>(EDataIndex::USERDATA);
	// �α����� ���´�.

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


	// �̳༮�� ���Ǳ��� �����ؼ� �÷��̸� �ϴ� ������.

	// ����� ������ ���ǰ� ������ ������ ���� �����Դϴ�.

	// ����� ���� ������ �����ǰ� �ִµ�.

	// Ŭ��� �͵�
	// 
	// �����ϴ� ������ �츮�� ¥�� �մϴ�.

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

	// �� ������Ʈ ��°�� ���Ͱ�
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


	//// n�ʵ��� ������ ������ ������ Ŀ�ؼǵ� ���� Ŀ�ؼ��̶�� ���� �߶󳻾� �Ѵ�.
	//MessageHandler<UDPSession> Handler;
	//// std::string MessageCode = std::to_string(Converter.GetMessageIdUint());
	//// GameServerDebug::LogInfo(MessageCode + " Message In");
	//if (false == UDPDis.GetHandler(Converter.GetMessageIdUint(), Handler))
	//{
	//	// ���⼭�� �޼����� �߸�ó���� �ð��� 
	//	GameServerDebug::AssertDebugMsg("Message Error");
	//	return;
	//}
	//Handler(_S, Converter.GetServerMessage());
}

