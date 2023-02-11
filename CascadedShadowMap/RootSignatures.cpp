#include "RootSignatures.h"

void RootSignatures::LoadComputeRS() {
    D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
        D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;

    D3D12_DESCRIPTOR_RANGE range = {
        D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
        1,
        0,
        0,
        0
    };

    CD3DX12_ROOT_PARAMETER rootParameters[3] = {};
    rootParameters[0].InitAsConstants(3, 0);
    rootParameters[1].InitAsDescriptorTable(1, &range);
    rootParameters[2].InitAsUnorderedAccessView(0);

    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc(_countof(rootParameters), rootParameters, 0, NULL, rootSignatureFlags);
    ComPtr<ID3DBlob> rootSignatureBlob;
    ComPtr<ID3DBlob> errorBlob;
    ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &rootSignatureBlob, &errorBlob));
    ThrowIfFailed(m_device->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(), rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&m_cRootSignature)));
}

void RootSignatures::LoadGraphicsRS() {
    D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
        D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
        D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;

    CD3DX12_ROOT_PARAMETER rootParameters[3] = {};

    CD3DX12_DESCRIPTOR_RANGE descriptorRange[3] = {};
    descriptorRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);

    descriptorRange[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 2);
    descriptorRange[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

    rootParameters[0].InitAsConstantBufferView(0);
    rootParameters[1].InitAsDescriptorTable(1, descriptorRange);
    rootParameters[2].InitAsDescriptorTable(2, descriptorRange + 1);

    D3D12_STATIC_SAMPLER_DESC staticSamplers[2] = {};

    D3D12_STATIC_SAMPLER_DESC texSampler = {
        D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR,
        D3D12_TEXTURE_ADDRESS_MODE_BORDER,
        D3D12_TEXTURE_ADDRESS_MODE_BORDER,
        D3D12_TEXTURE_ADDRESS_MODE_BORDER,
        0,
        1,
        D3D12_COMPARISON_FUNC_LESS_EQUAL,
        D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK,
        0.0f,
        0.0f,
        0,
        0,
        D3D12_SHADER_VISIBILITY_PIXEL
    };
    texSampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

    D3D12_STATIC_SAMPLER_DESC depthSampler = {
        D3D12_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT,
        D3D12_TEXTURE_ADDRESS_MODE_BORDER,
        D3D12_TEXTURE_ADDRESS_MODE_BORDER,
        D3D12_TEXTURE_ADDRESS_MODE_BORDER,
        0,
        1,
        D3D12_COMPARISON_FUNC_LESS_EQUAL,
        D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK,
        0.0f,
        0.0f,
        1,
        0,
        D3D12_SHADER_VISIBILITY_PIXEL
    };

    staticSamplers[0] = texSampler;
    staticSamplers[1] = depthSampler;

    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc(_countof(rootParameters), rootParameters, 2, staticSamplers, rootSignatureFlags);
    ComPtr<ID3DBlob> rootSignatureBlob;
    ComPtr<ID3DBlob> errorBlob;
    ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &rootSignatureBlob, &errorBlob));
    ThrowIfFailed(m_device->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(), rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&m_gRootSignature)));
}

void RootSignatures::OnInit() {
    LoadComputeRS();
    LoadGraphicsRS();
    m_cRootSignature->SetName(L"Compute Root Signature");
    m_gRootSignature->SetName(L"Graphics Root Signature");
}