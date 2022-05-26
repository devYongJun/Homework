#pragma once
#include <GameServerNet/TCPSession.h>
#include <GameServerMessage/Messages.h>
#include <GameServerCore\ThreadHandlerBase.h>
#include "ClientToServer.h"
#include "ServerToClient.h"
#include "ServerAndClient.h"


// �� ���ǵ� ���⼭ �޴´�.
class ThreadHandlerBigMessage final
	: public ThreadHandlerBase<BigMessage>
{
private: // Member Var

public: // Default
	ThreadHandlerBigMessage();
	~ThreadHandlerBigMessage();
	ThreadHandlerBigMessage(const ThreadHandlerBigMessage& _Other) = delete;
	ThreadHandlerBigMessage(ThreadHandlerBigMessage&& _Other) noexcept = delete;
	ThreadHandlerBigMessage& operator=(ThreadHandlerBigMessage&& _Other) = delete;
	ThreadHandlerBigMessage& operator=(const ThreadHandlerBigMessage& _Other) = delete;

	void Start() override;

private:

protected:

};