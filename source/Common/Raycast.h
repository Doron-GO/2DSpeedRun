#pragma once
#include"../Common/Vector2D.h"
#include"Collision.h"

using Vec = Vector2DFloat;

struct Raycast
{
	struct Ray {

		Vec p; //始点
		Vec vec;//方向ベクトル
	};
	struct Line {
		Vec p; //始点
		Vec end;//終点

	};

	bool CheckCollision(Ray ray, Collision collision , Vec playerPos);
	bool CheckCollision( Collision collision , Vec playerPos);
	bool CheckRay(Ray ray, Line line, Vec offset);
	bool CheckRectangle(Collision collision, Vec pos);

	//当たり判定をする関数
	bool RectToRectCollision(Vec pMin, Vec pMax, Vec iMin, Vec iMax);
	//の当たり判定上からの条件
	bool TopSide(Vec Max, Vec Min);
	bool DownSide(Vec Max, Vec Min);
	bool LeftSide(Vec Max, Vec Min);
	bool RightSide(Vec Max, Vec Min);

};

