#pragma once
#include <GameServerCore\GameServerSection.h>
#include <GameServerCore/GameServerActor.h>
#include "Monster.h"

class FightZone : public GameServerSection
{
	
public:
	// constrcuter destructer
	FightZone();
	~FightZone();
	
	void KillActor(std::shared_ptr<GameServerActor> _Actor);

	// delete Function
	FightZone(const FightZone& _Other) = delete;
	FightZone(FightZone&& _Other) noexcept = delete;
	FightZone& operator=(const FightZone& _Other) = delete;
	FightZone& operator=(FightZone&& _Other) noexcept = delete;

protected:
	std::atomic<int> MonsterCount = 0;
	float MonsterGenTime = 0.0f;

	std::atomic<size_t> WaitKillActorCount_;
	std::mutex WaitKillLock;
	std::vector<std::shared_ptr<GameServerActor>> WaitKillActors_;

private:
	void SectionUpdate(float _Delta);
	bool Init() override;
};

