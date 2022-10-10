#pragma once

#include "Window/Window.h"

namespace CLIENT
{
	class Application
	{
	private:
		//Window* mWindow;
		SharedPtr<Window> mWindow;
	public:
		bool Create();
		void Run();
		void Destroy();
	};
}