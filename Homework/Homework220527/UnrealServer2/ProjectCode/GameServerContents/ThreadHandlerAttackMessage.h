#pragma once
#include <GameServerNet/TCPSession.h>
#include <GameServerMessage/Messages.h>
#include <GameServerCore\ThreadHandlerBase.h>
#include "ClientToServer.h"
#include "ServerToClient.h"
#include "ServerAndClient.h"

class ThreadHandlerAttackMessage final
	: public ThreadHandlerBase<AttackMessage>
{
public:
	void Start() override;
};

