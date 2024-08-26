#pragma once

class CheckPoint;

class TimeCount
{
public:
	TimeCount(CheckPoint& CheckPoint);
	~ TimeCount();

	void Update(float startime);
	void Draw();
	void GoalUpdate(float startime);
	void GoalDraw();
	void GameUpdate(float startime);
	void GameDraw();
	void SingleDraw();
	void SetStart();
	bool IsEnd();
	float ElapsedTime();
	void Count();
	void SinglePlay();

private:

	void (TimeCount::* _update)(float startime);
	void (TimeCount::* _draw)();

	CheckPoint& checkPoint_;
	int fontHandle_;
	int goalImg_;
	int restertImg_ ;
	bool endFlag_;

	bool startFlag_;//計測開始フラグ;
	float deltaTime_ ;
	float nowTime_ ;
	float elapsedTime_;
	float oldTime_;
	float record_;
};

