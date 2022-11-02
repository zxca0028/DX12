#pragma once

#include "DirectX12/DirectX12.h"

namespace CLIENT
{
	class Fence final
	{
	public:
		explicit Fence();
		virtual ~Fence() = default;
	public:
		HRESULT Init();
		ID3D12Fence* Get() const
		{
			return mFence.Get();
		}
		u64 GetValue() const
		{
			return mFenceValue;
		}
		void IncreateValue()
		{
			++mFenceValue;
		}
		void WaitForFenceValue(u64 value);
	private:
		ComPtr<ID3D12Fence> mFence;
		u64 mFenceValue;
	public:
		static SharedPtr<Fence> Create();
	};
}