#pragma once

// Ό³Έν :
class GameServerActor
{
public:
	// constrcuter destructer
	GameServerActor();
	~GameServerActor();

	// delete Function
	GameServerActor(const GameServerActor& _Other) = delete;
	GameServerActor(GameServerActor&& _Other) noexcept = delete;
	GameServerActor& operator=(const GameServerActor& _Other) = delete;
	GameServerActor& operator=(GameServerActor&& _Other) noexcept = delete;

protected:

private:

};

