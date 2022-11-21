#include "pch.h"
#include "SystemUI.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_dx12.h"
#include "Imgui/imgui_impl_win32.h"
#include "Application.h"
#include "DirectX12/DirectX12.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace CLIENT
{
	SystemUI::~SystemUI()
	{
		ImGui_ImplDX12_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void SystemUI::Init()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.Fonts->AddFontFromFileTTF("../Font/Consolas.ttf", 16.0f);
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

		auto hWnd = Window::mWindowDesc.hWnd;
		
		ImGui_ImplWin32_Init(hWnd);
		Window::AddCallBack(ImGui_ImplWin32_WndProcHandler);
		ImGui_ImplDX12_Init
		(
			DirectX12::GetDevice(), 
			(i32)DirectX12::GetBackBufferCount(),
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DirectX12::GetSRVHeap(),
			DirectX12::GetSRVHeap()->GetCPUDescriptorHandleForHeapStart(),
			DirectX12::GetSRVHeap()->GetGPUDescriptorHandleForHeapStart()
		);

		GlobalInstance::Instance<Scheduler>()->ScheduleByFrame
		(
			0, 
			0, 
			0, 
			SchedulePriority::ImGuiBegin,
			[&]() -> EScheduleResult
			{
				ImGui_ImplDX12_NewFrame();
				ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();

				ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
				ImGuiWindowFlags window_flags      = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
				const ImGuiViewport* viewport      = ImGui::GetMainViewport();

				ImGui::SetNextWindowPos(viewport->WorkPos);
				ImGui::SetNextWindowSize(viewport->WorkSize);
				ImGui::SetNextWindowViewport(viewport->ID);

				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

				window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
				window_flags |= ImGuiWindowFlags_NoBackground;

				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

				ImGui::Begin("DockingSpace", nullptr, window_flags);
				ImGui::PopStyleVar();
				ImGui::PopStyleVar(2);

				ImGuiIO& io = ImGui::GetIO();
				if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
				{
					ImGuiID dockspace_id = ImGui::GetID("DockSpace");
					ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
				}
				
				ImGui::End();

				return EScheduleResult::Continue;
			}
		);

		GlobalInstance::Instance<Scheduler>()->ScheduleByFrame
		(
			0,
			0,
			0,
			SchedulePriority::ImGui,
			[&]() -> EScheduleResult
			{
				ImGui::Begin("Test1");
				ImGui::Button("Button");
				ImGui::End();

				return EScheduleResult::Continue;
			}
		);

		GlobalInstance::Instance<Scheduler>()->ScheduleByFrame
		(
			0,
			0,
			0,
			SchedulePriority::ImGuiRender,
			[&]() -> EScheduleResult
			{

				ImGui::Render();
				ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), DirectX12::GetCommandList());

				if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
				{
					ImGui::UpdatePlatformWindows();
					ImGui::RenderPlatformWindowsDefault(NULL, (void*)DirectX12::GetCommandList());
				}

				return EScheduleResult::Continue;
			}
		);
	}
}