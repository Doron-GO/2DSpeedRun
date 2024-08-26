#include <chrono>
#include "Time.h"

namespace Default
{
	template<class T>
		requires std::is_arithmetic_v<T>
	constexpr T NANO = 1000000000;
	constexpr Time::DeltaType MAX_DELTA = 0.5;
	constexpr Time::DeltaType TIME_RATE = 1.0;

}

Time::Time()
{
	now_ = std::chrono::system_clock::now();
	delta_ = 0.0;
	rate_ = Default::TIME_RATE;
	total_ = 0.0;
}

Time::DeltaType Time::Update(void)
{
	old_ = now_;
	now_ = std::chrono::system_clock::now();
	delta_ = static_cast<DeltaType>(std::chrono::duration_cast<std::chrono::nanoseconds>(now_ - old_).count()) / Default::NANO<DeltaType>;

	if (delta_ > Default::MAX_DELTA)
	{
		delta_ = 0.0;
	}

	total_ += delta_;

	return GetDeltaTime();
}

Time::DeltaType Time::GetDeltaTime(void)const
{
	return delta_ * rate_;
}

void Time::SetTimeRate(DeltaType rate)
{
	rate_ = rate;
}

Time::DeltaType Time::GetTotalTime(void) const
{
	return total_;
}