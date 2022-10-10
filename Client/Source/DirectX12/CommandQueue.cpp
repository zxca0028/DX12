#include "pch.h"
#include "DirectX12/CommandQueue.h"
#include "DirectX12/CommandList.h"
#include "DirectX12/Fence.h"

namespace CLIENT
{
	CommandQueue::CommandQueue()
	{
	}

	HRESULT CommandQueue::Init(D3D12_COMMAND_LIST_TYPE type)
	{
		mFence = Fence::Create();

		D3D12_COMMAND_QUEUE_DESC CommandQueueDesc;
		{
			CommandQueueDesc.Type     = type;
			CommandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
			CommandQueueDesc.Flags    = D3D12_COMMAND_QUEUE_FLAG_NONE;
			CommandQueueDesc.NodeMask = 0;

			if (FAILED(DirectX12::GetDevice()->CreateCommandQueue(&CommandQueueDesc, IID_PPV_ARGS(mCmdQueue.GetAddressOf()))))
			{
				LOG_ERROR("Fail to Create CommandQueue");
			}
		}

		mFenceValue.resize(DirectX12::GetBackBufferCount());

		return S_OK;
	}

	void CommandQueue::Flush()
	{
		mFence->IncreateValue();
		mCmdQueue->Signal(mFence->Get(), mFence->GetValue());
		mFence->WaitForFenceValue(mFence->GetValue());
	}

	CommandQueue* CommandQueue::Create(u64 bufferCount, D3D12_COMMAND_LIST_TYPE type)
	{
		auto pInstance = new CommandQueue();

		if (nullptr == pInstance)
		{
			LOG_ERROR("Fail to Assign CommandQueue");
		}
		if (FAILED(pInstance->Init(type)))
		{
			LOG_ERROR("Fail to Init CommandQueue");
		}

		LOG_INFO("Complete to Create CommandQueue");

		return pInstance;
	}
}