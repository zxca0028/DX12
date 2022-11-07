#pragma once

#include "Core.h"
#include <chrono>

namespace CLIENT
{
	class Timer;
	class TimerManager final : public ISingleton
	{
		friend class GlobalInstance;
		friend class Application;
	private:
		Dictionary<u64, SharedPtr<Timer>> mTimers;
	public:
		virtual ~TimerManager() = default;
	private:
		virtual void Init() override;
	private:
		void Update();
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