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
			//Window::Register<Window>();
			//DirectX12::Register<DirectX12>();
		}

		//mWindow = GlobalInstance::Instance<Window>();

		return true;
	}

	void Application::Run()
	{
		while (true)
		{
			//if (false == mWindow->Update())
			//{
			//	break;
			//}
			/* Timer */
			
			/* Scheduler */
		}
	}

	void Application::Destroy()
	{
		GlobalInstance::Release();
	}
}