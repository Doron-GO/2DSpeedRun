#pragma once
#include"../../Common/Vector2D.h"
#include"../../Common/Collision.h"
#include"../../Common/Raycast.h"
#include<vector>
#include<memory>
#include<list>

class Player;
using CHECKPOINT_SAKUJO= std::pair<bool, Vector2DFloat>;

class CheckPoint
{
public:

	//チェックポイントの方向
	enum class DIRCTION
	{	
		//水平
		HORIZONTAL,
		//垂直
		VERTICAL
	};

	CheckPoint(std::vector<std::shared_ptr<Player>> players, PointColList checkpoint);
	~CheckPoint();

	//アップデート
	void Update();

	//チェックポイントの取得
	std::pair<DIRCTION, Vector2DFloat> GetCheckPoint()const;

	//ゴールをしたかどうか
	const bool IsGoal();

	//シングルモードに切り替える
	void SetSinglePlayMode();

	//シングルモードアップデート
	void SinglePlayUpdate();

	//対戦モードアップデート
	void MultiPlayUpdate();


private:

	void (CheckPoint::* update_)();

	//プレイヤー情報
	std::vector<std::shared_ptr<Player>> players_;

	//pairを配列に格納
	//first:trueなら横方向のチェックポイント false:縦方向のチェックポイント
	// second:チェックポイントの座標
	std::vector < std::pair<bool, Vector2DFloat>> checkPoints_sakujo;
	std::vector < std::pair<DIRCTION, Vector2DFloat>> checkPoints_;

	//当たり判定
	Raycast rayCast_;

	//チェックポイント当たり判定リスト
	PointColList checkPointColList_;

	//次に向かうチェックポイントナンバー
	int currentCheckPoint_;

	//プレイヤーがゴールしたかどうか
	int goalFlag_;

};

