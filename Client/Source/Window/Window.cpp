#include "pch.h"
#include "Window.h"

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace CLIENT
{
	WindowDesc Window::mWindowDesc = {};
	vector<WindowCallBack> Window::mCallBack;

	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		for (auto& callBack : Window::mCallBack)
		{
			LRESULT result = callBack(hWnd, msg, wParam, lParam);
			if (result)
			{
				return result;
			}
		}

		switch (msg)
		{
		case WM_KEYDOWN:
		case WM_DESTROY:
			::PostQuitMessage(0);
			break;
		}
		return ::DefWindowProc(hWnd, msg, wParam, lParam);
	}

	void CLIENT::Window::Init()
	{
		mWindowDesc.hInst  = GetModuleHandle(NULL);
		mWindowDesc.Width  = Width;
		mWindowDesc.Height = Height;
		mWindowDesc.Title  = L"Title";

		WNDCLASSEXW wcex = {};
		{
			wcex.cbSize        = sizeof(WNDCLASSEX);
			wcex.style         = CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc   = WndProc;
			wcex.cbClsExtra    = 0;
			wcex.cbWndExtra    = 0;
			wcex.hInstance     = mWindowDesc.hInst;
			wcex.hIcon         = LoadIcon(0, IDI_APPLICATION);
			wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
			wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			wcex.lpszMenuName  = NULL;
			wcex.lpszClassName = mWindowDesc.Title;
			wcex.hIconSm       = LoadIcon(0, IDI_APPLICATION);

			RegisterClassExW(&wcex);
		}

		RECT rcWindow = { 0, 0, Width, Height };
		AdjustWindowRect(&rcWindow, WS_OVERLAPPEDWINDOW, false);

		mWindowDesc.hWnd = CreateWindowW
		(
			mWindowDesc.Title,
			mWindowDesc.Title,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			rcWindow.right - rcWindow.left,
			rcWindow.bottom - rcWindow.top,
			nullptr,
			nullptr,
			mWindowDesc.hInst,
			nullptr
		);

		if (0 == mWindowDesc.hWnd)
		{
			LOG_ERROR("Fail to Register Window");
		}

		ShowWindow(mWindowDesc.hWnd, SW_NORMAL);
		UpdateWindow(mWindowDesc.hWnd);

		LOG_INFO("Complete to Register Window");
	}

	bool Window::Update()
	{
		MSG msg;

		while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message != WM_QUIT)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				return false;
			}
		}

		return true;
	}

	void Window::AddCallBack(const WindowCallBack& callback)
	{
		mCallBack.push_back(callback);
	}
}