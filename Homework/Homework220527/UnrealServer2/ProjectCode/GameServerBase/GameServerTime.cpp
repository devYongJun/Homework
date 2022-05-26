#include "PreCompile.h"
#include "GameServerTime.h"

GameServerTime::GameServerTime() 
{
}

GameServerTime::~GameServerTime() 
{
}



void GameServerTime::Reset()
{
	Current_ = Timer_.now();
	Prev_ = Timer_.now();
}

// 애니메이션은 0.2초에 공격을 처리해야합니다.
// 서버    클라
//         0.0 공격 애니메이션 시작
//  0.02        받았어 클라야 니 공격 성공이야.
//  0.2        0.04
//         0.5

float GameServerTime::Update()
{
	Current_ = Timer_.now();
	DoubleDeltaTime = std::chrono::duration<double>(Current_ - Prev_).count();
	Prev_ = Current_;

	DeltaTime = static_cast<float>(DoubleDeltaTime);
	return DeltaTime;
}