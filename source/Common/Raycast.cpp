#include<DxLib.h>
#include"../_debug/_DebugDispOut.h"
#include "Raycast.h"

bool Raycast::CheckCollision(Ray ray, Collision collision, Vec playerPos)

{// ↓上下左右の一辺
    Raycast::Line lines[4] = {  

    //上の辺   
    {collision.first,
        (collision.first )+ Vec{collision.second.x,0} },
    //右辺
    {(collision.first) + Vec{collision.second.x,0} ,
            collision.first + collision.second },
    //下の辺
    { collision.first + collision.second ,
    collision.first + Vec{0,collision.second.y } },
    //左辺
    { collision.first + Vec{0,collision.second.y} ,
    collision.first }
    };

    bool reslut = false;
    for (auto line : lines)
    {
        reslut |=   CheckRay(ray,line, playerPos);
    }
    return reslut;
}

bool Raycast::CheckCollision(Collision collision, Vec playerPos)
{

    bool reslut = false;
    reslut |= CheckRectangle(collision, playerPos);

    return reslut;
}

bool Raycast::CheckRay(Ray ray, Line line, Vec offset)
{

    //デバッグ用の実際の当たり判定を赤いワイヤーフレームで表示する
    //_dbgDrawLine(
    //    static_cast<int>(line.p.x ),
    //    static_cast<int>(line.p.y ),
    //    static_cast<int>(line.end.x ),
    //    static_cast<int>(line.end.y ),
    //    0xff0000
    //);

    auto checkCross = [](Vec vec1, Vec& vec2)
    {
        return (vec1.x*vec2.y)-(vec2.x*vec1.y);
    };
    
    auto veclLine= line.end - line.p ;

    //ray.vec: 始点からの方向ベクトル
   auto crossRayLine= checkCross(ray.vec, veclLine);
   if (crossRayLine == 0.0)
   {
       //外積の結果が０の場合平衡は絶対交差しないのではじく
       return false;
   }

   Vec v =  line.p- ray.p;
  
   auto cross_vRay  = checkCross(v, ray.vec);
   auto cross_vLine = checkCross(v, veclLine);

  auto t1 = cross_vRay / crossRayLine;
  auto t2 = cross_vLine / crossRayLine;

  if (t1 > 0.0 && t1 <= 1.0 && t2 > 0.0 && t2 <= 1.0)
  {
      return true;
  }

    return false;
}

bool Raycast::CheckRectangle(Collision collision, Vec pos)
{
    if (collision.first.y < pos.y && collision.first.x<pos.x &&
        collision.second.y>pos.y && collision.second.x > pos.x)
    {
        return true;
    }
    return false;
}

bool Raycast::RectToRectCollision(Vec pMin, Vec pMax, Vec iMin, Vec iMax)
{
    if (RightSide(pMax, iMin) && LeftSide(iMax, pMin) &&
        TopSide(pMax, iMin) && DownSide(iMax, pMin))
    {
        return true;
    }
    return false;
}
bool Raycast::TopSide(Vec Max, Vec Min)
{
    return (Max.y <= Min.y);
}

bool Raycast::DownSide(Vec Max, Vec Min)
{
    return (Max.y <= Min.y);
}

bool Raycast::LeftSide(Vec Max, Vec Min)
{
    return (Min.x <= Max.x);
}

bool Raycast::RightSide(Vec Max, Vec Min)
{
    return (Min.x <= Max.x);
}
