#pragma once
#include<memory>
#include<vector>
#include<algorithm>
#include<map>
#include"../Stage/CheckPoint.h"
#include"../../Common/Vector2D.h"
#include"../../Common/Collision.h"
#include"../../Input/Input.h"

class Player;
class Blocks;

using Players = std::vector<std::shared_ptr<Player>>;
using Vec = Vector2DFloat;

class PlayerManager
{

public:

	enum class PLAYER_TYPE
	{
		P_1,
		P_2,
		P_3,
		P_4
	};

	PlayerManager(bool& conclusion, Blocks& blocks);
	~PlayerManager();

	//更新
	void Update(Input& input);
	//初期化
	void Init(int playerNum, ColList gruound, ColList Wall, ColList wire);
	//描画
	void Draw(Vector2DFloat cameraPos);

	const Vector2DFloat& GetPlayerPos(int playerNum);

	const Players GetPlayers();

	//次のチェックポイントに一番近いプレイヤーを検索する(背bン頭プレイヤー)
	void SearchFirstPlayer(std::pair< CheckPoint::DIRCTION,Vector2DFloat> checkPoint );

	//自身から一番近いプレイヤーを検索するし、その情報を配列に格納する
	void SearchHormingPlayer();

	//バトルの勝敗を決定する
	void BattleConclusion();

	//シングルモード用のゴール判定
	void SetGoalSingleMode();

	//アイテムとの当たり判定をプレイヤーの判定
	void CollisionItem();

	//１フレーム前の先頭プレイヤー番号の取得
	const PLAYER_TYPE GetOldFirstPlayerNum();

	//現フレームの先頭プレイヤー番号の取得
	const PLAYER_TYPE GetNewFirstPlayerNum();

	//先頭プレイヤー番号の更新
	void UpdateFirstPlayerType();

	//矩形同士の衝突判定
	bool IsRectCollision(Vec pMin, Vec pMax, Vec iMin, Vec iMax);

	//矩形同士の衝突判定
	bool IsCollisionTopSide(Vec Max,Vec Min);
	bool IsCollisionDownSide(Vec Max,Vec Min);
	bool IsCollisionLeftSide(Vec Max,Vec Min);
	bool IsCollisionRightSide(Vec Max,Vec Min);

	//シングルプレイモードにする
	void SetSinglePlayMode();

private:

	enum class IMG_TYPE
	{
		WIN,
		RESTART
	};


	std::map<IMG_TYPE, int>img_;

	//プレイヤー配列
	Players players_;

	//チェックポイントとプレイヤー間の距離
	std::vector< std::pair<int, float>> checkPointToPlayerDistance_;//firstはパッドナンバー:secondはdistance

	//1フレーム前の先頭プレイヤー番号
	PLAYER_TYPE oldFirstPlayerNum_;
	//現フレームの先頭プレイヤー番号
	PLAYER_TYPE newFirstPlayerNum_;

	//プレイヤーが最後の一人になったかどうかの判定(OutSideクラスで判定)
	bool &conclusion_;

	//勝負ありの画像
	int winImg_;

	//再戦の画像
	int restertImg_;

	//勝利したプレイヤーの番号
	int winner_;
	//シングルプレイ時のゴールに到達したかどうかの判定
	bool goalFlag_;

	//プレイヤーの生成時に引数で渡す
	Blocks& blocks_;

	//シングルプレイモードかどうかを判定する
	bool singlePlay_;

	//画像の初期化
	void InitImage(void);

};

