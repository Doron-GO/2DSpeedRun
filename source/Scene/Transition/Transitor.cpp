#include "Transitor.h"

Transitor::~Transitor()
{
	DeleteGraph(oldRT_);
	DeleteGraph(newRT_);
}

void Transitor::Start()
{
	oldRT_ = MakeScreen(sizeX, sizeY);
	newRT_ = MakeScreen(sizeX, sizeY);
	int result = GetDrawScreenGraph(0, 0, sizeX, sizeY, oldRT_);
	frame_ = 0.0f;
}


bool Transitor::IsEnd() const
{
	return frame_ >= interval_;
}
