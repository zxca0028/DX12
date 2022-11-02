#include "pch.h"
#include "TimerManager.h"

namespace CLIENT
{
	void TimerManager::Init()
	{
		LOG_INFO("Complete to Register TimerManager");
	}

	void TimerManager::Update()
	{
	}

	void TimerManager::CreateTimer()
	{
	}

	void TimerManager::DestroyTimer()
	{
	}

	void Timer::Tick()
	{
	}

	SharedPtr<Timer> Timer::Create()
	{
		return SharedPtr<Timer>();
	}
}