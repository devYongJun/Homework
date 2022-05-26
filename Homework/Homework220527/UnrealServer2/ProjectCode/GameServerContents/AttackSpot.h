#pragma once
#include <GameServerCore\GameServerActor.h>
#include "ServerAndClient.h"
#include <GameServerNet\RedisConnecter.h>

class AttackSpot : public GameServerActor
{
public:
	// constrcuter destructer
	AttackSpot(FVector4 pos, int radius, int ownerIndex);
	~AttackSpot();

	// delete Function
	AttackSpot(const AttackSpot& _Other) = delete;
	AttackSpot(AttackSpot&& _Other) noexcept = delete;
	AttackSpot& operator=(const AttackSpot& _Other) = delete;
	AttackSpot& operator=(AttackSpot&& _Other) noexcept = delete;

	void Update(float _Time) override;

	void SectionInitialize() override;
	void TCPSessionInitialize() override;
	void UDPSessionInitialize() override;

	bool InsertSection() override;
	void DeathEvent() override;

public:
	int OwnerIndex = -1;
	bool Checked = false;
	GameServerCollision* HitCollision = nullptr;
	int Radius;
	FVector4 Pos;
};

