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

	class Window final : public ISingleton
	{
		friend class ISingleton;
		friend class Application;
		friend class DirectX12;
	private:
		static WindowDesc mWindowDesc;
	private:
		void Init();
		bool Update();
	};
}