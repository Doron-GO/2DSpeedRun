#include "DangerZoneSmaller.h"

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
	auto count = (count_ / 10);
	if (count>= SHRINK_START_TIME)
	{
		Activated();
	}
	else{count_++;}
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
