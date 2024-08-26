#pragma once
#include<DxLib.h>

class Transitor
{
protected:

	int sizeX ;
	int sizeY ;

	const int interval_;//切り替えにかかる時間
	int frame_ = 0;//開始から現在までのフレーム数
	int oldRT_ = 0;//切り替え前の画面
	int newRT_ = 0;//切り替え後の画面
public:
	Transitor(int interval = 60) : interval_(interval), sizeX(1600), sizeY(1000) {}
	virtual ~Transitor();
	void Start();//演出開始
	virtual void Update() = 0;//Scene::Updateの最後に呼んでね
	virtual void Draw() = 0;//Scene::Drawの最後に呼んでね
	virtual bool IsEnd()const;//演出が終了した

};

