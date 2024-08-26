#pragma once
#include"../../Common/Vector2D.h"
#include"../../Common/Collision.h"
#include"../../Common/Raycast.h"
#include<vector>
#include<memory>
#include<list>

class Player;
constexpr int CHECKPOINT_MAX =6;

class CheckPoint
{
public:

	CheckPoint(std::vector<std::shared_ptr<Player>> players, PointColList checkpoint);
	~CheckPoint();

	void Update();
	std::pair<bool, Vector2DFloat> GetCheckPoint2()const;
	const bool IsGoal();
	void SetSingleMode();

	void SinglePlay();
	void MultiPlay();
private:

	void (CheckPoint::* _work)();

	std::vector<std::shared_ptr<Player>> players_;

	//ペア―の入った配列
	//first:プレイヤーが向かう方向が上下なのか左右なのかを判定 true:上下　false:左右
	//second: チェックポイント座標
	std::vector < std::pair<bool, Vector2DFloat>> checkPoints_;

	//当たり判定クラス
	Raycast rayCast_;

	//チェックポイント当たり判定リスト
	PointColList checkPointColList_;

	//次に向かうチェックポイントナンバー
	int currentCheckPoint_;

	//プレイヤーがゴールしたかどうか
	int goalFlag_;

};

