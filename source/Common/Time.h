#pragma once
#include <chrono>

class Time
{

public:

	using DeltaType = float;
	Time();
	/// <summary>
	/// 更新
	/// </summary>
	/// <returns>計算済みデルタタイム</returns>
	DeltaType Update(void);

	/// <summary>
	/// デルタタイム取得用関数
	/// </summary>
	/// <returns>デルタタイム</returns>
	[[nodiscard]]
	DeltaType GetDeltaTime(void)const;

	/// <summary>
	/// 時間の進み具合の設定
	/// </summary>
	/// <param name="rate">進み具合(1.0が通常)</param>
	void SetTimeRate(DeltaType rate);

	/// <summary>
	/// 総時間の取得
	/// </summary>
	/// <returns>総時間</returns>
	[[nodiscard]]
	DeltaType GetTotalTime(void)const;

private:
	//現在の時間
	std::chrono::system_clock::time_point now_;		
	//前フレーム時点でのデルフレームの時間
	std::chrono::system_clock::time_point old_;		
	//現タ値
	DeltaType delta_;
	//進み具合
	DeltaType rate_;		
	//総時間
	DeltaType total_;		

};
