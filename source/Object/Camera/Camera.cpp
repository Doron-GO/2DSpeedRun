#include<algorithm>
#include "Camera.h"
#include"../../Object/Player/Player.h"

Camera::Camera(): _state (& Camera::Follow)
{
    float time = 0;
}

Camera::~Camera()
{
}


void Camera:: Update()
{
    (this->*_state)();
}

void Camera::Switching()
{
    Vector2DFloat view = { 1600.0f, 1000.0f };
    Vector2DFloat offset;
    offset.x = (view.x / 4.0f) * 2.0f - target_.lock()->GetPos().x;
    offset.y = (view.y / 2.0f) - (target_.lock()->GetPos().y);
    if (switchTime_ <= MAXFRAME){switchTime_++;}
    cameraPos_.x = oldPos_.x * (1.0f - switchTime_ / MAXFRAME) + offset.x * switchTime_ / MAXFRAME;
    cameraPos_.y = oldPos_.y * (1.0f - switchTime_ / MAXFRAME) + offset.y * switchTime_ / MAXFRAME;
    if (switchTime_ >= MAXFRAME)
    {
        _state = &Camera::Follow;
        switchTime_ = 0.0f;
    }
}

void Camera::StateChanging(int num)
{
    _state = &Camera::Switching;
}

void Camera::Follow()
{
    Vector2DFloat view = { 1600.0f, 1000.0f };
    Vector2DFloat offset;
    offset.x = (view.x / 4.0f) * 2.0f - target_.lock()->GetPos().x;
    offset.y = (view.y / 2.0f) - target_.lock()->GetPos().y;
    cameraPos_ = offset;
    oldPos_ = cameraPos_;
}

bool Camera::ReConnect(std::weak_ptr<Player> actor)
{
    target_ = actor;
    return true;
}


const Vector2DFloat& Camera::GetPos(void) const
{
    return cameraPos_; 
}

const Vector2DFloat& Camera::GetTargetPos(void) const
{
     return target_.lock()->GetPos(); 
}

