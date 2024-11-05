#include<DxLib.h>
#include "OutSide.h"
#include"Camera.h"
#include"../../Object/Player/Player.h"
#include"DangerZoneSmaller.h"

//�����摜
const std::string EXP_IMG_EXPLOSION_PASS = "Src/Img/Explosion.png";
//�唚���摜
const std::string EXP_IMG_BIG_EXPLOSION_PASS = "Src/Img/BigExplosion.png";
//�������̃p�X
const std::string EXP_SOUND_EXPLOSION_PASS = "Src/Sound/Explosion.mp3";
//��ʃT�C�Y
Vector2DFloat screenSize ={1600.0f,1000.0f};

//�����摜�ǂݍ��ݏ��
const int IMG_EXP_X_NUM = 11;
const int IMG_EXP_Y_NUM = 1;
const VECTOR IMG_EXP_SIZE = { 32.0f,32.0f };

//�唚���摜�ǂݍ��ݏ��
const int IMG_BIGEXP_X_NUM = 8;
const int IMG_BIGEXP_Y_NUM = 1;
const VECTOR IMG_BIGEXP_SIZE = { 32.0f,32.0f };

//���S����̎l�p�̑傫��
const Vector2DFloat OUTSIDE_SCALE = { 800.0f,500.0f };

//�ǔ��Ώې؂�ւ��ɂ����鎞��
const float MAX_SWITCHING_COUNT = 60.0f;

//���Ɍ������đ���
const Vector2DFloat GO_TO_LEFT = { -20.0f ,0.0f };
//�E�Ɍ������đ���
const Vector2DFloat GO_TO_RIGHT = { 20.0f,0.0f };

//��Ɍ������đ���
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

	//���S����̎l�p�̍������W
	outSideMax_ = OUTSIDE_SCALE;
	//���S����̎l�p�̍�����W
	outSideMin_ = -OUTSIDE_SCALE;

	dangerZoneSmaller_ = std::make_unique<DangerZoneSmaller>(outSideMax_, outSideMin_);

	//������W��0�Őݒ�
	minPos_ = { 0.0f,0.0f };
	//�E�����W�̓X�N���[���T�C�Y�Őݒ�
	maxPos_ = screenSize;

	//�����摜�J�E���g�̏�����
	expCount_ = 0;
	//�ǔ��ΏەύX
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
	//��ʉE���Ȃ��ɍs��
	if (lowerPos_.x >= maxPos_.x && lowerPos_.y >= maxPos_.y)
	{

		lowerVec_ = GO_TO_UP;
		lowerSide_ = SIDE::RIGHT;
	}
	//��ʍ����Ȃ�E�ɍs��
	if (lowerPos_.y >= maxPos_.y && lowerPos_.x <= minPos_.x)
	{

		//���Ɍ������đ���
		lowerVec_ = GO_TO_RIGHT;
		lowerSide_ = SIDE::DOWN;
	}
	//��ʍ���Ȃ牺�ɍs��
	if (lowerPos_.x <= minPos_.x && lowerPos_.y <= minPos_.y)
	{
		lowerVec_ = { 0.0f ,20.0f };
		lowerSide_ = SIDE::LEFT;
	}
	//��ʍ���ɂȂ�����E�ɍs��
	if (upperPos_.y <= minPos_.y && upperPos_.x <= minPos_.x)
	{
		upperVec_ = { 20.0f,0.0f };
		upperSide_ = SIDE::UP;
	}
	//��ʉE��ɂȂ����牺�ɍs��
	if (upperPos_.x >= maxPos_.x && upperPos_.y <= minPos_.y)
	{
		upperVec_ = { 0.0f,20.0f };
		upperSide_ = SIDE::RIGHT;
	}
	//��ʉE���ɂȂ����獶�ɍs��
	if (upperPos_.y >= maxPos_.y && upperPos_.x >= maxPos_.x)
	{
		upperVec_ = { -20.0f ,0.0f };
		upperSide_ = SIDE::DOWN;
	}
	//��ʍ����ɍs�������ɍs��
	if (upperPos_.x <= minPos_.x && upperPos_.y >= maxPos_.y)
	{
		upperVec_ = { 0.0f ,-20.0f };
		upperSide_ = SIDE::LEFT;
	}

	lowerPos_ += lowerVec_;
	upperPos_ += upperVec_;

	//��莞�Ԃ��Ƃɔ���������A���Ɖ����o��
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
	//�ォ��̔��e�Ɖ�����̔��e���d�Ȃ�����ǂ���������
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
	//�J�������W
	const Vector2DFloat& cameraPos = camera_.GetTargetPos();

	//�ڍs�ɂ����鎞��
	float SWITCHING_TIME = switchingTime_ / 60.0f;
	//1�b������0�ɂ���
	float SWITCHING_RATE = (1.0f - switchingTime_ / 60.0f);

	outsidePos_.x = 
		outsideOldPos_.x * SWITCHING_RATE +
		cameraPos.x * SWITCHING_TIME;

	outsidePos_.y =
		outsideOldPos_.y * SWITCHING_RATE +
		cameraPos.y * SWITCHING_TIME;

	//�ڍs���ԃJ�E���g��60�𒴂�����X�e�[�g��ύX���āA
	//switchingTime_�����Z�b�g����
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
	//�����`��
	ExplosionDraw();
	//�唚���`��
	BigExplosionDraw();
	//�O�g�`��
	OutSideDraw();
}

void OutSide::IsDead(std::vector< std::shared_ptr<Player >> players)
{
 	for (const auto& player : players)
	{
		//�v���C���[�����łɒE�����Ă����甲����
		if (!player->IsAlive())
		{
			continue;
		}
		//�v���C���[���g�̊O�ɏo���玀�S��Ԃɂ���
		if (!IsOutSide(player->GetPos()))
		{
			//�������o�J�n���W��ݒ�
			UpDownORLeftRight(player->GetPos());
			//�����t���O��true
			isExploding_ = true;
			//�v���C���[���u�]��ԂɕύX
			player->Dead();
			//���S�����v���C���[�̃R���g���[���ԍ������
			padNum_ = player->padNum_;
			//�R���g���[����U��������
			StartJoypadVibration(padNum_, 400, 300);
			//�������Đ��J�E���g�����Z�b�g
			expSoundCount_ = 0;
			//��ʏk���J�n�J�E���g�����Z�b�g
			dangerZoneSmaller_->ResetCounter();
			//�c��v���C���[���J�E���g�����炷
			playerCount_--;
		}
	}
	//�c��v���C���[������l�ɂȂ�����ΐ�I���ɂ���
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

