#include "StripTransitor.h"

StripTransitor::StripTransitor(int width, int interval)
{
}

void StripTransitor::Update()
{
	if (frame_ < interval_) {
		++frame_;
		SetDrawScreen(newRT_);
	}
	else if (frame_ == interval_) {
		SetDrawScreen(DX_SCREEN_BACK);
	}
}

void StripTransitor::Draw()
{
	if (IsEnd()) {
		return;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	VECTOR wsize = { 1600,1000 };
	int rate = static_cast<int>(frame_ / interval_);
	int lp = static_cast<int>( (wsize.x / width_) + 1 );
	int X = static_cast<int>(wsize.x);
	int Y = static_cast<int>(wsize.y);
	DrawRectGraph(0, 0, 0, 0, X,Y, oldRT_, true);
	for (int i = 0; i < lp; ++i) {
		if (i % 2 == 0) {
			DrawRectGraph(i * width_, Y * (rate - 1), i * width_, 0, width_, Y, newRT_, true);
		}
		else {
			DrawRectGraph(i * width_, Y * (1 - rate), i * width_, 0, width_, Y, newRT_, true);
		}
	}
}
