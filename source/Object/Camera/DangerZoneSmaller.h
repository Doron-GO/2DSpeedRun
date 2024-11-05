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


	//死亡エリアの座標の最大値(四角の右下)
	Vector2DFloat& outSideMax_;

	//死亡エリアの座標の最小値(四角の左上)
	Vector2DFloat& outSideMin_;

};

