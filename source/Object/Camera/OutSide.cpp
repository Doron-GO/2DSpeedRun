#include<DxLib.h>
#include "OutSide.h"
#include"Camera.h"
#include"../../Object/Player/Player.h"
#include"DangerZoneSmaller.h"

const std::string EXP_IMG_EXPLOSION_PASS = "Src/Img/Explosion.png";
const std::string EXP_IMG_BIG_EXPLOSION_PASS = "Src/Img/BigExplosion.png";
const std::string EXP_SOUND_EXPLOSION_PASS = "Src/Sound/Explosion.mp3";


Vector2DFloat screenSize ={1600.0f,1000.0f};

OutSide::OutSide(Camera& camera, int playerCount) :camera_(camera), playerCount_(playerCount)
{
	LoadDivGraph(EXP_IMG_EXPLOSION_PASS.c_str(), 11, 11, 1, 32, 32, bombImg_);
	LoadDivGraph(EXP_IMG_BIG_EXPLOSION_PASS.c_str(), 8, 8, 1, 32, 32, bigBombImg_);
	ExplosionSound_ = LoadSoundMem(EXP_SOUND_EXPLOSION_PASS.c_str());
	outSideMax_ = OUTSIDE_SCALE;
	outSideMin_ = -OUTSIDE_SCALE;
	dangerZone_ = std::make_unique<DangerZoneSmaller>(outSideMax_, outSideMin_);
	minPos_ = { 0.0f,0.0f };
	maxPos_ = { 1600.0f, 1000.0f };
	expCount_ = 0;
	switchingTime_ = 0;
	singlePlayFlag_ = false;
	bigExploding_ = false;
	isExploding_ = false;
	conclusion_ = false;
	_update = &OutSide::FollowStateUpdate;

}

OutSide::~OutSide()
{

}

void OutSide::Update(std::vector<std::shared_ptr<Player>> players)
{
	(this->*_update)();
	auto &camera = camera_.GetPos();

	minPos_ = outsidePos_ + -OUTSIDE_SCALE;
	maxPos_ = outsidePos_ + OUTSIDE_SCALE;

	if (!singlePlayFlag_)
	{
		dangerZone_->Update();
		IsDead(players);
	}
	if (isExploding_)
	{
		MoveChainExplosion();
		ClearExplosion();
	}
	if (bigExploding_)
	{
		SideChange(lowerPos_, lowerSide_);
	}
	dangerZone_->Smaller();
}


void OutSide::MoveChainExplosion(void)
{
	if (lowerPos_.y <= minPos_.y && lowerPos_.x >= maxPos_.x)
	{
		lowerVec_ = { -20.0f ,0.0f };
		lowerSide_ = SIDE::UP;
	}
	//画面右下なら上に行く
	if (lowerPos_.x >= maxPos_.x && lowerPos_.y >= maxPos_.y)
	{
		lowerVec_ = { 0.0f,-20.0f };
		lowerSide_ = SIDE::RIGHT;
	}
	//画面左下なら右に行く
	if (lowerPos_.y >= maxPos_.y && lowerPos_.x <= minPos_.x)
	{
		lowerVec_ = { 20.0f ,0.0f };
		lowerSide_ = SIDE::DOWN;
	}
	//画面左上なら下に行く
	if (lowerPos_.x <= minPos_.x && lowerPos_.y <= minPos_.y)
	{
		lowerVec_ = { 0.0f ,20.0f };
		lowerSide_ = SIDE::LEFT;
	}
	//画面左上になったら右に行く
	if (upperPos_.y <= minPos_.y && upperPos_.x <= minPos_.x)
	{
		upperVec_ = { 20.0f,0.0f };
		upperSide_ = SIDE::UP;
	}
	//画面右上になったら下に行く
	if (upperPos_.x >= maxPos_.x && upperPos_.y <= minPos_.y)
	{
		upperVec_ = { 0.0f,20.0f };
		upperSide_ = SIDE::RIGHT;
	}
	//画面右下になったら左に行く
	if (upperPos_.y >= maxPos_.y && upperPos_.x >= maxPos_.x)
	{
		upperVec_ = { -20.0f ,0.0f };
		upperSide_ = SIDE::DOWN;
	}
	//画面左下に行ったら上に行く
	if (upperPos_.x <= minPos_.x && upperPos_.y >= maxPos_.y)
	{
		upperVec_ = { 0.0f ,-20.0f };
		upperSide_ = SIDE::LEFT;
	}

	lowerPos_ += lowerVec_;
	upperPos_ += upperVec_;

	//一定時間ごとに爆発させる、あと音も出す
	if ((count_++) % 3 == 0)
	{
		upBombs_.emplace_back(upperPos_, upperSide_);
		downBombs_.emplace_back(lowerPos_, lowerSide_);
		PlaySoundMem(ExplosionSound_, DX_PLAYTYPE_BACK, true);
	}
	SideChange(upperPos_, upperSide_);
	SideChange(lowerPos_, lowerSide_);

}

