#include "pch.h"
#include "Window.h"

namespace CLIENT
{
	WindowDesc Window::mWindowDesc = {};

	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
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
			wcex.lpszMenuName  = 0;
			wcex.lpszClassName = mWindowDesc.Title;
			wcex.hIconSm       = LoadIcon(0, IDI_APPLICATION);
			
			RegisterClassExW(&wcex);
		}

		mWindowDesc.hWnd = CreateWindowW
		(
			mWindowDesc.Title,
			mWindowDesc.Title,
			WS_OVERLAPPEDWINDOW, 
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			mWindowDesc.Width,
			mWindowDesc.Height,
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
}