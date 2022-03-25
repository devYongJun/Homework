#include "ThreadHandlerJoinResultMessage.h"
#include "../../Global/ClientGameInstance.h"
#include "Kismet/GameplayStatics.h"

ThreadHandlerJoinResultMessage::ThreadHandlerJoinResultMessage(std::shared_ptr<JoinResultMessage> _JoinResultMessage)
	: JoinResultMessage_(_JoinResultMessage)
{
	// UGameplayStatics::
}

void ThreadHandlerJoinResultMessage::Init(UClientGameInstance* _Inst, UWorld* _World)
{
	Inst_ = _Inst;
	World_ = _World;
}


void ThreadHandlerJoinResultMessage::Start()
{

	// EGameServerCode Code

	if (EGameServerCode::OK == JoinResultMessage_->Code)
	{
		//UGameplayStatics::OpenLevel(World_, TEXT("PlayLevel"));
		return;
	}

	// ·Î±×ÀÎ ui
}

