#include "pch.h"
#include "Application.h"

namespace CLIENT
{
	bool Application::Create()
	{
		GlobalInstance::Create();
		{
			Log::Register<Log>();
		}

		if (false == (mWindow = CreateUniquePtr<Window>())->Create())
		{
			return false;
		}

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
			/* Timer */

			/* Scheduler */
		}
	}

	void Application::Destroy()
	{
		GlobalInstance::Destroy();
	}
}