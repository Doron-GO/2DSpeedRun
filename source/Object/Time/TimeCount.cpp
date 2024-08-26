#include "TimeCount.h"
#include "../Stage/CheckPoint.h"
#include<DxLib.h>

TimeCount::TimeCount(CheckPoint& CheckPoint):checkPoint_(CheckPoint),oldTime_(0.0f),
deltaTime_(0.000001f),elapsedTime_(0.0f), nowTime_(0.0f), startFlag_(false),
_draw(&TimeCount::GameDraw),_update(&TimeCount::GameUpdate), endFlag_(false), record_(0.0f)
{
	goalImg_ = LoadGraph("Src/Img/UIimage/Goal.png");
	restertImg_ = LoadGraph("Src/Img/UIimage/RESTERT.png");
	fontHandle_ = CreateFontToHandle(NULL, 70, 3);
}

TimeCount::~TimeCount()
{
}

void TimeCount::Update(float startime)
{
	(this->*_update)( startime);
}

void TimeCount::Draw()
{
	(this->*_draw)();
}

void TimeCount::GoalUpdate(float startime)
{
	Count();
}

void TimeCount::GoalDraw()
{
	DrawGraph(0, -200, goalImg_, true);
	if (elapsedTime_>= record_ +2.0f)
	{
		//DrawRotaGraph2F(800.0f, 700.0f, 208.0f, 20.0f, 1.0, 0.0, restertImg_, true);
		DrawFormatStringToHandle(500, 600, 0xff0000, fontHandle_, "RECORD:%f", record_);
		endFlag_ = true;
	}
}

void TimeCount::GameUpdate(float startime)
{
	if (startime >= 2.0f && (!startFlag_))
	{
		SetStart();
		startFlag_ = true;
	}
	Count();
	if (checkPoint_.IsGoal())
	{
		record_ = elapsedTime_;
		_draw = &TimeCount::GoalDraw;
		_update = &TimeCount::GoalUpdate;
	}

}

void TimeCount::GameDraw()
{	
}

void TimeCount::SingleDraw()
{
	DrawFormatStringToHandle(0, 0, 0xff0000, fontHandle_, "RECORD:%f", elapsedTime_);

}

void TimeCount::SetStart()
{
	oldTime_ = static_cast<float>( GetNowHiPerformanceCount() );
}

bool TimeCount::IsEnd()
{
	return endFlag_;
}

float TimeCount::ElapsedTime()
{
	return elapsedTime_;
}

void TimeCount::Count()
{
	nowTime_ = static_cast<float>( GetNowHiPerformanceCount() );
	deltaTime_ = (nowTime_ - oldTime_) / 1000000.0f;
	oldTime_ = nowTime_;
	elapsedTime_ += deltaTime_;
}

void TimeCount::SinglePlay()
{
	_draw = &TimeCount::SingleDraw;
}
