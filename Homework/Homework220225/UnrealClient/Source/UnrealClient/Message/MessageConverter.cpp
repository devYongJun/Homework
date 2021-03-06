#include "MessageConverter.h"
#include <memory>
MessageConverter::MessageConverter(const std::vector<unsigned char>&_buffer)
	: buffer_(_buffer)
{
	GameServerSerializer Sr = GameServerSerializer(buffer_);

	MessageId Type;
	memcpy_s(&Type, sizeof(MessageId), &buffer_[0], sizeof(MessageId));
	switch (Type)
	{
	case MessageId::Login:
		Message_ = std::make_shared<LoginMessage>();
		break;
	case MessageId::LoginResult:
		Message_ = std::make_shared<LoginResultMessage>();
		break;
	case MessageId::ServerDestroy:
		Message_ = std::make_shared<ServerDestroyMessage>();
		break;
	case MessageId::MonsterUpdate:
		Message_ = std::make_shared<MonsterUpdateMessage>();
		break;
	case MessageId::Test:
		Message_ = std::make_shared<TestMessage>();
		break;
	case MessageId::Chat:
		Message_ = std::make_shared<ChatMessage>();
		break;
	default:
		return;
	}
	Message_->DeSerialize(Sr);
}