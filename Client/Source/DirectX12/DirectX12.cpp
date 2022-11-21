#include "pch.h"
#include "DirectX12.h"
#include "CommandQueue.h"
#include "CommandList.h"
#include "PipelineState.h"
#include "RootSignature.h"
#include "Window/Window.h"

namespace CLIENT
{
	static ComPtr<IDXGIFactory4>        gFactory;
	static ComPtr<ID3D12Device>         gDevice;
	static ComPtr<IDXGISwapChain>       gSwapChain;
	static ComPtr<ID3D12DescriptorHeap> gRTVHeap;
	static ComPtr<ID3D12DescriptorHeap> gDSVHeap;
	static ComPtr<ID3D12DescriptorHeap> gCBVHeap;
	static ComPtr<ID3D12DescriptorHeap> gSRVHeap;

	static        ComPtr<ID3D12Resource>  gDepthStencilBuffer;
	static vector<ComPtr<ID3D12Resource>> gBackBuffers;

	static SharedPtr<CommandQueue>  gCommandQueue;
	static SharedPtr<CommandList>   gCommandList;
	static SharedPtr<PipelineState> gPipelineState;
	static SharedPtr<RootSignature> gRootSignatrue;

	static const u32 gBackBufferCount = 2;
	static		 u32 gBackBufferIndex = 0;

	static u32 gRTVDescriptorSize = 0;
	static u32 gDSVDescriptorSize = 0;
	static u32 gCSUDescriptorSize = 0;

	static u32  gMSAAQuality = 0;
	static bool gMSAAState   = false;

	static DXGI_FORMAT gBackBufferFormat   = DXGI_FORMAT_R8G8B8A8_UNORM;
	static DXGI_FORMAT gDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	static D3D12_RECT     gScissorRect = {};
	static D3D12_VIEWPORT gViewPort    = {};

