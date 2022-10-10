#include "pch.h"
#include "Fence.h"

namespace CLIENT
{
	Fence::Fence()
	{
	}

	HRESULT Fence::Init()
	{
		if (FAILED(DirectX12::GetDevice()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(mFence.GetAddressOf()))))
		{
			LOG_ERROR("Fail to Create Fence");
		}

		return S_OK;
	}

	void Fence::WaitForFenceValue(u64 value)
	{
		if (mFence->GetCompletedValue() < mFenceValue)
		{
			HANDLE eveneHandle = CreateEventEx(nullptr, 0, 0, EVENT_ALL_ACCESS);

			if (FAILED(mFence->SetEventOnCompletion(mFenceValue, eveneHandle)))
			{
				LOG_ERROR("Fail to SetEventOnCompletion");
			}

			WaitForSingleObject(eveneHandle, INFINITE);
			CloseHandle(eveneHandle);
		}
	}

	Fence* Fence::Create()
	{
		auto pInstance = new Fence();
	
		if (nullptr == pInstance)
		{
			LOG_ERROR("Fail to Assign Fence");
		}
		if (FAILED(pInstance->Init()))
		{
			LOG_ERROR("Fail to Init Fence");
		}

		LOG_INFO("Complete to Create Fence");
			
		return pInstance;
	}
}