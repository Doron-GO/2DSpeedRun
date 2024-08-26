#include "CheckPoint.h"
#include"../../Object/Player/Player.h"

CheckPoint::CheckPoint(std::vector<std::shared_ptr<Player>> players, PointColList checkpoint)
	:players_(players), checkPointColList_(checkpoint), goalFlag_(false),_work(&CheckPoint::MultiPlay)
{

	//ˆê”Ô‰E
	checkPoints_.push_back(std::pair<bool, Vector2DFloat>{ false,{9800.0f,0.0f} });
	//ˆê”Ôª
	checkPoints_.push_back(std::pair<bool, Vector2DFloat>{ true,{0.0f,0.0f} });
	//ˆê”Ô¶
	checkPoints_.push_back(std::pair<bool, Vector2DFloat>{ false,{0.0f,0.0f} });
	//ˆê”Ô‰º
	checkPoints_.push_back(std::pair<bool, Vector2DFloat>{ true,{0.0f,3000.0f} });
	//ˆê”Ô‰E
	checkPoints_.push_back(std::pair<bool, Vector2DFloat>{ false,{9800.0f,0.0f} });
	//ˆê”Ô‰º
	checkPoints_.push_back(std::pair<bool, Vector2DFloat>{ true,{0.0f,3000.0f} });
	//ˆê”Ô¶
	checkPoints_.push_back(std::pair<bool, Vector2DFloat>{ false,{0.0f,0.0f} });
	//ˆê”Ô‰º
	checkPoints_.push_back(std::pair<bool, Vector2DFloat>{ true,{0.0f,3000.0f} });


	currentCheckPoint_ = 0;
}

CheckPoint::~CheckPoint()
{
}

void CheckPoint::Update()
{
	for (const auto& player : players_)
	{
		if (player->IsAlive())
		{
			auto& colRect = player->GetCol_Rect();
			if (rayCast_.RectToRectCollision(checkPointColList_[currentCheckPoint_].first, checkPointColList_[currentCheckPoint_].second,
				colRect.min_, colRect.max_))
			{
				(this->*_work)();
				break;
			}
		}
	}
}

std::pair<bool, Vector2DFloat> CheckPoint::GetCheckPoint2() const
{
	return checkPoints_[currentCheckPoint_];
}

const bool CheckPoint::IsGoal()
{
	return goalFlag_;
}

void CheckPoint::SetSingleMode()
{
	_work = &CheckPoint::SinglePlay;
}

void CheckPoint::SinglePlay()
{
	if (currentCheckPoint_ < CHECKPOINT_MAX)
	{ 
		currentCheckPoint_++; 
	}
	else
	{
		goalFlag_ = true;
		currentCheckPoint_ = 0;
	}
}

void CheckPoint::MultiPlay()
{
	if (currentCheckPoint_ < CHECKPOINT_MAX) { currentCheckPoint_++; }
	else
	{
		currentCheckPoint_ = 0;
	}
}
