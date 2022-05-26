#include "PreCompile.h"
#include "AttackSpot.h"
#include "Monster.h"
#include "Player.h"
#include "ClientToServer.h"
#include "ServerToClient.h"
#include "GameServerCore\GameServerCore.h"
#include "GameServerBase\GameServerThread.h"
#include <GameServerNet\TCPSession.h>
#include "ContentsSystemEnum.h"
#include "ContentsUserData.h"
#include "CharacterTable.h"
#include "FightZone.h"
#include "GameServerBase\GameServerDebug.h"
#include <GameServerCore\GameServerSection.h>
#include <GameServerCore\DBQueue.h>

AttackSpot::AttackSpot(FVector4 pos, int radius, int ownerIndex)
	: HitCollision(nullptr)
	, Pos(pos)
	, Radius(radius)
	, OwnerIndex(ownerIndex)
{

}

AttackSpot::~AttackSpot()
{
	if (nullptr != HitCollision)
	{
		HitCollision->Death();
		HitCollision = nullptr;
	}
}

void AttackSpot::Update(float _Time)
{
	if (Checked == false)
	{
		FightZone* zone = (FightZone*)GetSection();
		GameServerActor* target = nullptr;
		const std::list<std::shared_ptr<GameServerActor>>& AllMonsters = GetSection()->GetAiActors();
		
		for (auto& OtherActor : AllMonsters)
		{
			std::vector<GameServerCollision*> Result;
			if (true == HitCollision->CollisionCheckResult(CollisionCheckType::SPHERE, ECollisionGroup::MONSTER, CollisionCheckType::SPHERE, Result))
			{
				target = Result[0]->GetOwnerActorConvert<Monster>();
				break;
			}
		}

		if (target != nullptr)
		{
			AttackResultMessage msg;
			msg.ObjectIndex = target->GetIndex();
			msg.ThreadIndex = target->GetThreadIndex();
			msg.SectionIndex = target->GetSectionIndex();
			msg.Hp = 0;

			GameServerSerializer Sr;
			msg.Serialize(Sr);

			std::list<std::shared_ptr<GameServerActor>> playerList = GetSection()->GetPlayableActor();

			for (auto& OtherActor : playerList)
			{
				std::shared_ptr<Player> OtherPlayer = std::dynamic_pointer_cast<Player>(OtherActor);
				OtherPlayer->GetTCPSession()->Send(Sr.GetData());
			}
			if (zone != nullptr)
			{
				zone->KillActor(target->DynamicCast<GameServerActor>());
			}
			for (auto& OtherActor : playerList)
			{
				if (OtherActor->GetIndex() == OwnerIndex)
				{
					std::shared_ptr<Player> owner = std::dynamic_pointer_cast<Player>(OtherActor);
					std::string ownerName = owner->UserData->SelectData.NickName;
					DBQueue::Queue([=]
					{
						std::shared_ptr<RedisConnecter> Connector = GetBaseRankConntor();

						RedisCommend::ZSCORE MyKillCount = RedisCommend::ZSCORE("UserRank", ownerName);
						MyKillCount.Process(*Connector);
						int killCount = MyKillCount.value;
						killCount++;

						RedisCommend::ZADD KillMonsterAddCount = RedisCommend::ZADD("UserRank", killCount, ownerName);
						KillMonsterAddCount.Process(*Connector);
					});
				}
			}
			
		}
		if (zone != nullptr)
		{ 
			zone->KillActor(DynamicCast<GameServerActor>());
		}
		Checked = true;
	}
}

void AttackSpot::SectionInitialize()
{
	float radius = static_cast<float>(Radius);
	if (nullptr == HitCollision)
	{
		HitCollision = GetSection()->CreateCollision(ECollisionGroup::ATTACK, this);
		HitCollision->SetScale({ radius, radius, radius });
	}
	SetPos(Pos);
	SetScale({ radius, radius, radius });
}

void AttackSpot::TCPSessionInitialize()
{
	
}

void AttackSpot::UDPSessionInitialize()
{
}

bool AttackSpot::InsertSection()
{
	return true;
}

void AttackSpot::DeathEvent()
{
}
