#pragma once

#include "DirectX12.h"

namespace CLIENT
{
	class RootSignature final
	{
	public:
		explicit RootSignature();
		virtual ~RootSignature() = default;
	public:
		HRESULT Init();
	private:
		ComPtr<ID3D12RootSignature> mRootSignature;
	public:
		ID3D12RootSignature* Get() const
		{
			return mRootSignature.Get();
		}
	public:
		static SharedPtr<RootSignature> Create();
	};
}