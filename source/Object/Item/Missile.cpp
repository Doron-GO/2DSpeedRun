#include<DxLib.h>
#include<string>
#include "Missile.h"

const std::string PATH_IMG_ITEM_MISSILE = PATH_IMG+ "Missile.png";
const std::string PATH_IMG_EXPLOSION= PATH_IMG + "Explosion.png";

Missile::Missile()
{
	count_ = 0;
	drawCount_ = 0;
	_update = &Missile::WaitUpdate;
	_draw = &Missile::MissileDraw;
	type_ = ITEM_TYPE::MISSILE;
	LoadDivGraph(PATH_IMG_EXPLOSION.c_str(), 11, 11, 1, 32, 31, explosionImg_);
	
	missileImg_ = LoadGraph(PATH_IMG_ITEM_MISSILE.c_str());
	explosionFlag_ = false;
	activateFlag_ = false;
}

Missile::~Missile()
{
	DeleteGraph(missileImg_);
	for (int num = 0; num < EXPLOSION_IMG_NUM; num++)
	{
		DeleteGraph(explosionImg_[num]);
	}

}

void Missile::Update()
{
	(this->*_update)();
	col_.min_ = { itemPos_.x - MISSIL_COLISION_OFFSET,itemPos_.y + MISSIL_COLISION_OFFSET };
	col_.max_ = { itemPos_.x + MISSIL_COLISION_OFFSET,itemPos_.y - MISSIL_COLISION_OFFSET };
}


void Missile::Draw(Vector2DFloat offset)
{
	(this->*_draw)(offset);
}

void Missile::Activate(Vector2DFloat playerpos)
{
	_update = &Missile::ActivateUpdate;
	_draw = &Missile::MissileDraw;
	itemPos_ = playerpos;
	activateFlag_ = true;
}

bool Missile::IsEnd()
{
	if (_update == &Missile::ActivateUpdate &&!activateFlag_ )
	{
		_draw = &Missile::ExplosionDraw;
		explosionFlag_ = true;
		return true;
	}
	return false;
}

void Missile::ActivateUpdate()
{		
	vel_ = (vel_ + (targetPos_ - itemPos_).Normalized()).Normalized() * 12.0f;
	if (count_++ > 50)
	{	
		IsCollision();
	}
	if (activateFlag_) {itemPos_ += vel_;}
}

void Missile::WaitUpdate()
{

}

void Missile::IsCollision()
{
	for (const auto& colG : grndColList_)
	{
		Collision col = { Vector2DFloat{colG.first.x,colG.first.y + colG.second.y},
			Vector2DFloat{colG.first.x + colG.second.x,colG.first.y}};
		if(raycast_.RectToRectCollision(col_.min_, col_.max_,col.first, col.second ))
		{
			activateFlag_ = false;
			_draw = &Missile::ExplosionDraw;

		}
	}
}

void Missile::ExplosionDraw(Vector2DFloat offset)
{
	DrawRotaGraph2F(itemPos_.x + offset.x, itemPos_.y + offset.y,
		20.0f, 20.0f,
		2.5, 0.0,
		explosionImg_[(drawCount_ / 3) % 11],
		true);
	if (drawCount_++ >= 29)
	{	
		drawCount_ =0;
		explosionFlag_ = false;
	}
}

void Missile::MissileDraw(Vector2DFloat offset)
{
	float deg = atan2f(vel_.y, vel_.x);
	auto rad = 90 * (DX_PI_F / 180.0f);
	auto angle = deg + rad;
	DrawRotaGraph2F(itemPos_.x + offset.x, itemPos_.y + offset.y, 4, 10, 3.0, angle, missileImg_, true);

}