void OutSide::ClearExplosion(void)
{
	//上からの爆弾と下からの爆弾が重なったらどっちも消す
	if (lowerPos_.distance(upperPos_) < 40)
	{
		PlaySoundMem(ExplosionSound_, DX_PLAYTYPE_BACK, true);
		StartJoypadVibration(padNum_, 1000, 400);
		upBombs_.clear();
		isExploding_ = false;
		bigExploding_ = true;
		bigExpCount_ = 0;
	}
	auto bombsCheck = [this](std::list<Bomb>& bomb)
	{
		for (auto& b : bomb)
		{
			b.frame_++;
			SideChange(b.pos_, b.side_);

			if (b.frame_ > 29)
			{
				b.isDead = true;
			}
		}
	};

	bombsCheck(upBombs_);
	bombsCheck(downBombs_);

	upBombs_.remove_if([](const Bomb& b) {
		return b.isDead;
		});
	downBombs_.remove_if([](const Bomb& b) {
		return b.isDead;
		});

}

void OutSide::ExplosionDraw(void)
{
	Vector2DFloat camera = camera_.GetPos();

	if (isExploding_)
	{

		if (expCount_ > EXPLOSION_IMG_NUM - 1)
		{
			expCount_ = 0;
		}
		DrawRotaGraph2F(upperPos_.x + camera.x, upperPos_.y + camera.y,
			16.0f, 16.0f,
			5.0, 0.0,
			bigBombImg_[expCount_],
			true, 0, 0);

		DrawRotaGraph2F(lowerPos_.x + camera.x, lowerPos_.y + camera.y,
			16.0f, 16.0f,
			5.0, 0.0,
			bigBombImg_[expCount_],
			true, 0, 0);
		expCount_++;
		auto bombsDraw = [](std::list<Bomb>& bomb, Vector2DFloat camera, int image[11])
		{
			for (auto& b : bomb)
			{

				DrawRotaGraph2F(b.pos_.x + camera.x, b.pos_.y + camera.y,
					18.0, 16.0f,
					4.5, 0.0,
					image[b.frame_ % 10],
					true, 0, 0);
			}
		};
		bombsDraw(upBombs_, camera, bombImg_);
		bombsDraw(downBombs_, camera, bombImg_);
	}
}

void OutSide::BigExplosionDraw(void)
{
	if (bigExploding_)
	{
		DrawRotaGraph2F(lowerPos_.x + camera_.GetPos().x, lowerPos_.y + camera_.GetPos().y,
			16.0f, 16.0f,
			9.5, 0.0,
			bigBombImg_[bigExpCount_ / 4],
			true, 0, 0);
		bigExpCount_++;
	}
	if (bigExpCount_ > 100)
	{
		bigExploding_ = false;
		upperPos_ = { 0.0f,0.0f };
		lowerPos_ = { 0.0f,0.0f };
		bigExpCount_ = 0;
	}
}

void OutSide::OutSideDraw(void)
{
	Vector2DFloat camera = camera_.GetPos();
	Vector2DFloat min = (minPos_ + camera);
	Vector2DFloat max = (maxPos_ + camera);

	DrawBoxAA(0.0f, 0.0f, screenSize.x, min.y, 0x000000, true);
	DrawBoxAA(0.0f, screenSize.y, screenSize.x, max.y, 0x000000, true);
	DrawBoxAA(0.0f, 0.0f, min.x, screenSize.y, 0x000000, true);
	DrawBoxAA(screenSize.x, 0.0f, max.x, screenSize.y, 0x000000, true);
}

void OutSide::StateChanging()
{
	_update = &OutSide::SwitchingStateUpdate;
}

void OutSide::FollowStateUpdate()
{
	outsidePos_= camera_.GetTargetPos();
	outsideOldPos_ =outsidePos_;
}

