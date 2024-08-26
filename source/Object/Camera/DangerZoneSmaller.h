#pragma once
#include"../../Common/Vector2D.h"

class OutSide;

class DangerZoneSmaller
{

public:

	DangerZoneSmaller(Vector2DFloat& max, Vector2DFloat& min);
	~DangerZoneSmaller();
	
	//死亡判定のエリアを収縮させる関数
	void Smaller();
	
	// 死亡判定のエリアを収縮中のアップデート
	void UpdateSmaller();

	//メンバ関数ポインタを呼び出すアップデート関数
	void Update();

	//縮小待機中アップデート
	void UpdateWait();

	//アップデートをUpdateSmallerに変更
	void Activated();

	//カウントを0にリセット
	void ResetCounter();

private:
	
	//縮小を始めるまでの計測用
	int count_;

	//アップデート管理用メンバ関数ポインタ
	void (DangerZoneSmaller::* _update)();

	//縮小開始時間
	static constexpr int SHRINK_START_TIME = 100;

	//収縮するときの最小サイズ
	const Vector2DFloat MAX_SHRINK_SIZE ={ 200.0f,150.0f };

	//縮小量
	const Vector2DFloat SCALE_STEP = { 0.2f, 0.13f };

	//死亡エリアの座標の最大値(四角の右下)
	Vector2DFloat& outSideMax_;

	//死亡エリアの座標の最小値(四角の左上)
	Vector2DFloat& outSideMin_;

};

