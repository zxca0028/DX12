#include "pch.h"
#include "CommandList.h"
#include "DirectX12/DirectX12.h"

namespace CLIENT
{
    CommandList::CommandList()
    {
    }

    HRESULT CommandList::Init(D3D12_COMMAND_LIST_TYPE type)
    {
        DirectX12::GetDevice()->CreateCommandAllocator(type, IID_PPV_ARGS(&mCmdAllocator));
        DirectX12::GetDevice()->CreateCommandList(0, type, mCmdAllocator.Get(), nullptr, IID_PPV_ARGS(mCmdList.GetAddressOf()));

        mCmdList->Close();

        return S_OK;
    }

    void CommandList::Reset()
    {
        mCmdList->Reset(mCmdAllocator.Get(), nullptr);
    }

    void CommandList::ResetAllocator()
    {
        mCmdAllocator->Reset();
    }

    SharedPtr<CommandList> CommandList::Create(D3D12_COMMAND_LIST_TYPE type)
    {
        auto pInstance = CreateSharedPtr<CommandList>();

        if (nullptr == pInstance)
        {
            LOG_ERROR("Fail to Assign CommandList");
        }
        if (FAILED(pInstance->Init(type)))
        {
            LOG_ERROR("Fail to Init CommandList");
        }

        LOG_INFO("Complete to Create CommandList");

        return pInstance;
    }
}