#include<DxLib.h>
#include "Wire.h"
#include"Player.h"

Wire::Wire(Player& player, ColList& list) :player_(player), col(list)
{
	 _state = &Wire::StandbyState;
	 pow_=0.0f;
	 Scale_ = { 0.0f,0.0f };
	 vel_ = { 0.0f,0.0f };
	 v_ = 0.0f;
	 angle_ = 0.0f;
	 fulcrum_pos = { 0.0f,0.0f };
	 length_ = 0.0f;

}

Wire::~Wire()
{
}

void Wire::Update()
{
	(this->*_state)();
	Pump();
}

void Wire::Draw(Vector2DFloat cameraPos)
{
	Vector2DFloat pPos = player_.pos_;
	pPos += cameraPos;
	if (_state== &Wire::SwingState|| _state == &Wire::AnchoringState)
	{
		int X = static_cast<int>(fulcrum_.x + cameraPos.x);
		int Y = static_cast<int>(fulcrum_.y + cameraPos.y);
		DrawCircle(X,Y,5,0xff0000);
		int PX = static_cast<int>(pPos.x);
		int PY = static_cast<int>(pPos.y);
		DrawLine(PX, PY, X, Y, 0xffffff);
	}
}


void Wire::SwingState()
{
	float gravity = 0.5f;
	//アングルをけってい
	angle_ = atan2f(player_.pos_.x - fulcrum_.x, player_.pos_.y - fulcrum_.y);
	v_ += gravity * sinf(angle_);
	vel_ = { -v_ * cosf(angle_),v_ * sinf(angle_) };
	Vector2DFloat vel = { vel_.x,vel_.y };
	if (_state == &Wire::SwingState)
	{
		player_.pos_ = fulcrum_ + (player_.pos_ - fulcrum_).Normalized() * length_;//長さを補正
		player_.pos_ += vel;	//velを加算
	}
	if (player_.pos_.y <= fulcrum_.y + -150.0f)
	{
		SwingJump();
		player_.StartSwingJump();
	}
}

void Wire::StandbyState()
{
	fulcrum_ = player_.pos_;
}

void Wire::SwingJump()
{
	player_.movePow_.x = (vel_.x / 1.3f);
	player_.movePow_.y = (vel_.y / 1.3f);
	_state = &Wire::EndSwingState;
}

void Wire::ChangeStandby()
{
	_state = &Wire::StandbyState;
}

void Wire::EndSwingState()
{
	//何もさせない
}

void Wire:: AnchoringState()
{	
	fulcrum_pos = VAdd(fulcrum_pos, Scale_);
	fulcrum_.x = fulcrum_pos.x;
	fulcrum_.y = fulcrum_pos.y;
	if (!IsHitHook())
	{
		SetSwingPalam();
	}
}

void Wire::SetSwingPalam()
{	
	auto lVec = player_.pos_ - fulcrum_;//支点→錘のベクトル(紐)
	length_ = lVec.Magnitude();							//紐の長さ
	vel_.x = player_.movePow_.x;	//初速度的な
	//ここでアングルの初期設定をする
	angle_ = atan2f(player_.pos_.x - fulcrum_.x, player_.pos_.y - fulcrum_.y);
	v_ = -2 * vel_.x * cosf(angle_);//x軸の速度

	if (player_.dir_LR_ == Player::DIR_LR::LEFT)
	{
		pow_ = 0.15f;
	}
	else
	{
		pow_ = -0.15f;
	}
	_state = &Wire::SwingState;
	player_.StartSwing();
}

void Wire::SetAnchorPalam()
{
	fulcrum_ = player_.pos_;
	VECTOR moveVec = { player_.GetDiagonallyVecVec().x,
		(-35.0f)+17.0f};
	moveVec_.x = moveVec.x/2.0f;
	moveVec_.y = moveVec.y/2.0f;
	moveVec = VNorm(moveVec);
	Scale_ = VScale(moveVec, 30.0f);
	fulcrum_pos.x = fulcrum_.x;
	fulcrum_pos.y = fulcrum_.y;	
	_state = &Wire::AnchoringState;
}

void Wire::StartSwingJump()
{
	player_.StartSwingJump();
	_state = &Wire::EndSwingState;
}

void Wire::StartSwing()
{
	_state = &Wire::SwingState;
}

void Wire::EndSwing()
{
	_state = &Wire::EndSwingState;
}

void Wire::Pump()
{
	if (_state==&Wire::SwingState)
	{
		v_ += pow_;
	}
}

bool Wire::IsHitHook()
{
	for (const auto& col : col)
	{	
		if (rayCast_.CheckCollision( col, fulcrum_))
		{		
			return false;
		}
	}
	return true;
}
