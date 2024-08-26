#pragma once
#include<memory>
#include<Dxlib.h>
#include<vector>
#include"../../Common/Vector2D.h"

class Player;

static constexpr float MAXFRAME =60.0f;

class Camera
{

public:

	Camera();
	~Camera();
	void Update();

	//カメラの状態を変える
	void StateChanging(int num);
	//追従対象変更する
	bool ReConnect(std::weak_ptr<Player> actor);

	//カメラ座標
	const Vector2DFloat& GetPos(void)const;

	//追従対象座標
	const Vector2DFloat& GetTargetPos(void)const;

private:

	//追従対象変更の時間
	float switchTime_ ;

	//追従対象
	std::weak_ptr<Player> target_;

	//現フレームのカメラ座標
	Vector2DFloat cameraPos_;

	//前フレームのカメラ座標
	Vector2DFloat oldPos_;

	void (Camera::* _state)();

	//追従対象変更中
	void Switching();

	//カメラ追従状態
	void Follow();

};

