#pragma once
#include<vector>
#include<list>
#include<memory>
#include"../../Common/Vector2D.h"
#include"../../Common/Collision.h"


class Camera;
class DangerZoneSmaller;
class Player;

//上下左右
enum class SIDE
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};


//画面端の爆発を作るための構造体
struct Bomb
{
	Vector2DFloat pos_ = {};
	int frame_ = 0;
	bool isDead = false;
	SIDE side_ ;
	Bomb(const Vector2DFloat& p, SIDE side) :pos_(p),side_(side) {}
};

class OutSide
{

public:


	bool conclusion_;

	OutSide(Camera& camera, int playerCount);
	~OutSide();

	void Update(std::vector< std::shared_ptr<Player >> players);

	//アップデート状態を変更
	void StateChanging();

	//カメラ追従状態
	void FollowStateUpdate();
	
	//追従対象変更状態
	void SwitchingStateUpdate();
	
	//描画
	void Draw(Vector2DFloat offset);

	//画面外かどうかを判定
	void IsDead(std::vector< std::shared_ptr<Player >> players);
	bool IsOutSide(Vector2DFloat pos);

	//シングルプレイモードに変更
	void SinglePlay();

	//画面からはみ出した時、上下から出たのか左右から出たのか
	//true:上下　false: 左右
	void UpDownORLeftRight(Vector2DFloat pos);

	//画面端についたプレイヤーが上端なのか下端なのかを判定
	void UpORDown(Vector2DFloat pos);

	//画面端についたプレイヤーが右端なのか左端なのかを判定
	void LeftOrRight(Vector2DFloat pos);

	//const int NumberOfSurvivors();

	//画面端ぴったりに走るように座標を画面端に合わせる関数
	void SideChange(Vector2DFloat& pos,SIDE side_);

private:

	void (OutSide::* _update)();

	//右回りに走る爆発が今画面端のどの辺にいるのか
	SIDE upperSide_;

	//左回りに走る爆発が今画面端のどの辺にいるのか
	SIDE lowerSide_;

	//カメラクラス
	Camera& camera_;

	//死亡範囲を縮小させるクラス
	std::unique_ptr<DangerZoneSmaller> dangerZone_;

	//画面端左上座標
	Vector2DFloat minPos_;
	
	//画面端右下座標
	Vector2DFloat maxPos_;

	//四角の左上
	const Vector2DFloat OUTSIDE_SCALE = { 800.0f,500.0f };

	//爆発画像の枚数
	static constexpr int EXPLOSION_IMG_NUM = 8;

	//爆発画像の枚数
	static constexpr int BIG_EXPLOSION_IMG_NUM = 11;

	//死亡エリアの座標の最大値(四角の右下)
	Vector2DFloat outSideMax_;

	//死亡エリアの座標の最小値(四角の左上)
	Vector2DFloat outSideMin_;

	//現フレームのカメラ追従対象
	Vector2DFloat outsidePos_;

	//前フレームのカメラ
	Vector2DFloat outsideOldPos_;

	//画面上を走る右回りの爆発の座標
	Vector2DFloat upperPos_;

	//進む方向と量
	Vector2DFloat upperVec_;

	//画面端下を走る左回りの爆発の座標
	Vector2DFloat lowerPos_;

	//進む方向と量
	Vector2DFloat lowerVec_;

	//画面端を走る爆発(上に向かう)
	std::list<Bomb> upBombs_;

	//画面端を走る爆発(下に向かう)
	std::list<Bomb> downBombs_;

	//小爆発中かどうか
	bool isExploding_ ;

	//大爆発中かどうか
	bool bigExploding_ ;

	//現在のモードがシングルモードかどうか
	bool singlePlayFlag_ ;

	//爆発音
	int ExplosionSound_;

	//振動させるコントローラのナンバー
	int padNum_;

	//小爆発画像ハンドル
	int bombImg_[EXPLOSION_IMG_NUM];
	
	//大爆発画像ハンドル
	int bigBombImg_[BIG_EXPLOSION_IMG_NUM];

	//爆発消去カウント
	int count_;

	//爆発画像カウント
	int expCount_  ;

	//大爆発カウント
	int bigExpCount_;

	//プレイヤー人数カウント用
	int playerCount_;
	
	//追尾対象を変更する
	float switchingTime_ ;

	//画面端を走るように爆発場所を変えていく
	void MoveChainExplosion(void);
	
	//爆発画像の表示が終わったらそれを削除する
	void ClearExplosion(void);

	//小爆発画像表示
	void ExplosionDraw(void);
	
	//大爆発画像表示
	void BigExplosionDraw(void);

	//画面端の黒枠の表示
	void OutSideDraw(void);
};

