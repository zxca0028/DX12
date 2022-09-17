#pragma once

#include "Core.h"

namespace CLIENT
{
	struct WindowDesc
	{
		HWND      hWnd  = nullptr;
		HINSTANCE hInst = nullptr;
		u32 Width  = 0;
		u32 Height = 0;
		const wchar_t* Title = nullptr;
	};

	class Window final
	{
	private:
		WindowDesc mWindowDesc;
	public:
		bool Create();
		bool Update();
	};
}