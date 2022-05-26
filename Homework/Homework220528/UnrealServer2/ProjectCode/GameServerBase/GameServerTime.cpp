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

// �ִϸ��̼��� 0.2�ʿ� ������ ó���ؾ��մϴ�.
// ����    Ŭ��
//         0.0 ���� �ִϸ��̼� ����
//  0.02        �޾Ҿ� Ŭ��� �� ���� �����̾�.
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