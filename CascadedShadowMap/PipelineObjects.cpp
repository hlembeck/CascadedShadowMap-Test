#include "PipelineObjects.h"

void PipelineObjects::LoadHeightmapTerrain() {
    ComPtr<ID3DBlob> vertexShader;
    UINT compileFlags = 0;
    ThrowIfFailed(D3DCompileFromFile(L"HeightmapTerrain.hlsl", NULL, NULL, "main", "cs_5_1", compileFlags, 0, &vertexShader, NULL));

    D3D12_COMPUTE_PIPELINE_STATE_DESC psDesc = {
        m_cRootSignature.Get(),
        CD3DX12_SHADER_BYTECODE(vertexShader.Get())
    };
    ThrowIfFailed(m_device->CreateComputePipelineState(&psDesc, IID_PPV_ARGS(&m_heightMapTerrain)));
}

void PipelineObjects::LoadLPDM() {
    ComPtr<ID3DBlob> vertexShader;
    ComPtr<ID3DBlob> pixelShader;
    ComPtr<ID3DBlob> geometryShader;
    UINT compileFlags = 0;

    ThrowIfFailed(D3DCompileFromFile(L"LightPerspectiveDepthMap.hlsl", NULL, NULL, "vsMain", "vs_5_1", compileFlags, 0, &vertexShader, NULL));
    ThrowIfFailed(D3DCompileFromFile(L"LightPerspectiveDepthMap.hlsl", NULL, NULL, "psMain", "ps_5_1", compileFlags, 0, &pixelShader, NULL));
    ThrowIfFailed(D3DCompileFromFile(L"LightPerspectiveDepthMap.hlsl", NULL, NULL, "gsMain", "gs_5_1", compileFlags, 0, &geometryShader, NULL));

    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    D3D12_DEPTH_STENCIL_DESC depthDesc = {
        TRUE,
        D3D12_DEPTH_WRITE_MASK_ALL,
        D3D12_COMPARISON_FUNC_LESS,
        FALSE
    };

    D3D12_RASTERIZER_DESC rasterizerDesc = {
        D3D12_FILL_MODE_SOLID,
        D3D12_CULL_MODE_BACK,
        FALSE,
        4194304,
        1.0f, //clamp
        0.0f, //slope scaled
        TRUE,
        FALSE,
        FALSE,
        0,
        D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {
        m_gRootSignature.Get(),
        CD3DX12_SHADER_BYTECODE(vertexShader.Get()),
        CD3DX12_SHADER_BYTECODE(pixelShader.Get()),
        {},
        {},
        CD3DX12_SHADER_BYTECODE(geometryShader.Get()),
        {},
        CD3DX12_BLEND_DESC(D3D12_DEFAULT),
        UINT_MAX,
        CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT),
        depthDesc,
        { inputElementDescs, _countof(inputElementDescs) },
        D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED,
        D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
        1
    };
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R32_FLOAT;
    psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
    psoDesc.SampleDesc.Count = 1;
    ThrowIfFailed(m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_lpdm)));
}

void PipelineObjects::LoadDirectionalLDM() {
    ComPtr<ID3DBlob> vertexShader;
    ComPtr<ID3DBlob> pixelShader;
    ComPtr<ID3DBlob> geometryShader;
    UINT compileFlags = 0;

    ThrowIfFailed(D3DCompileFromFile(L"DepthPeelingDirectional.hlsl", NULL, NULL, "VS", "vs_5_1", compileFlags, 0, &vertexShader, NULL));
    ThrowIfFailed(D3DCompileFromFile(L"DepthPeelingDirectional.hlsl", NULL, NULL, "PS", "ps_5_1", compileFlags, 0, &pixelShader, NULL));
    ThrowIfFailed(D3DCompileFromFile(L"DepthPeelingDirectional.hlsl", NULL, NULL, "GS", "gs_5_1", compileFlags, 0, &geometryShader, NULL));

    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    D3D12_DEPTH_STENCIL_DESC depthDesc = {
        TRUE,
        D3D12_DEPTH_WRITE_MASK_ALL,
        D3D12_COMPARISON_FUNC_LESS,
        FALSE
    };

    D3D12_RASTERIZER_DESC rasterizerDesc = {
        D3D12_FILL_MODE_SOLID,
        D3D12_CULL_MODE_BACK,
        FALSE,
        1,
        1.0f, //clamp
        0.0f, //slope scaled
        TRUE,
        FALSE,
        FALSE,
        0,
        D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {
        m_gRootSignature.Get(),
        CD3DX12_SHADER_BYTECODE(vertexShader.Get()),
        CD3DX12_SHADER_BYTECODE(pixelShader.Get()),
        {},
        {},
        CD3DX12_SHADER_BYTECODE(geometryShader.Get()),
        {},
        CD3DX12_BLEND_DESC(D3D12_DEFAULT),
        UINT_MAX,
        CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT),
        depthDesc,
        { inputElementDescs, _countof(inputElementDescs) },
        D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED,
        D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
        1
    };
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R32_FLOAT;
    psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
    psoDesc.SampleDesc.Count = 1;
    ThrowIfFailed(m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_directionalLDM)));
}

