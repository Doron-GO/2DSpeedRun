#pragma once
#include<map>
#include<list>
#include<string>
#include<array>

constexpr int GraphHD = 0;
constexpr int InvLimit = 1;
constexpr int ColLeft = 2;
constexpr int ColTop = 3;
constexpr int ColRight = 4;
constexpr int ColBottom = 5;

using AnimeData = std::array<int, 6>;
using AnimNode = std::pair<int, std::list<AnimeData>>;

//再生するとき
using AnimItr = std::list<AnimeData>::iterator;

struct AnimStr
{
	std::string imgKey_; //ImageMngに渡すキー値
	std::string objID_; //オブジェクトの種別
	std::string state_; //状態（Idle等）
	AnimItr animID_;	//アニメーションの再生位置
	int animCount_=0;		//当該アニメーションの経過フレーム
	int animPlayCount_=0;//当該アニメーションの現ループ数
	bool endFlag = false;
};

class AnimeMng
{
public:

	static AnimeMng& GetInstance()
	{
		static AnimeMng s_Instance;
		return s_Instance;
	}
	std::string LoadAnime(std::string fileName);
	bool UpdateAnime(AnimStr& animCacha);
	void SetAnime(AnimStr& animDate, std::string state);

private:

	std::map<std::string, AnimNode> animeTbl_;

	AnimeMng();
	~AnimeMng();

};


