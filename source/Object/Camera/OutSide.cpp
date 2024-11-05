#include<DxLib.h>
#include "OutSide.h"
#include"Camera.h"
#include"../../Object/Player/Player.h"
#include"DangerZoneSmaller.h"

//爆発画像
const std::string EXP_IMG_EXPLOSION_PASS = "Src/Img/Explosion.png";
//大爆発画像
const std::string EXP_IMG_BIG_EXPLOSION_PASS = "Src/Img/BigExplosion.png";
//爆発音のパス
const std::string EXP_SOUND_EXPLOSION_PASS = "Src/Sound/Explosion.mp3";
//画面サイズ
Vector2DFloat screenSize ={1600.0f,1000.0f};

//爆発画像読み込み情報
const int IMG_EXP_X_NUM = 11;
const int IMG_EXP_Y_NUM = 1;
const VECTOR IMG_EXP_SIZE = { 32.0f,32.0f };

//大爆発画像読み込み情報
const int IMG_BIGEXP_X_NUM = 8;
const int IMG_BIGEXP_Y_NUM = 1;
const VECTOR IMG_BIGEXP_SIZE = { 32.0f,32.0f };

//死亡判定の四角の大きさ
const Vector2DFloat OUTSIDE_SCALE = { 800.0f,500.0f };

//追尾対象切り替えにかかる時間
const float MAX_SWITCHING_COUNT = 60.0f;

//左に向かって走る
const Vector2DFloat GO_TO_LEFT = { -20.0f ,0.0f };
//右に向かって走る
const Vector2DFloat GO_TO_RIGHT = { 20.0f,0.0f };

//上に向かって走る
const Vector2DFloat GO_TO_UP = { 0.0f,-20.0f };


OutSide::OutSide(Camera& camera, int playerCount) :camera_(camera), playerCount_(playerCount)
{
	LoadDivGraph(EXP_IMG_EXPLOSION_PASS.c_str(),
		IMG_EXP_ALL_NUM,
		IMG_EXP_X_NUM,
		IMG_EXP_Y_NUM,
		static_cast<int>(IMG_EXP_SIZE.x),
		static_cast<int>(IMG_EXP_SIZE.y),
		bombImg_);

	LoadDivGraph(EXP_IMG_BIG_EXPLOSION_PASS.c_str(),
		IMG_BIGEXP_ALL_NUM,
		IMG_BIGEXP_X_NUM,
		IMG_BIGEXP_Y_NUM,
		static_cast<int>(IMG_BIGEXP_SIZE.x),
		static_cast<int>(IMG_BIGEXP_SIZE.y),
		bigBombImg_);

	ExplosionSound_ = LoadSoundMem(EXP_SOUND_EXPLOSION_PASS.c_str());

	//死亡判定の四角の左下座標
	outSideMax_ = OUTSIDE_SCALE;
	//死亡判定の四角の左上座標
	outSideMin_ = -OUTSIDE_SCALE;

	dangerZoneSmaller_ = std::make_unique<DangerZoneSmaller>(outSideMax_, outSideMin_);

	//左上座標は0で設定
	minPos_ = { 0.0f,0.0f };
	//右下座標はスクリーンサイズで設定
	maxPos_ = screenSize;

	//爆発画像カウントの初期化
	expCount_ = 0;
	//追尾対象変更
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
		dangerZoneSmaller_->Update();
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
	dangerZoneSmaller_->Smaller();
}


void OutSide::MoveChainExplosion(void)
{
	if (lowerPos_.y <= minPos_.y && lowerPos_.x >= maxPos_.x)
	{
		lowerVec_ = GO_TO_RIGHT;
		lowerSide_ = SIDE::UP;
	}
	//画面右下なら上に行く
	if (lowerPos_.x >= maxPos_.x && lowerPos_.y >= maxPos_.y)
	{

		lowerVec_ = GO_TO_UP;
		lowerSide_ = SIDE::RIGHT;
	}
	//画面左下なら右に行く
	if (lowerPos_.y >= maxPos_.y && lowerPos_.x <= minPos_.x)
	{

		//左に向かって走る
		lowerVec_ = GO_TO_RIGHT;
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
	if ((expSoundCount_++) % 3 == 0)
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

		if (expCount_ > IMG_EXP_ALL_NUM - 1)
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
	//カメラ座標
	const Vector2DFloat& cameraPos = camera_.GetTargetPos();

	//移行にかかる時間
	float SWITCHING_TIME = switchingTime_ / 60.0f;
	//1秒かけて0にする
	float SWITCHING_RATE = (1.0f - switchingTime_ / 60.0f);

	outsidePos_.x = 
		outsideOldPos_.x * SWITCHING_RATE +
		cameraPos.x * SWITCHING_TIME;

	outsidePos_.y =
		outsideOldPos_.y * SWITCHING_RATE +
		cameraPos.y * SWITCHING_TIME;

	//移行時間カウントが60を超えたらステートを変更して、
	//switchingTime_をリセットする
	if (switchingTime_ > MAX_SWITCHING_COUNT)
	{
		_update = &OutSide::FollowStateUpdate;
		switchingTime_ = 0.0f;
	}
	else
	{
		switchingTime_++;
	}
}

void OutSide::Draw(Vector2DFloat offset)
{
	//爆発描画
	ExplosionDraw();
	//大爆発描画
	BigExplosionDraw();
	//外枠描画
	OutSideDraw();
}

void OutSide::IsDead(std::vector< std::shared_ptr<Player >> players)
{
 	for (const auto& player : players)
	{
		//プレイヤーがすでに脱落していたら抜ける
		if (!player->IsAlive())
		{
			continue;
		}
		//プレイヤーが枠の外に出たら死亡状態にする
		if (!IsOutSide(player->GetPos()))
		{
			//爆発演出開始座標を設定
			UpDownORLeftRight(player->GetPos());
			//爆発フラグをtrue
			isExploding_ = true;
			//プレイヤーを志望状態に変更
			player->Dead();
			//死亡したプレイヤーのコントローラ番号を特定
			padNum_ = player->padNum_;
			//コントローラを振動させる
			StartJoypadVibration(padNum_, 400, 300);
			//爆発音再生カウントをリセット
			expSoundCount_ = 0;
			//画面縮小開始カウントをリセット
			dangerZoneSmaller_->ResetCounter();
			//残りプレイヤー数カウントを減らす
			playerCount_--;
		}
	}
	//残りプレイヤー数が一人になったら対戦終了にする
	if (playerCount_ <2) 
	{
		conclusion_ = true;
	}
}

bool OutSide::IsOutSide(Vector2DFloat pos)
{
	return (minPos_.x < pos.x && pos.y> minPos_.y&&
		maxPos_.x>pos.x && maxPos_.y>pos.y);
}

void OutSide::SetSinglePlayMode()
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

