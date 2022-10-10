#pragma once

#include "DirectX12/DirectX12.h"

namespace CLIENT
{
	class Fence;
	class CommandList;
	class CommandQueue final
	{
	private:
		explicit CommandQueue();
		virtual ~CommandQueue() = default;
	public:
		HRESULT Init(D3D12_COMMAND_LIST_TYPE type);
		void Flush();
	public:
		ID3D12CommandQueue* Get() const
		{
			return mCmdQueue.Get();
		}
	private:
		ComPtr<ID3D12CommandQueue> mCmdQueue;
		vector<u64> mFenceValue;
		Fence* mFence;
	public:
		static CommandQueue* Create(u64 bufferCount, D3D12_COMMAND_LIST_TYPE type);
	};
}