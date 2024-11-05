#include "TimeCount.h"
#include "../Stage/CheckPoint.h"
#include<DxLib.h>


const float LIMIT_TIME_START = 2.0f;

const float LIMIT_TIME_DRAW = 2.0f;

const float LIMIT_TIME_DRAW_RECORD = 1.0f;


const int POSX_IMG_GOAL = 0;
const int POSY_IMG_GOAL = -200;

const int POSX_RECORD = 0;
const int POSY_RECORD = 0;

const int POSX_FINAL_RECORD = 350;
const int POSY_FINAL_RECORD = 600;

//デルタタイムを作る際に使う定数(割る)
const float DELTA_DIVIDE = 1000000.0f;



TimeCount::TimeCount(CheckPoint& CheckPoint):checkPoint_(CheckPoint),oldTime_(0.0f),
deltaTime_(0.000001f),elapsedTime_(0.0f), nowTime_(0.0f), startFlag_(false),
endFlag_(false), record_(0.0f), startTime_(0.0f)
{
	goalImg_ = LoadGraph("Src/Img/UIimage/Goal.png");
	restertImg_ = LoadGraph("Src/Img/UIimage/RESTERT.png");
	finalRecordFontHandle_ = CreateFontToHandle(NULL, 130, 3);
	recordFontHandle_ = CreateFontToHandle(NULL, 50, 3);

	update_.emplace(STATE::GAME, std::bind(&TimeCount::GameUpdate, this));
	update_.emplace(STATE::SINGLE, std::bind(&TimeCount::GameUpdate, this));
	update_.emplace(STATE::GOAL, std::bind(&TimeCount::GoalUpdate, this));

	draw_.emplace(STATE::GAME, std::bind(&TimeCount::GameDraw, this));
	draw_.emplace(STATE::GOAL, std::bind(&TimeCount::GoalDraw, this));
	draw_.emplace(STATE::SINGLE, std::bind(&TimeCount::SingleDraw, this));

	state_ = STATE::GAME;

}

TimeCount::~TimeCount()
{
}

void TimeCount::Update(float startime)
{
	startTime_ = startime;
	update_[state_]();
}

void TimeCount::Draw()
{
	draw_[state_]();
}

void TimeCount::GoalUpdate()
{
	Count();
}

void TimeCount::GoalDraw()
{
	DrawGraph(POSX_IMG_GOAL, POSY_IMG_GOAL, goalImg_, true);

	if (elapsedTime_>= record_ + LIMIT_TIME_DRAW_RECORD)
	{
		DrawFormatStringToHandle(
			POSX_FINAL_RECORD,
			POSY_FINAL_RECORD,
			0xff0000,
			finalRecordFontHandle_, "RECORD:%f", record_);
		endFlag_ = true;
	}
}

void TimeCount::GameUpdate()
{
	if (startTime_ >= LIMIT_TIME_START && (!startFlag_))
	{
		SetStart();
		startFlag_ = true;
	}
	Count();
	if (checkPoint_.IsGoal())
	{
		record_ = elapsedTime_;
		state_ = STATE::GOAL;
	}

}

void TimeCount::GameDraw()
{	
	//何もしない
}

void TimeCount::SingleDraw()
{
	DrawFormatStringToHandle(
		POSX_RECORD,
		POSY_RECORD,
		0xff0000,
		recordFontHandle_,
		"RECORD:%f",
		elapsedTime_);

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
	deltaTime_ = (nowTime_ - oldTime_) / DELTA_DIVIDE;
	oldTime_ = nowTime_;
	elapsedTime_ += deltaTime_;
}

void TimeCount::SetSinglePlayMode()
{
	state_ = STATE::SINGLE;
}
