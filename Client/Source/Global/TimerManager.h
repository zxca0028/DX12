#pragma once

#include "Core.h"

namespace CLIENT
{
	class Timer;
	class TimerManager final : public ISingleton
	{
		friend class Application;
	public:
		void Update();
	private:
		void CreateTimer();
		void DestroyTimer();
	};

	class Timer final
	{
		friend class TimerManager;
	private:
		std::chrono::steady_clock::time_point mTimePoint;
		f32 mTotalTime = 0.0f;
		f32 mDeltaTime = 0.0f;
	private:
		void Tick();
	public:
		static SharedPtr<Timer> Create();
	};
}