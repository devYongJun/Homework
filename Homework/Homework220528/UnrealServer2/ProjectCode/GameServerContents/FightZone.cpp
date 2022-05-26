#include "PreCompile.h"
#include "FightZone.h"
#include <GameServerNet/TCPSession.h>
#include <GameServerNet/UDPSession.h>


FightZone::FightZone() 
{
	WaitKillActors_.reserve(100);
}

FightZone::~FightZone() 
{
}


void FightZone::SectionUpdate(float _Delta)
{
	MonsterGenTime -= _Delta;

	if (MonsterCount == 0 && MonsterGenTime <= 0.0f)
	{
		std::shared_ptr<Monster> monster = CreateActor<Monster>();
		monster->SetName("Monster");
		MonsterCount++;
	}

	if (WaitKillActorCount_ != 0)
	{
		std::lock_guard<std::mutex> lock(WaitKillLock);

		for (size_t i = 0; i < WaitKillActors_.size(); i++)
		{
			WaitKillActors_[i]->ClearLinkObject();
			WaitKillActors_[i]->Death();
			AIActors_.remove(WaitKillActors_[i]);
		}
		WaitKillActors_.clear();
		WaitKillActorCount_ = 0;
	}
}

void FightZone::KillActor(std::shared_ptr<GameServerActor> _Actor)
{
	if (_Actor->GetNameView() == "Monster")
	{
		MonsterCount--;
		if (MonsterCount == 0)
		{
			MonsterGenTime = 1.0f;
		}
	}
	_Actor->Death();
	_Actor->SetSection(nullptr);

	WaitKillActors_.push_back(_Actor);
	WaitKillActorCount_++;
}

bool FightZone::Init() 
{
	return true;
}