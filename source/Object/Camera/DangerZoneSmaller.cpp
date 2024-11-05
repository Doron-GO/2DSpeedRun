#include "DangerZoneSmaller.h"

//k¬ŠJŽnŽžŠÔ
constexpr int SHRINK_START_TIME = 100;

//Žûk‚·‚é‚Æ‚«‚ÌÅ¬ƒTƒCƒY
const Vector2DFloat MAX_SHRINK_SIZE = { 200.0f,150.0f };

//k¬—Ê
const Vector2DFloat SCALE_STEP = { 0.2f, 0.13f };

const int COUNT_RATE = 10;


DangerZoneSmaller::DangerZoneSmaller(Vector2DFloat& max, Vector2DFloat& min):outSideMax_(max),outSideMin_(min)
{
	_update = &DangerZoneSmaller::UpdateWait;
	count_ = 0;
}

DangerZoneSmaller::~DangerZoneSmaller()
{
}

void DangerZoneSmaller::Smaller()
{
	if (outSideMax_ >= MAX_SHRINK_SIZE && outSideMin_<= -MAX_SHRINK_SIZE)
	{
		outSideMax_-= SCALE_STEP;
		outSideMin_ += SCALE_STEP;
	}
}

void DangerZoneSmaller::Update()
{
	(this->*_update)();
}

void DangerZoneSmaller::UpdateWait()
{
	int count = (count_ / COUNT_RATE);

	if (count<SHRINK_START_TIME)
	{
		count_++;
	}
	else
	{
		Activated();
	}
}
void DangerZoneSmaller::Activated()
{
	_update = &DangerZoneSmaller::UpdateSmaller;
}
void DangerZoneSmaller::ResetCounter()
{
	count_ = 0;
}
void DangerZoneSmaller::UpdateSmaller()
{
	Smaller();
}