	void DirectX12::Init()
	{
		gBackBuffers.resize(gBackBufferCount);

		/* Debug Layer */
		#if defined(DEBUG) || defined(_DEBUG)
		ID3D12Debug* pDebugController;
		{
			if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&pDebugController))))
			{
				LOG_ERROR("Fail to Create DebugInterface");
			}
			else
			{
				LOG_INFO("Complete to Create DebugInterface");
			}

			pDebugController->EnableDebugLayer();
		}
		#endif

		/* Factory */
		if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&gFactory))))
		{
			LOG_ERROR("Fail to Create Factory");
		}
		else
		{
			LOG_INFO("Complete to Create Factory")
		}

		/* Device */
		if (FAILED(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(gDevice.GetAddressOf()))))
		{
			IDXGIAdapter* pWarpAdapter;
			{
				if (FAILED(gFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter))))
				{
					LOG_ERROR("Fail to Create Adapter");
				}
				else
				{
					LOG_INFO("Complete to Create Adapter")
				}
				if (FAILED(D3D12CreateDevice(pWarpAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(gDevice.GetAddressOf()))))
				{
					LOG_ERROR("Fail to Create Device");
				}
				else
				{
					LOG_INFO("Complete to Create Device");
				}
			}
		}
		else
		{
			LOG_INFO("Complete to Create Device");
		}


		/* Command Queue (+Fence) */
		gCommandQueue = CommandQueue::Create(gBackBufferCount, D3D12_COMMAND_LIST_TYPE_DIRECT);


		/* Command List */
		gCommandList = CommandList::Create(D3D12_COMMAND_LIST_TYPE_DIRECT);


		/* Descriptor Size */
		gRTVDescriptorSize = gDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		gDSVDescriptorSize = gDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		gCSUDescriptorSize = gDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);


		/* Quality Level */
		D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS QualityLevelDesc;
		{
			QualityLevelDesc.Format           = gBackBufferFormat;
			QualityLevelDesc.SampleCount      = 4;
			QualityLevelDesc.Flags            = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
			QualityLevelDesc.NumQualityLevels = 0;

			if (FAILED(gDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &QualityLevelDesc, sizeof(QualityLevelDesc))))
			{
				LOG_ERROR("Failed to CheckFeatureSupport");
			}
			else
			{
				LOG_INFO("Complete to CheckFeatureSupport");
			}

			gMSAAQuality = QualityLevelDesc.NumQualityLevels;
			assert(gMSAAQuality > 0 && "Unexpected MASS Quality Level.");
		}


		/* LogAdapter */
		#ifdef _DEBUG
		LogAdapters();
		#endif
		
		/* SwapChain */
		DXGI_SWAP_CHAIN_DESC SwapChainDesc;
		{
			SwapChainDesc.BufferDesc.Width				     = Window::mWindowDesc.Width;
			SwapChainDesc.BufferDesc.Height				     = Window::mWindowDesc.Height;
			SwapChainDesc.BufferDesc.RefreshRate.Numerator   = 60;
			SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
			SwapChainDesc.BufferDesc.Format			         = gBackBufferFormat;
			SwapChainDesc.BufferDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			SwapChainDesc.BufferDesc.Scaling		         = DXGI_MODE_SCALING_UNSPECIFIED;
			SwapChainDesc.SampleDesc.Count			         = gMSAAState ? 4 : 1;
			SwapChainDesc.SampleDesc.Quality		         = gMSAAState ? (gMSAAQuality - 1) : 0;
			SwapChainDesc.BufferUsage				         = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			SwapChainDesc.BufferCount				         = gBackBufferCount;
			SwapChainDesc.OutputWindow				         = Window::mWindowDesc.hWnd;
			SwapChainDesc.Windowed					         = true;
			SwapChainDesc.SwapEffect				         = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			SwapChainDesc.Flags						         = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

			if (FAILED((gFactory->CreateSwapChain(gCommandQueue->Get(), &SwapChainDesc, gSwapChain.GetAddressOf()))))
			{
				LOG_ERROR("Fail to Create SwapChain");
			}
			else
			{
				LOG_INFO("Complete to Create SwapChain");
			}
		}


		/* DescriptorHeaps */
		D3D12_DESCRIPTOR_HEAP_DESC RTVDesc;
		{
			RTVDesc.NumDescriptors = gBackBufferCount;
			RTVDesc.Type		   = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			RTVDesc.Flags		   = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			RTVDesc.NodeMask       = 0;

			if (FAILED(gDevice->CreateDescriptorHeap(&RTVDesc, IID_PPV_ARGS(gRTVHeap.GetAddressOf()))))
			{
				LOG_ERROR("Fail to Create RTV_DescriptorHeap");
			}
			else
			{
				LOG_INFO("Complete to Create RTV_DescriptorHeap");
			}
		}
		D3D12_DESCRIPTOR_HEAP_DESC DSVDesc;
		{
			DSVDesc.NumDescriptors = 1;
			DSVDesc.Type		   = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
			DSVDesc.Flags		   = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			DSVDesc.NodeMask       = 0;

			if (FAILED(gDevice->CreateDescriptorHeap(&DSVDesc, IID_PPV_ARGS(gDSVHeap.GetAddressOf()))))
			{
				LOG_ERROR("Fail to Create DSV_DescriptorHeap");
			}
			else
			{
				LOG_INFO("Complete to Create DSV_DescriptorHeap");
			}
		}
		D3D12_DESCRIPTOR_HEAP_DESC CBVDesc;
		{
			CBVDesc.NumDescriptors = 1024;
			CBVDesc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			CBVDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			CBVDesc.NodeMask       = 0;

			if (FAILED(gDevice->CreateDescriptorHeap(&CBVDesc, IID_PPV_ARGS(gCBVHeap.GetAddressOf()))))
			{
				LOG_ERROR("Fail to Create CBV_DescriptorHeap");
			}
			else
			{
				LOG_INFO("Complete to Create CBV_DescriptorHeap");
			}
		}
		D3D12_DESCRIPTOR_HEAP_DESC SRVDesc;
		{
			SRVDesc.NumDescriptors = 1;
			SRVDesc.Type		   = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			SRVDesc.Flags		   = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			SRVDesc.NodeMask       = 0;

			if (FAILED(gDevice->CreateDescriptorHeap(&SRVDesc, IID_PPV_ARGS(gSRVHeap.GetAddressOf()))))
			{
				LOG_ERROR("Fail to Create SRV_DescriptorHeap");
			}
			else
			{
				LOG_INFO("Complete to Create SRV_DescriptorHeap");
			}
		}

		/* RootSignature */
		gRootSignatrue = RootSignature::Create();

		Resize();
		CreateResource();
		ScissorRect();

		GlobalInstance::Instance<Scheduler>()->ScheduleByFrame
		(
			0,
			0,
			0,
			SchedulePriority::Graphics_BeginFrame,
			[&]() -> EScheduleResult
			{
				Begin();

				return EScheduleResult::Continue;
			}
		);

		GlobalInstance::Instance<Scheduler>()->ScheduleByFrame
		(
			0,
			0,
			0,
			SchedulePriority::Graphics_EndFrame,
			[&]() -> EScheduleResult
			{
				End();

				return EScheduleResult::Continue;
			}
		);

		LOG_INFO("Complete to Register DirectX12");
	}

	IDXGIFactory4* DirectX12::GetDXGIFactory()
	{
		return gFactory.Get();
	}

	ID3D12Device* DirectX12::GetDevice()
	{
		return gDevice.Get();
	}

	SharedPtr<CommandQueue> DirectX12::GetCommandQueue()
	{
		return gCommandQueue;
	}

	ID3D12Resource* DirectX12::GetBackBuffer()
	{
		return gBackBuffers[gBackBufferIndex].Get();
	}

	const u64 DirectX12::GetBackBufferCount()
	{
		return gBackBufferCount;
	}

	const u64 DirectX12::GetBackBufferIndex()
	{
		return gBackBufferIndex;
	}

	ID3D12DescriptorHeap* DirectX12::GetSRVHeap()
	{
		return gSRVHeap.Get();
	}

	ID3D12GraphicsCommandList* DirectX12::GetCommandList()
	{
		return gCommandList->Get();
	}

	void DirectX12::Begin()
	{
		gCommandList->ResetAllocator();
		gCommandList->Reset(/*PSO*/);
		
		gCommandList->Get()->RSSetViewports(1, &gViewPort);
		gCommandList->Get()->RSSetScissorRects(1, &gScissorRect);

		auto ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(GetBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		gCommandList->Get()->ResourceBarrier(1, &ResourceBarrier);

		auto BackBufferView   = CD3DX12_CPU_DESCRIPTOR_HANDLE(gRTVHeap->GetCPUDescriptorHandleForHeapStart(), gBackBufferIndex, gRTVDescriptorSize);
		auto DepthStencilView = gDSVHeap->GetCPUDescriptorHandleForHeapStart();

		gCommandList->Get()->ClearRenderTargetView(BackBufferView, DirectX::Colors::LightBlue, 0, nullptr);
		gCommandList->Get()->ClearDepthStencilView(DepthStencilView, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.f, 0, 0, nullptr);
		
		gCommandList->Get()->OMSetRenderTargets(1, &BackBufferView, true, &DepthStencilView);
	
		ID3D12DescriptorHeap* DescriptorHeaps[] = { gCBVHeap.Get() };
		gCommandList->Get()->SetDescriptorHeaps(_countof(DescriptorHeaps), DescriptorHeaps);
		gCommandList->Get()->SetDescriptorHeaps(1, gSRVHeap.GetAddressOf());
		

		//gCommandList->Get()->SetGraphicsRootSignature()
	}

	void DirectX12::End()
	{
		auto ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(GetBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		gCommandList->Get()->ResourceBarrier(1, &ResourceBarrier);

		gCommandList->Get()->Close();
		ID3D12CommandList* pCommandList[] = { gCommandList->Get() };
		gCommandQueue->Get()->ExecuteCommandLists(_countof(pCommandList), pCommandList);

		gCommandQueue->Flush();

		gSwapChain->Present(0, 0);
		gBackBufferIndex = (gBackBufferIndex + 1) % gBackBufferCount;
	}

	void DirectX12::Present()
	{
		gSwapChain->Present(0, 0);
		gBackBufferIndex = (gBackBufferIndex + 1) % gBackBufferCount;

		gCommandQueue->Flush();
	}

	void DirectX12::Flush()
	{
		gCommandQueue->Flush();
	}

	void DirectX12::Resize()
	{
		gCommandQueue->Flush();

		gCommandList->Reset();

		for (u32 i = 0; i < gBackBufferCount; ++i)
		{
			gBackBuffers[i].Reset();
		}

		gDepthStencilBuffer.Reset();

		if (FAILED(gSwapChain->ResizeBuffers((u32)gBackBufferCount, Width, Height, gBackBufferFormat, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH)))
		{
			LOG_ERROR("Fail to Resize Buffers");
		}
	
		CD3DX12_CPU_DESCRIPTOR_HANDLE RTV(gRTVHeap->GetCPUDescriptorHandleForHeapStart());

		for (u32 i = 0; i < gBackBufferCount; ++i)
		{
			if (FAILED(gSwapChain->GetBuffer(i, IID_PPV_ARGS(gBackBuffers[i].GetAddressOf()))))
			{
				LOG_ERROR("Fail to Get Buffer");
			}

			gDevice->CreateRenderTargetView(gBackBuffers[i].Get(), nullptr, RTV);

			RTV.Offset(1, gRTVDescriptorSize);
		}
	}

	void DirectX12::CreateResource()
	{
		D3D12_RESOURCE_DESC DSDesc;
		{
			DSDesc.Dimension		  = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			DSDesc.Alignment		  = 0;
			DSDesc.Width			  = Width;
			DSDesc.Height			  = Height;
			DSDesc.DepthOrArraySize   = 1;
			DSDesc.MipLevels		  = 1;
			DSDesc.Format			  = DXGI_FORMAT_R24G8_TYPELESS;
			DSDesc.SampleDesc.Count   = gMSAAState ? 4 : 1;
			DSDesc.SampleDesc.Quality = gMSAAState ? (gMSAAQuality - 1) : 0;
			DSDesc.Layout			  = D3D12_TEXTURE_LAYOUT_UNKNOWN;
			DSDesc.Flags			  = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
		}

		D3D12_CLEAR_VALUE ClearValue;
		{
			ClearValue.Format				= gDepthStencilFormat;
			ClearValue.DepthStencil.Depth   = 1.f;
			ClearValue.DepthStencil.Stencil = 0;
		}

		CD3DX12_HEAP_PROPERTIES HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

		if (FAILED(gDevice->CreateCommittedResource(&HeapProperties, D3D12_HEAP_FLAG_NONE, &DSDesc, D3D12_RESOURCE_STATE_COMMON, &ClearValue, IID_PPV_ARGS(gDepthStencilBuffer.GetAddressOf()))))
		{
			LOG_ERROR("Fail to Create CommittedResource");
		}

		D3D12_DEPTH_STENCIL_VIEW_DESC DSVDesc;
		{
			DSVDesc.Flags			   = D3D12_DSV_FLAG_NONE;
			DSVDesc.ViewDimension	   = D3D12_DSV_DIMENSION_TEXTURE2D;
			DSVDesc.Format			   = gDepthStencilFormat;
			DSVDesc.Texture2D.MipSlice = 0;
		}

		gDevice->CreateDepthStencilView(gDepthStencilBuffer.Get(), &DSVDesc, gDSVHeap->GetCPUDescriptorHandleForHeapStart());
	
		auto Transition = CD3DX12_RESOURCE_BARRIER::Transition(gDepthStencilBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
		gCommandList->Get()->ResourceBarrier(1, &Transition);

		gCommandList->Get()->Close();
		ID3D12CommandList* CommandList[] = { gCommandList->Get() };
		gCommandQueue->Get()->ExecuteCommandLists(_countof(CommandList), CommandList);

		gCommandQueue->Flush();
	}

	void DirectX12::ScissorRect()
	{
		gViewPort.TopLeftX = 0;
		gViewPort.TopLeftY = 0;
		gViewPort.Width    = static_cast<float>(Width);
		gViewPort.Height   = static_cast<float>(Height);
		gViewPort.MinDepth = 0.f;
		gViewPort.MaxDepth = 1.f;

		gScissorRect = { 0, 0, Width, Height };
	}

	void DirectX12::LogAdapters()
	{
		u32 i = 0;
		IDXGIAdapter* pAdapter = nullptr;

		vector<IDXGIAdapter*> Adapters;

		while (gFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND)
		{
			DXGI_ADAPTER_DESC Desc;
			pAdapter->GetDesc(&Desc);

			std::wstring Text = TEXT("***Adapter : ");
			Text += Desc.Description;
			Text += TEXT("\n");

			OutputDebugString(Text.c_str());

			Adapters.push_back(pAdapter);

			++i;
		}

		for (u32 i = 0; i < Adapters.size(); ++i)
		{
			LogAdaptersOutputs(Adapters[i]);

			Adapters[i]->Release();
			Adapters[i] = 0;
		}
	}

	void DirectX12::LogAdaptersOutputs(IDXGIAdapter* adapter)
	{
		u32 i = 0;

		IDXGIOutput* pOutput = nullptr;

		while (adapter->EnumOutputs(i, &pOutput) != DXGI_ERROR_NOT_FOUND)
		{
			DXGI_OUTPUT_DESC desc;
			pOutput->GetDesc(&desc);

			std::wstring text = L"***Output: ";
			text += desc.DeviceName;
			text += L"\n";
			OutputDebugString(text.c_str());

			LogOutputDisplayModes(pOutput, gBackBufferFormat);

			pOutput->Release();
			pOutput = 0;

			++i;
		}
	}

	void DirectX12::LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format)
	{
		u32 count = 0;
		u32 flags = 0;

		// Call with nullptr to get list count.
		output->GetDisplayModeList(format, flags, &count, nullptr);

		std::vector<DXGI_MODE_DESC> modeList(count);
		output->GetDisplayModeList(format, flags, &count, &modeList[0]);

		for (auto& x : modeList)
		{
			UINT n = x.RefreshRate.Numerator;
			UINT d = x.RefreshRate.Denominator;
			std::wstring text =
				L"Width = " + std::to_wstring(x.Width) + L" " +
				L"Height = " + std::to_wstring(x.Height) + L" " +
				L"Refresh = " + std::to_wstring(n) + L"/" + std::to_wstring(d) +
				L"\n";

			::OutputDebugString(text.c_str());
		}
	}
}