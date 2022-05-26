#include "PreCompile.h"
#include "GameServerSection.h"
#include "GameServerActor.h"
#include <GameServerNet\TCPSession.h>
#include <GameServerNet\UDPSession.h>


GameServerSection::GameServerSection()
	: Index_(0)
{
	WaitJoinActors_.reserve(100);
	
}

GameServerSection::~GameServerSection()
{
}

// �̰� ȣ�����ִ°� SectionThread�� ���� �̴ϴ�.
bool GameServerSection::Update(float _Time)
{
	SectionUpdate(_Time);

	for (auto& Actor : PlayableActors_)
	{
		if (false == Actor->IsUpdate())
		{
			continue;
		}

		Actor->AccTimeUpdate(_Time);
		Actor->Update(_Time);
	}

	for (auto& Actor : AIActors_)
	{
		if (false == Actor->IsUpdate())
		{
			continue;
		}

		Actor->AccTimeUpdate(_Time);
		Actor->Update(_Time);
	}

	for (size_t i = 0; i < MoveActors_.size(); i++)
	{
		KeyActors_.erase(MoveActors_[i].MoveActor->GetIndex());
		PlayableActors_.remove(MoveActors_[i].MoveActor);
		MoveActors_[i].NextSection->MoveActor(MoveActors_[i].MoveActor);
	}
	MoveActors_.clear();

	if (0 != WaitJoinActorCount_)
	{
		std::lock_guard<std::mutex> lock(WaitJoinLock);

		for (size_t i = 0; i < WaitJoinActors_.size(); i++)
		{
			for (auto& Actor : PlayableActors_)
			{
				WaitJoinActors_[i]->PlayerbleActorEvent(Actor.get());
			}

			for (auto& Actor : AIActors_)
			{
				WaitJoinActors_[i]->AIActorEvent(Actor.get());
			}

			// �ܺο����� �÷��̾���� �־��ټ��� �ִ�.
			// �ܺο����� ���͸� ������� ����!
			if (nullptr != WaitJoinActors_[i]->GetTCPSession())
			{
				PlayableActors_.push_back(WaitJoinActors_[i]);
			}
			else 
			{
				AIActors_.push_back(WaitJoinActors_[i]);
			}

			// �ھ�Լ� udp�ϳ� ���ͼ� �װ͵� �޾ƾ� �Ѵ�.
			// �׷��� �װɷ� send���Ҽ� �����ϱ�.
			WaitJoinActors_[i]->SetSection(this);
			WaitJoinActors_[i]->AccTimeReset();

			if (false == WaitJoinActors_[i]->InsertSection())
			{
				// ���� ������. �α׸� �����ټ��� �������̴�.
				// �����˴ϴ�.
				continue;
			}
			KeyActors_.insert(std::make_pair(WaitJoinActors_[i]->GetIndex(), WaitJoinActors_[i]));
			WaitJoinActors_[i]->SectionMoveEnd();
		}
		WaitJoinActors_.clear();
		WaitJoinActorCount_ = 0;
	}

	return true;
}

void GameServerSection::SectionMove(std::shared_ptr<GameServerActor> _MoveActor, GameServerSection* _MoveSection)
{
	MoveActors_.push_back({ _MoveActor, _MoveSection });
}

void GameServerSection::Release() 
{
	{
		// ���ǿ����� ������ �޸𸮰� ����
		std::list<std::shared_ptr<GameServerActor>>::iterator StartIter = PlayableActors_.begin();
		std::list<std::shared_ptr<GameServerActor>>::iterator EndIter = PlayableActors_.end();

		// �߰��� 
		// �ϳ� 
		for (; StartIter != EndIter; )
		{
			if (false == (*StartIter)->IsDeath())
			{
				++StartIter;
				continue;
			}


			(*StartIter)->DeathEvent();
			KeyActors_.erase((*StartIter)->GetIndex());
			StartIter = PlayableActors_.erase(StartIter);
		}
	}

	{
		// ���ǿ����� ������ �޸𸮰� ����
		std::list<std::shared_ptr<GameServerActor>>::iterator StartIter = AIActors_.begin();
		std::list<std::shared_ptr<GameServerActor>>::iterator EndIter = AIActors_.end();

		// �߰��� 
		// �ϳ� 
		for (; StartIter != EndIter; )
		{
			if (false == (*StartIter)->IsDeath())
			{
				++StartIter;
				continue;
			}

			(*StartIter)->DeathEvent();
			KeyActors_.erase((*StartIter)->GetIndex());
			StartIter = AIActors_.erase(StartIter);
		}
	}


	//for (auto& Actor : AIActor_)
	//{
	//	if (false == Actor->())
	//	{
	//		continue;
	//	}

	//	Actor->AccTimeUpdate(_Time);
	//	Actor->Update(_Time);
	//}
}


