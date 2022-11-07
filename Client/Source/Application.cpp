#include "pch.h"
#include "Application.h"
#include "DirectX12/DirectX12.h"
#include "System/SystemUI.h"

namespace CLIENT
{
	bool Application::Create()
	{
		GlobalInstance::Init();
		{
			GlobalInstance::Register<Log>();
			GlobalInstance::Register<Window>();
			GlobalInstance::Register<TimerManager>();
			GlobalInstance::Register<Scheduler>();
			GlobalInstance::Register<DirectX12>();

			GlobalInstance::Register<SystemUI>();
		}	

		return true;
	}

	void Application::Run()
	{
		while (true)
		{
			if (GlobalInstance::IsValid<Window>())
			{
				if (false == GlobalInstance::Instance<Window>()->Update())
				{
					break;
				}
			}
			if (GlobalInstance::IsValid<TimerManager>())
			{
				GlobalInstance::Instance<TimerManager>()->Update();
			}
			if (GlobalInstance::IsValid<Scheduler>())
			{
				GlobalInstance::Instance<Scheduler>()->Update();
			}
		}
	}

	void Application::Destroy()
	{
		GlobalInstance::Release();
	}
}