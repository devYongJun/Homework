#pragma once
#include <vector>
#include <GameServerBase\GameServerThread.h>
#include <unordered_map>
#include "GameServerSection.h"
// 설명 :
class GameServerSectionManager
{
public:
	// constrcuter destructer
	GameServerSectionManager();
	~GameServerSectionManager();

	// delete Function
	GameServerSectionManager(const GameServerSectionManager& _Other) = delete;
	GameServerSectionManager(GameServerSectionManager&& _Other) noexcept = delete;
	GameServerSectionManager& operator=(const GameServerSectionManager& _Other) = delete;
	GameServerSectionManager& operator=(GameServerSectionManager&& _Other) noexcept = delete;

protected:

private:
	// 섹션이 100개라고 쓰레드가 100개가 되는게 아닙니다.
	// 생각하기 쉽다고 효율이 좋은건 아니야.
	// 모두가 적당히 놀아주기를 바래야 하거나
	// 한맵에 많은 유저가 있기를 바라면 안되죠?
	// 
	// 쓰레드 10개 섹션 100개를 따로 처리하는 방식이 됩니다.
	// Post죠.
	// 전반적으로 느리게 됩니다.

	std::vector<std::shared_ptr<GameServerThread>> SecitonThread_;

	// 섹션을 추가하는 순간만큼은 락을 걸어야 합니다.
	// -1이면 룸에 들어가 있지 않은것.
	std::unordered_map<uint32_t, std::shared_ptr<GameServerSection>> AllSeciton_;


};

