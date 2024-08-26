#include "DeltaTime.h"
#include<DxLib.h>

void DeltaTime::update()
{	
	nowTime_ = float(GetNowHiPerformanceCount());
	deltaTime_ = (nowTime_ - oldTime_) / 1000000.0f;
	oldTime_ = nowTime_;

	elapsedTime_ += deltaTime_;

}

const float DeltaTime::GetDeltaTime()
{
	return deltaTime_;
}

const float DeltaTime::GetElapsedTime()
{
	return elapsedTime_;
}

void DeltaTime::SetStart()
{
	oldTime_ = float(GetNowHiPerformanceCount());
}

void DeltaTime::Reset()
{
	oldTime_ = float(GetNowHiPerformanceCount());
	elapsedTime_ = 0.0f;

}

DeltaTime::DeltaTime() :oldTime_(0.0f), deltaTime_(0.000001f), elapsedTime_(0.0f), nowTime_(0.0f)
{

}

DeltaTime::~DeltaTime()
{
}
