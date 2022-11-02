#pragma once

#include "DirectX12.h"

namespace CLIENT
{
	class CommandList final 
	{
	public:
		explicit CommandList();
		virtual ~CommandList() = default;
	public:
		HRESULT Init(D3D12_COMMAND_LIST_TYPE type);
	private:
		ComPtr<ID3D12GraphicsCommandList> mCmdList;
		ComPtr<ID3D12CommandAllocator>    mCmdAllocator;
	public:
		void Reset();
		void ResetAllocator();
	public:
		ID3D12GraphicsCommandList* Get() const
		{
			return mCmdList.Get();
		}
		ID3D12CommandAllocator* Allocator() const
		{
			return mCmdAllocator.Get();
		}
	public:
		static SharedPtr<CommandList> Create(D3D12_COMMAND_LIST_TYPE type);
	};
}