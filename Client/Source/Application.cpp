#include "pch.h"
#include "Application.h"
#include "DirectX12/DirectX12.h"

namespace CLIENT
{
	bool Application::Create()
	{
		GlobalInstance::Create();
		{
			Log::Register<Log>();
			Window::Register<Window>();
			TimerManager::Register<TimerManager>();
			Scheduler::Register<Scheduler>();
			DirectX12::Register<DirectX12>();
		}

		mWindow = GlobalInstance::Instance<Window>();
		
		return true;
	}

	void Application::Run()
	{
		while (true)
		{
			if (false == mWindow->Update())
			{
				break;
			}
			if (GlobalInstance::IsVaild<TimerManager>())
			{
				GlobalInstance::Instance<TimerManager>()->Update();
			}
			if (GlobalInstance::IsVaild<Scheduler>())
			{
				GlobalInstance::Instance<Scheduler>()->Update();
			}
		}
	}

	void Application::Destroy()
	{
		GlobalInstance::Instance<DirectX12>()->Flush();
		GlobalInstance::Destroy<DirectX12>();
		GlobalInstance::Destroy<Scheduler>();
		GlobalInstance::Destroy<TimerManager>();
		GlobalInstance::Destroy<Window>();
		GlobalInstance::Destroy<Log>();
		GlobalInstance::Release();
	}
}