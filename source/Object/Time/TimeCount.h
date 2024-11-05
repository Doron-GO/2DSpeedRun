#pragma once
#include<functional>
#include<map>

class CheckPoint;

class TimeCount
{
public:
	TimeCount(CheckPoint& CheckPoint);
	~ TimeCount();

	void Update(float startime);
	void Draw();
	void GoalUpdate();
	void GoalDraw();
	void GameUpdate();
	void GameDraw();
	void SingleDraw();
	void SetStart();
	bool IsEnd();
	float ElapsedTime();
	void Count();
	void SetSinglePlayMode();

private:

	enum class STATE
	{
		GAME,
		GOAL,
		SINGLE
	};

	STATE state_;

	std::map<STATE, std::function<void(void)>> draw_;
	std::map<STATE, std::function<void(void)>> update_;

	CheckPoint& checkPoint_;
	int finalRecordFontHandle_;
	int recordFontHandle_;
	int goalImg_;
	int restertImg_ ;
	bool endFlag_;

	float startTime_;
	bool startFlag_;//計測開始フラグ;
	float deltaTime_ ;
	float nowTime_ ;
	float elapsedTime_;
	float oldTime_;
	float record_;
};

