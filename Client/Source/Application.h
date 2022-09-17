#pragma once

#include "Window/Window.h"

namespace CLIENT
{
	class Application
	{
	private:
		UniquePtr<Window> mWindow;
	public:
		bool Create();
		void Run();
		void Destroy();
	};
}