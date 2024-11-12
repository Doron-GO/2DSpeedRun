#pragma once
#include "Transitor.h"

class IrisTransitor :
    public Transitor
{
public:
	IrisTransitor(bool irisOut = false, float interval = 60.0f, bool isTiled = false, int gHandle = -1);
	~IrisTransitor();
	virtual void Update() override;
	virtual void Draw() override;

private:
	int handleForMaskScreen_;//マスク転送用グラフィックハンドル
	int maskH_;//マスクハンドル
	float diagonalLength_;//対角線の長さ
	bool irisOut_ = false;//アイリスアウトフラグ(falseならアイリスイン)
	bool isTiled_;
	int gHandle_;
};

