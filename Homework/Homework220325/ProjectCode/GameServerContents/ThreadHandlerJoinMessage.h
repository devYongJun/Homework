#pragma once
#include <GameServerCore\ThreadHandlerBase.h>

// 그 세션도 여기서 받는다.
class ThreadHandlerJoinMessage final
	: public ThreadHandlerBase<JoinMessage>
{
private: // Member Var
	JoinResultMessage JoinResult_;

public: // Default
	ThreadHandlerJoinMessage();
	~ThreadHandlerJoinMessage();
	ThreadHandlerJoinMessage(const ThreadHandlerJoinMessage& _Other) = delete;
	ThreadHandlerJoinMessage(ThreadHandlerJoinMessage&& _Other) noexcept = delete;

	// 이 메세지를 처음 받았을때 실행하는 함수
	void Start() override;

public:


private:
	// 이 메세지를 처리하는 단계
	void DBCheck();

	// 이 메세지가 처리가 끝났을때 실행되는 함수
	void ResultSend();



protected:
	ThreadHandlerJoinMessage& operator=(const ThreadHandlerJoinMessage& _Other) = delete;
	ThreadHandlerJoinMessage& operator=(ThreadHandlerJoinMessage&& _Other) = delete;

};

