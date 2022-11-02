#pragma once

#include "Core.h"

namespace CLIENT
{
	class CommandQueue;
	class RootSignature;

	class DirectX12 final : public ISingleton
	{
		friend class ISingleton;
	public:
		static IDXGIFactory4* GetDXGIFactory();
		static ID3D12Device*  GetDevice();
		static SharedPtr<CommandQueue>  GetCommandQueue();
	public:
		static ID3D12Resource* GetBackBuffer();
		static const u64 GetBackBufferCount();
		static const u64 GetBackBufferIndex();
	public:
		static ID3D12DescriptorHeap* GetSRVHeap();
	public:
		static ID3D12GraphicsCommandList* GetCommandList();
	public:
		void Begin();
		void End();
		void Present();
		void Flush();
	private:
		void Init();
		void Resize();
		void CreateResource();
		void ScissorRect();
	private:
		void LogAdapters();
		void LogAdaptersOutputs(IDXGIAdapter* adapter);
		void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);
	};
}