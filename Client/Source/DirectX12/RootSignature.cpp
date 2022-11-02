#include "pch.h"
#include "RootSignature.h"
#include "DirectX12.h"

namespace CLIENT
{
	RootSignature::RootSignature()
	{
	}

	HRESULT RootSignature::Init()
	{
		CD3DX12_ROOT_PARAMETER SlotRootParameter[2];

		CD3DX12_DESCRIPTOR_RANGE CBVTable;

		CBVTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);

		SlotRootParameter[0].InitAsDescriptorTable(1, &CBVTable);
		SlotRootParameter[1].InitAsDescriptorTable(2, &CBVTable);

		CD3DX12_ROOT_SIGNATURE_DESC RootSignitureDesc(1, SlotRootParameter, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		Microsoft::WRL::ComPtr<ID3DBlob> SerializedRootSignature = nullptr;
		Microsoft::WRL::ComPtr<ID3DBlob> ErrorBlob = nullptr;

		HRESULT hr = D3D12SerializeRootSignature(&RootSignitureDesc, D3D_ROOT_SIGNATURE_VERSION_1, SerializedRootSignature.GetAddressOf(), ErrorBlob.GetAddressOf());

		if (nullptr != ErrorBlob)
		{
			::OutputDebugStringA((char*)ErrorBlob->GetBufferPointer());
		}

		if (FAILED((hr)))
		{
			LOG_ERROR("Fail to SerializeRootSignature");
		}

		if (FAILED(DirectX12::GetDevice()->CreateRootSignature(0, SerializedRootSignature->GetBufferPointer(), SerializedRootSignature->GetBufferSize(), IID_PPV_ARGS(mRootSignature.GetAddressOf()))))
		{
			LOG_ERROR("Fail to Create RootSignature");
		}

		return S_OK;
	}

	SharedPtr<RootSignature> RootSignature::Create()
	{
		auto pInstance = CreateSharedPtr<RootSignature>();

		if (nullptr == pInstance)
		{
			LOG_ERROR("Fail to Assign RootSignature");
		}
		if (FAILED(pInstance->Init()))
		{
			LOG_ERROR("Fail to Init RootSignature");
		}

		LOG_INFO("Complete to Create RootSignature");

		return pInstance;
	}
}