void GameServerSection::MoveActor(std::shared_ptr<GameServerActor> _Actor)
{
	// �ű�� ������ �Ϸ�Ǳ� ������
	_Actor->SetSection(nullptr);
	// �̳����� �������Ÿ��� �Ϸ��� �ϸ� ��Ʈ�� �����Դϴ�.

	// ���� �ٸ��������� �Ϻ��ϰ� �Ѿ�� �˼��� ����.
	// �׶� ������ ���� ����� �ϴ� ���ǿ� �̳༮������ ���� ���� �Ϸ��� �ϸ�
	// ������ ������� �Ѵ�.
	InsertActor(_Actor->GetIndex(), _Actor);
}

// �����忡�� �������� ���ƿü� �ִ� ����̴�.
void GameServerSection::InsertActor(uint64_t _ID, std::shared_ptr<GameServerActor> _Actor)
{
	if (_ID == -1)
	{
		GameServerDebug::AssertDebugMsg("actor index is -1 Fuck you !!!!!!");
		// GameServerDebug::AssertDebugMsg("���ر� �븮������ ������!!!!!!");
		return;
	}

	std::lock_guard<std::mutex> lock(WaitJoinLock);
	_Actor->SetSectionIndex(GetIndex());
	_Actor->SetThreadIndex(GetThreadIndex());
	WaitJoinActors_.push_back(_Actor);
	WaitJoinActorCount_ = WaitJoinActors_.size();

}

// �����浹üũ�� �ѹ� �ϰ� �����ų�?
void GameServerSection::TCPBroadcasting(const std::vector<unsigned char>& _Buffer, uint64_t _IgnoreIndex /*= -1*/)
{
	std::string Text;
	for (auto& Actors : PlayableActors_)
	{
		if (_IgnoreIndex == Actors->GetIndex())
		{
			continue;
		}

		Text += std::to_string(Actors->GetIndex()) + " ";

		if (false == Actors->GetTCPSession()->Send(_Buffer))
		{
			GameServerDebug::AssertDebugMsg("Broadcasting Error " + std::to_string(Actors->GetIndex()));
		}
		;
	}

	GameServerDebug::Log(LOGTYPE::LOGTYPE_INFO, Text);
}

// ObjectMessageComponent.cpp �޼��� 40���� ����.

void GameServerSection::UDPBroadcasting(const std::vector<unsigned char>& _Buffer, uint64_t _IgnoreIndex/* = -1*/)
{
	for (auto& Actors : PlayableActors_)
	{
		if (_IgnoreIndex == Actors->GetIndex())
		{
			continue;
		}

		// UDP����� �ѹ��� ������ �������� ���� �༮�̴�.

		int Port = Actors->GetUDPEndPointConstRef().GetPort();

		if (0 == Port)
		{
			continue;
		}

		// ���� ����� Ŭ���̾�Ʈ�� �ص�����Ʈ�� �̹� �˰� �־�� �ϰ�
		Actors->GetUDPSession()->Send(_Buffer, Actors->GetUDPEndPointConstRef());
	}
}

void GameServerSection::ActorPost(uint64_t _ObjectIndex, std::shared_ptr<GameServerMessage> _Message)
{
	// std::lock_guard<std::mutex> lock(WaitLock);

	// ����static�� 1���� �ʱ�ȭ ����?
	std::map<uint64_t, std::shared_ptr<GameServerActor>>::iterator FindIter;

	FindIter = KeyActors_.find(_ObjectIndex);

	if (KeyActors_.end() == FindIter)
	{
		GameServerDebug::AssertDebugMsg("�������� �ʴ� ���Ϳ� �޼����� ���½��ϴ�.");
		return;
	}

	if (nullptr == FindIter->second)
	{
		GameServerDebug::AssertDebugMsg("�������� �ʴ� ���Ϳ� �޼����� ���½��ϴ�.");
		return;
	}

	// �̵����̴�.
	if (true == FindIter->second->IsSectionMove_)
	{
		return;
	}
	


	FindIter->second->Messagequeue_.push(_Message);

}

void GameServerSection::ActorPointPost(uint64_t _ObjectIndex, const IPEndPoint& _EndPoint, std::shared_ptr<GameServerMessage> _Message) 
{
	// std::lock_guard<std::mutex> lock(WaitLock);

	// ����static�� 1���� �ʱ�ȭ ����?
	std::map<uint64_t, std::shared_ptr<GameServerActor>>::iterator FindIter;

	FindIter = KeyActors_.find(_ObjectIndex);

	if (KeyActors_.end() == FindIter)
	{
		GameServerDebug::AssertDebugMsg("�������� �ʴ� ���Ϳ� �޼����� ���½��ϴ�.");
		return;
	}

	if (nullptr == FindIter->second)
	{
		GameServerDebug::AssertDebugMsg("�������� �ʴ� ���Ϳ� �޼����� ���½��ϴ�.");
		return;
	}

	FindIter->second->UDPEndPoint = _EndPoint;

	FindIter->second->Messagequeue_.push(_Message);

}

bool GameServerSection::Init()
{
	return true;
}

GameServerCollision* GameServerSection::CreateCollision(int _Order, GameServerActor* _Owner)
{
	GameServerCollision* NewCollision = new GameServerCollision();
	NewCollision->SetGroupIndex(_Order);
	NewCollision->OwnerSection = this;
	NewCollision->OwnerActor = _Owner;
	CollisionList[_Order].push_back(NewCollision);

	return NewCollision;
}
