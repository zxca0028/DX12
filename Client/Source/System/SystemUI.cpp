#include "pch.h"
#include "SystemUI.h"
#include "Window/Window.h"
#include "DirectX12/DirectX12.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_dx12.h"
#include "Imgui/imgui_impl_win32.h"

namespace CLIENT
{
	void SystemUI::Init()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.Fonts->AddFontFromFileTTF("../Font/Consolas.ttf", 14.0f);
		ImGui::StyleColorsDark();

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		auto hWnd = GlobalInstance::Instance<Window>()->GetHWND();

		ImGui_ImplWin32_Init(hWnd);
		ImGui_ImplDX12_Init
		(
			DirectX12::GetDevice(), 
			(i32)DirectX12::GetBackBufferCount(),
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DirectX12::GetSRVHeap(),
			DirectX12::GetSRVHeap()->GetCPUDescriptorHandleForHeapStart(),
			DirectX12::GetSRVHeap()->GetGPUDescriptorHandleForHeapStart()
		);

		GlobalInstance::Instance<Scheduler>()->ScheduleByFrame(0, 0, 0, SchedulePriority::ImGuiLayer, )
	}

	EScheduleResult SystemUI::Update()
	{
		return EScheduleResult();
	}
}