void OutSide::SwitchingStateUpdate()
{
	if (switchingTime_ <= 60.0f) { switchingTime_++; }
	outsidePos_.x = outsideOldPos_.x * (1.0f - switchingTime_ / 60.0f)+ camera_.GetTargetPos().x *  switchingTime_ / 60.0f;
	outsidePos_.y = outsideOldPos_.y * (1.0f - switchingTime_ / 60.0f) + camera_.GetTargetPos().y *  switchingTime_ / 60.0f;
	if (switchingTime_ >= 60.0f)
	{
		_update = &OutSide::FollowStateUpdate;
		switchingTime_ = 0.0f;
	}
}

void OutSide::Draw(Vector2DFloat offset)
{
	ExplosionDraw();
	BigExplosionDraw();
	OutSideDraw();
}

void OutSide::IsDead(std::vector< std::shared_ptr<Player >> players)
{
 	for (const auto& player : players)
	{
		if (!player->IsAlive())
		{
			return;
		}
		if (!IsOutSide(player->GetPos()))
		{
			UpDownORLeftRight(player->GetPos());
			player->Dead();
			padNum_ = player->GetPadNum();
			isExploding_ = true;
			count_ = 0;
			dangerZone_->ResetCounter();
			playerCount_--;
			StartJoypadVibration(padNum_, 400, 300);
		}
	}
	if (playerCount_<=1) {
		conclusion_ = true;
	}
}

bool OutSide::IsOutSide(Vector2DFloat pos)
{
	return (minPos_.x < pos.x && pos.y> minPos_.y&&
		maxPos_.x>pos.x && maxPos_.y>pos.y);
}

void OutSide::SinglePlay()
{
	singlePlayFlag_=true;
}

void OutSide::UpDownORLeftRight(Vector2DFloat pos)
{
	if (pos.y <= minPos_.y || maxPos_.y <= pos.y)
	{
		UpORDown(pos);
	}
	if (minPos_.x >= pos.x || maxPos_.x<=pos.x)
	{
		LeftOrRight(pos);
	}
}

void OutSide::UpORDown(Vector2DFloat pos)
{
	Vector2DFloat up;
	Vector2DFloat low;
	Vector2DFloat camera{ camera_.GetPos().x,camera_.GetPos().y };

	if (pos.y <=minPos_.y )
	{
		up = { pos.x  ,minPos_.y };
		upperVec_ = { 30.0f,0.0f };
		low = { pos.x ,minPos_.y };
		lowerVec_ = { -30.0f ,0.0f };
		upperSide_ = SIDE::UP;
		lowerSide_ = SIDE::UP;
	}
	else if(maxPos_.y <= pos.y)
	{
		up = { pos.x  , maxPos_.y };
		upperVec_ = { -30.0f,0.0f };
		low = { pos.x, maxPos_.y };
		lowerVec_ = { 30.0f ,0.0f };
		upperSide_ = SIDE::DOWN;
		lowerSide_ = SIDE::DOWN;
	}
	else
	{

	}
	upperPos_ = up;
	lowerPos_ = low;
}

void OutSide::LeftOrRight(Vector2DFloat pos)
{
	Vector2DFloat up;
	Vector2DFloat low;

	if (pos.x >= maxPos_.x)
	{
		up = { maxPos_.x ,pos.y  };
		upperVec_ = { 0.0f,20.0f };
		low = { maxPos_.x, pos.y };
		lowerVec_ = { 0.0f ,-20.0f };
		upperSide_ = SIDE::RIGHT;
		lowerSide_ = SIDE::RIGHT;

	}
	else if (minPos_.x >= pos.x)
	{
		up = { minPos_.x ,pos.y };
		upperVec_ = { 0.0f,-20.0f };
		low = { minPos_.x, pos.y };
		lowerVec_ = { 0.0f ,20.0f };
		upperSide_ = SIDE::LEFT;
		lowerSide_ = SIDE::LEFT;

	}
	else
	{

	}
	upperPos_ = up;
	lowerPos_ = low ;
}


//const int OutSide::NumberOfSurvivors()
//{
//	return playerCount_;
//}

void OutSide::SideChange(Vector2DFloat& pos, SIDE side_)
{
	switch (side_)
	{
	case SIDE::UP:
		pos.y = minPos_.y;
		break;
	case SIDE::DOWN:
		pos.y = maxPos_.y;
		break;
	case SIDE::LEFT:
		pos.x = minPos_.x;
		break;
	case SIDE::RIGHT:
		pos.x = maxPos_.x;
		break;
	}
}