void PipelineObjects::LoadCubicalLDM() {
    ComPtr<ID3DBlob> vertexShader;
    ComPtr<ID3DBlob> pixelShader;
    ComPtr<ID3DBlob> geometryShader;
    UINT compileFlags = 0;

    ThrowIfFailed(D3DCompileFromFile(L"DepthPeelingCubical.hlsl", NULL, NULL, "VS", "vs_5_1", compileFlags, 0, &vertexShader, NULL));
    ThrowIfFailed(D3DCompileFromFile(L"DepthPeelingCubical.hlsl", NULL, NULL, "PS", "ps_5_1", compileFlags, 0, &pixelShader, NULL));
    ThrowIfFailed(D3DCompileFromFile(L"DepthPeelingCubical.hlsl", NULL, NULL, "GS", "gs_5_1", compileFlags, 0, &geometryShader, NULL));

    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    D3D12_DEPTH_STENCIL_DESC depthDesc = {
        TRUE,
        D3D12_DEPTH_WRITE_MASK_ALL,
        D3D12_COMPARISON_FUNC_LESS,
        FALSE
    };

    D3D12_RASTERIZER_DESC rasterizerDesc = {
        D3D12_FILL_MODE_SOLID,
        D3D12_CULL_MODE_BACK,
        FALSE,
        1,
        1.0f, //clamp
        0.0f, //slope scaled
        TRUE,
        FALSE,
        FALSE,
        0,
        D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {
        m_gRootSignature.Get(),
        CD3DX12_SHADER_BYTECODE(vertexShader.Get()),
        CD3DX12_SHADER_BYTECODE(pixelShader.Get()),
        {},
        {},
        CD3DX12_SHADER_BYTECODE(geometryShader.Get()),
        {},
        CD3DX12_BLEND_DESC(D3D12_DEFAULT),
        UINT_MAX,
        CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT),
        depthDesc,
        { inputElementDescs, _countof(inputElementDescs) },
        D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED,
        D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
        1
    };
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R32_FLOAT;
    psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
    psoDesc.SampleDesc.Count = 1;
    ThrowIfFailed(m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_cubicalLDM)));
}

void PipelineObjects::LoadCSM() {
    ComPtr<ID3DBlob> vertexShader;
    ComPtr<ID3DBlob> geometryShader;
    UINT compileFlags = 0;

    ThrowIfFailed(D3DCompileFromFile(L"CSM.hlsl", NULL, NULL, "VS", "vs_5_1", compileFlags, 0, &vertexShader, NULL));
    ThrowIfFailed(D3DCompileFromFile(L"CSM.hlsl", NULL, NULL, "GS", "gs_5_1", compileFlags, 0, &geometryShader, NULL));

    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    D3D12_DEPTH_STENCIL_DESC depthDesc = {
        TRUE,
        D3D12_DEPTH_WRITE_MASK_ALL,
        D3D12_COMPARISON_FUNC_LESS,
        FALSE
    };

    D3D12_RASTERIZER_DESC rasterizerDesc = {
        D3D12_FILL_MODE_SOLID,
        D3D12_CULL_MODE_BACK,
        FALSE,
        1,
        1.0f, //clamp
        0.0f, //slope scaled
        TRUE,
        FALSE,
        FALSE,
        0,
        D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {
        m_gRootSignature.Get(),
        CD3DX12_SHADER_BYTECODE(vertexShader.Get()),
        {},
        {},
        {},
        CD3DX12_SHADER_BYTECODE(geometryShader.Get()),
        {},
        CD3DX12_BLEND_DESC(D3D12_DEFAULT),
        UINT_MAX,
        CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT),
        depthDesc,
        { inputElementDescs, _countof(inputElementDescs) },
        D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED,
        D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
        1
    };
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R32_FLOAT;
    psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
    psoDesc.SampleDesc.Count = 1;
    ThrowIfFailed(m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_csm)));
}

void PipelineObjects::LoadSimplePSO() {
    ComPtr<ID3DBlob> vertexShader;
    ComPtr<ID3DBlob> pixelShader;
    UINT compileFlags = 0;

    ThrowIfFailed(D3DCompileFromFile(L"LocalIllumination.hlsl", NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_1", compileFlags, 0, &vertexShader, NULL));
    ThrowIfFailed(D3DCompileFromFile(L"LocalIllumination.hlsl", NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_1", compileFlags, 0, &pixelShader, NULL));

    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    D3D12_DEPTH_STENCIL_DESC depthDesc = {
        TRUE,
        D3D12_DEPTH_WRITE_MASK_ALL,
        D3D12_COMPARISON_FUNC_LESS,
        FALSE
    };

    D3D12_RASTERIZER_DESC rasterizerDesc = {
        D3D12_FILL_MODE_SOLID,
        D3D12_CULL_MODE_BACK,
        FALSE,
        1,
        1.0f, //clamp
        0.0f, //slope scaled
        TRUE,
        FALSE,
        FALSE,
        0,
        D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {
        m_gRootSignature.Get(),
        CD3DX12_SHADER_BYTECODE(vertexShader.Get()),
        CD3DX12_SHADER_BYTECODE(pixelShader.Get()),
        {},
        {},
        {},
        {},
        CD3DX12_BLEND_DESC(D3D12_DEFAULT),
        UINT_MAX,
        CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT),
        CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT)/*depthDesc*/,
        { inputElementDescs, _countof(inputElementDescs) },
        D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED,
        D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
        1
    };
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    //psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
    psoDesc.SampleDesc.Count = 1;
    ThrowIfFailed(m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_simplePSO)));
}

void PipelineObjects::OnInit() {
    RootSignatures::OnInit();
    LoadHeightmapTerrain();
    //LoadLPDM();
    //LoadDirectionalLDM();
    //LoadCubicalLDM();
    LoadCSM();
    LoadSimplePSO();

    //Debug
    m_heightMapTerrain->SetName(L"Heightmap Terrain PSO");
    //m_lpdm->SetName(L"LightPerspectiveDepthMap PSO");
    //m_directionalLDM->SetName(L"Directional LDM PSO");
    //m_cubicalLDM->SetName(L"Cubical LDM PSO");
    m_csm->SetName(L"CSM PSO");
    m_simplePSO->SetName(L"Simple PSO");
}