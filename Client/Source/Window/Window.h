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
		friend class GlobalInstance;
		friend class Application;
		friend class DirectX12;
	private:
		static WindowDesc mWindowDesc;
	public:
		virtual ~Window() = default;
	private:
		virtual void Init() override;
	public:
		HWND GetHWND()
		{
			return mWindowDesc.hWnd;
		}
	private:
		bool Update();
	};
}