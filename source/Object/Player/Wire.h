#pragma once
#include"../../Common/Vector2D.h"
#include"../../Common/Raycast.h"
#include "../../Common/Time.h"

class Player;

class Wire
{
public:

	Wire(Player& player,ColList& list);
	~Wire();

	void Update();
	void Draw(Vector2DFloat cameraPos);

	//スウィングを始まる前にスウィングに必要なパラメータを設定する
	void SetSwingPalam();
	
	//進行方向の斜め上にフックを伸ばす
	void SetAnchorPalam();

	//フェーズをEndSwingStateにして、player_.EndSwing();
	void StartSwingJump(); 

	//スイングフェーズに切り替える関数
	void StartSwing(); 

	//スイングフェーズに切り替える関数
	void EndSwing();

	//スイングを徐々に加速させる
	void Pump();
	
	//フックとブロックとの当たり判定
	bool IsHitHook();

	//スイング中にジャンプ
	void SwingJump();

	//スイング待機状態ステートに変更
	void ChangeStandby();

private:

	void (Wire::* _state)();

	//フックを付けるのが可能なブロックのリスト
	ColList col;

	Time timeManager_;

	Player&  player_;

	//当たり判定
	Raycast rayCast_;

	//支点の座標
	Vector2DFloat fulcrum_;

	//支点の座標(計算用)
	VECTOR fulcrum_pos;

	//スイング時の角度
	float angle_;

	//移動する方向
	VECTOR vel_;

	 //大きさ
	float v_;
	
	Vector2DFloat moveVec_;

	VECTOR Scale_;
	
	//長さ
	float length_;
	
	//力
	float pow_;

	//スイング状態
	void SwingState();

	//スイング待機状態
	void StandbyState();

	//スイング終了状態
	void EndSwingState();

	//ワイヤーを伸ばしている状態
	void AnchoringState();

};

