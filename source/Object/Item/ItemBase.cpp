#include<DxLib.h>
#include "ItemBase.h"

ItemBase::ItemBase()
{
	activateFlag_ = false;
	explosionFlag_ = false;
}

ItemBase::~ItemBase()
{
}

void ItemBase::Update()
{
}

void ItemBase::Draw(Vector2DFloat offset)
{

}


void ItemBase::SetPos(Vector2DFloat pos)
{
	targetPos_ = pos;
}

void ItemBase::Activate(Vector2DFloat playerpos)
{
	itemPos_ = playerpos;
}

bool ItemBase::IsEnd()
{
	//if (activateFlag_)
	//{
	//	return true;
	//	activateFlag_ = false;
	//}
	return false;
}

void ItemBase::End()
{
	activateFlag_ = false;
}

bool ItemBase::IsActivate()
{
	return activateFlag_;
}

bool ItemBase::IsExplosion()
{
	return explosionFlag_;
}

const Vector2DFloat ItemBase::GetPos()
{
	return itemPos_;
}


