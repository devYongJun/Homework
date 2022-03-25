#include "PreCompile.h"
#include <GameServerContents\StudyGameServerCore.h>

// 해결안됨.
// 1. 두번째 접속자가 터지고 있죠.
// 2. 굳이 모든 포인터를 들고 있다가 삭제되게 할거냐.

// 리시브로 갑니다.

int main() 
{

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// 내 게임에서는 몬스터가 있을거야
	// 월드가 5개 있을거야.

	GameServerCore::Start<StudyGameServerCore>();
}