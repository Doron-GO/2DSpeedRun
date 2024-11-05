#include "CheckPoint.h"
#include"../../Object/Player/Player.h"

constexpr int CHECKPOINT_MAX = 6;

CheckPoint::CheckPoint(std::vector<std::shared_ptr<Player>> players, PointColList checkpoint)
	:players_(players), checkPointColList_(checkpoint), goalFlag_(false),update_(&CheckPoint::MultiPlayUpdate)
{

	//àÍî‘âE
	checkPoints_.push_back(std::pair<DIRCTION, Vector2DFloat>{ DIRCTION::VERTICAL,{9800.0f,0.0f} });
	//àÍî‘Å™
	checkPoints_.push_back(std::pair<DIRCTION, Vector2DFloat>{ DIRCTION::HORIZONTAL, { 0.0f,0.0f } });
	//àÍî‘ç∂
	checkPoints_.push_back(std::pair<DIRCTION, Vector2DFloat>{ DIRCTION::VERTICAL, { 0.0f,0.0f } });
	//àÍî‘â∫
	checkPoints_.push_back(std::pair<DIRCTION, Vector2DFloat>{ DIRCTION::HORIZONTAL, { 0.0f,3000.0f } });
	//àÍî‘âE
	checkPoints_.push_back(std::pair<DIRCTION, Vector2DFloat>{ DIRCTION::VERTICAL, { 9800.0f,0.0f } });
	//àÍî‘â∫
	checkPoints_.push_back(std::pair<DIRCTION, Vector2DFloat>{ DIRCTION::HORIZONTAL, { 0.0f,3000.0f } });
	//àÍî‘ç∂
	checkPoints_.push_back(std::pair<DIRCTION, Vector2DFloat>{ DIRCTION::VERTICAL, { 0.0f,0.0f } });
	//àÍî‘â∫
	checkPoints_.push_back(std::pair<DIRCTION, Vector2DFloat>{ DIRCTION::HORIZONTAL , { 0.0f,3000.0f } });

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
			if (rayCast_.RectToRectCollision(checkPointColList_[currentCheckPoint_].first, checkPointColList_[currentCheckPoint_].second,
			player->colRect_.min_, player->colRect_.max_))
			{
				(this->*update_)();
				break;
			}
		}
	}
}

std::pair<CheckPoint::DIRCTION, Vector2DFloat> CheckPoint::GetCheckPoint() const
{
	return checkPoints_[currentCheckPoint_];
}

const bool CheckPoint::IsGoal()
{
	return goalFlag_;
}

void CheckPoint::SetSinglePlayMode()
{
	update_ = &CheckPoint::SinglePlayUpdate;
}

void CheckPoint::SinglePlayUpdate()
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

void CheckPoint::MultiPlayUpdate()
{
	if (currentCheckPoint_ < CHECKPOINT_MAX) { currentCheckPoint_++; }
	else
	{
		currentCheckPoint_ = 0;
	}
}
