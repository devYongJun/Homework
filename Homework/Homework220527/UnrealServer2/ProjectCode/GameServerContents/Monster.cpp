#include "PreCompile.h"
#include "Monster.h"
#include "GameServerCore\GameServerSection.h"
#include <GameServerBase\GameServerMath.h>
#include "ContentsSystemEnum.h"

Monster::Monster() 
	: Target(nullptr)
{
}

Monster::~Monster() 
{
	if (nullptr != HitCollision)
	{
		HitCollision->Death();
		HitCollision = nullptr;
	}
}

void Monster::StateUpdate(float _DeltaTime)
{
	EMonsterState State = static_cast<EMonsterState>(Message_.Data.State);

	switch (State)
	{
	case EMonsterState::NONE:
		break;
	case EMonsterState::MState_Idle:
		IdleUpdate(_DeltaTime);
		break;
	case EMonsterState::MState_Trace:
		TraceUpdate(_DeltaTime);
		break;
	case EMonsterState::MState_Att:
		AttUpdate(_DeltaTime);
		break;
	case EMonsterState::MAX:
		break;
	default:
		break;
	}
}


void Monster::Update(float _Time)
{
	if (nullptr != Target && false == Target->GetSectionCompare(GetSection()))
	{
		Target = nullptr;
		ChangeState(EMonsterState::MState_Idle);
		return;
	}

	if (Message_.Data.State == static_cast<int>(EMonsterState::MState_Att))
	{
		AttUpdate(_Time);
		return;
	}

	StateUpdate(_Time);
	BroadcastingMonsterUpdateMessage();
}

GameServerSerializer& Monster::GetSerializeMonsterUpdateMessage()
{
	Message_.Data.Dir = GetDir();
	Message_.Data.Pos = GetPos();
	Serializer_.Reset();

	Message_.Data.DeltaTime = GetAccTime();
	Message_.Serialize(Serializer_);

	return Serializer_;
}

void Monster::BroadcastingMonsterUpdateMessage()
{
	if (IsDeath() == false)
	{
		GetSection()->UDPBroadcasting(GetSerializeMonsterUpdateMessage().GetData());
	}
}

void Monster::SectionInitialize()
{

}
void Monster::TCPSessionInitialize()
{

}
void Monster::UDPSessionInitialize() 
{

}

bool Monster::InsertSection() 
{
	Message_.Data.ObjectIndex = GetIndex();
	Message_.Data.SectionIndex = GetSectionIndex();
	Message_.Data.ThreadIndex = GetThreadIndex();

	GameServerRandom Random;

	SetPos({ Random.Randomfloat(-900, 900), Random.Randomfloat(-900, 900), -200 });

	PlayerSensorCollision = GetSection()->CreateCollision(ECollisionGroup::MONSTERSENSOR, this);
	PlayerSensorCollision->SetScale({ 1000.0f, 1000.0f });

	HitCollision = GetSection()->CreateCollision(ECollisionGroup::MONSTER, this);
	HitCollision->SetScale({ 50.0f, 50.0f, 50.0f });

	ChangeState(EMonsterState::MState_Idle);

	return true;
}

// 객체로서 삭제되는 것
void Monster::DeathEvent() 
{
	GameServerDebug::LogInfo("Monster Dead");
}

void Monster::ChangeState(EMonsterState _State)
{
	switch (_State)
	{
	case EMonsterState::NONE:
		break;
	case EMonsterState::MState_Idle:
		IdleStart();
		break;
	case EMonsterState::MState_Trace:
		TraceStart();
		break;
	case EMonsterState::MState_Att:
		AttStart();
		break;
	case EMonsterState::MAX:
		break;
	default:
		break;
	}

	Message_.Data.State = static_cast<int>(_State);
}

void Monster::IdleStart() 
{
}


void Monster::TraceStart()
{

}

void Monster::AttStart() 
{
	AttTime = 1.0f;
}

void Monster::IdleUpdate(float _DeltaTime) 
{
	std::vector<GameServerCollision*> Result;

	if (true == PlayerSensorCollision->CollisionCheckResult(CollisionCheckType::SPHERE, ECollisionGroup::PLAYER, CollisionCheckType::SPHERE, Result))
	{
		Target = Result[0]->GetOwnerActor();
		ChangeState(EMonsterState::MState_Trace);
		return;
	}

	Result.clear();

}
void Monster::TraceUpdate(float _DeltaTime) 
{
	if (nullptr == Target)
	{
		ChangeState(EMonsterState::MState_Idle);
		return;
	}

	if ( 150.0f >= (Target->GetPos() - GetPos()).Length3DReturn())
	{
		ChangeState(EMonsterState::MState_Att);
		// BroadcastingMonsterUpdateMessage();
		return;
	}
	else 
	{
		SetDir((Target->GetPos() - GetPos()).Normalize3DReturn());
		//    1, 0               0.0
		Move((Target->GetPos() - GetPos()).Normalize3DReturn() * 400.0f * _DeltaTime);
	}

	if (false == Target->GetSectionCompare(GetSection()))
	{
		Target = nullptr;
		ChangeState(EMonsterState::MState_Idle);
		return;
	}

	if (false == Target->IsUpdate()
		|| true == Target->IsSectionMove()
		|| Target->IsDeath())
	{
		Target = nullptr;
		ChangeState(EMonsterState::MState_Idle);
		return;
	}
}


void Monster::AttUpdate(float _DeltaTime) 
{
	AttTime -= _DeltaTime;
	if (0.0f >= AttTime)
	{
		AttTime = 0.0f;
		ChangeState(EMonsterState::MState_Idle);
		return;
	}

}