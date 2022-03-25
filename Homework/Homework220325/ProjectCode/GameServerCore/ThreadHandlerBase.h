#pragma once
#include <memory>
#include <functional>
#include <GameServerNet/TCPSession.h>
#include <GameServerMessage/Messages.h>
#include "DBQueue.h"
#include "NetQueue.h"

template<class MessageType>
class ThreadHandlerBase : public std::enable_shared_from_this<ThreadHandlerBase<MessageType>>
{
public:
	// message std::shared_ptr<T> req
	ThreadHandlerBase() {}

	virtual ~ThreadHandlerBase() {}

	void Init(
		std::shared_ptr<MessageType> _Message,
		std::shared_ptr<TCPSession> _TCPSession
	) 
	{
		Message_ = _Message;
		TCPSession_ = _TCPSession;
	}

	virtual void Start() = 0;

	template<typename ChildThreadHandler>
	void DBWork(void(ChildThreadHandler::* _ChildFunction)())
	{
		DBQueue::Queue(std::bind(_ChildFunction, std::dynamic_pointer_cast<ChildThreadHandler>(this->shared_from_this())));
	}
	template<typename ChildThreadHandler>
	void NetWork(void(ChildThreadHandler::* _ChildFunction)())
	{
		NetQueue::Queue(std::bind(_ChildFunction, std::dynamic_pointer_cast<ChildThreadHandler>(this->shared_from_this())));
	}


protected:
	std::shared_ptr<MessageType> Message_;
	std::shared_ptr<TCPSession> TCPSession_;
};


