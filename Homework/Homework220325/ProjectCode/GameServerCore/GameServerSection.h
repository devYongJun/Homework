#pragma once

// Ό³Έν :
class GameServerSection
{
public:
	// constrcuter destructer
	GameServerSection();
	~GameServerSection();

	// delete Function
	GameServerSection(const GameServerSection& _Other) = delete;
	GameServerSection(GameServerSection&& _Other) noexcept = delete;
	GameServerSection& operator=(const GameServerSection& _Other) = delete;
	GameServerSection& operator=(GameServerSection&& _Other) noexcept = delete;

protected:

private:

};

