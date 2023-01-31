#include "StaticShapes.h"

StaticCube::StaticCube(const PIPELINESTATE pipelineState) : m_pipelineState(pipelineState) {}

StaticCube::~StaticCube() {}

void StaticCube::LoadObject(ID3D12Device* device) {
	D3D12_INPUT_ELEMENT_DESC elemDescs[] = {
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};
	m_layoutDesc = {elemDescs,_countof(elemDescs)};

    XMFLOAT4 vertices[72] = {
        {-1.0f,-1.0f,1.0f,1.0f}, {0.0f,0.0f,1.0f,0.0f},
        {1.0f,1.0f,1.0f,1.0f}, {0.0f,0.0f,1.0f,0.0f},
        {-1.0f,1.0f,1.0f,1.0f}, {0.0f,0.0f,1.0f,0.0f},
        {-1.0f,-1.0f,1.0f,1.0f}, {0.0f,0.0f,1.0f,0.0f},
        {1.0f,-1.0f,1.0f,1.0f}, {0.0f,0.0f,1.0f,0.0f},
        {1.0f,1.0f,1.0f,1.0f}, {0.0f,0.0f,1.0f,0.0f},

        {-1.0f,-1.0f,-1.0f,1.0f}, {0.0f,0.0f,-1.0f,0.0f},
        {-1.0f,1.0f,-1.0f,1.0f}, {0.0f,0.0f,-1.0f,0.0f},
        {1.0f,1.0f,-1.0f,1.0f}, {0.0f,0.0f,-1.0f,0.0f},
        {-1.0f,-1.0f,-1.0f,1.0f}, {0.0f,0.0f,-1.0f,0.0f},
        {1.0f,1.0f,-1.0f,1.0f}, {0.0f,0.0f,-1.0f,0.0f},
        {1.0f,-1.0f,-1.0f,1.0f}, {0.0f,0.0f,-1.0f,0.0f},

        {-1.0f,-1.0f,-1.0f,1.0f},{-1.0f,0.0f,0.0f,0.0f},
        {-1.0f,1.0f,1.0f,1.0f},{-1.0f,0.0f,0.0f,0.0f},
        {-1.0f,1.0f,-1.0f,1.0f},{-1.0f,0.0f,0.0f,0.0f},
        {-1.0f,-1.0f,-1.0f,1.0f},{-1.0f,0.0f,0.0f,0.0f},
        {-1.0f,-1.0f,1.0f,1.0f},{-1.0f,0.0f,0.0f,0.0f},
        {-1.0f,1.0f,1.0f,1.0f},{-1.0f,0.0f,0.0f,0.0f},

        {1.0f,-1.0f,1.0f,1.0f},{1.0f,0.0f,0.0f,0.0f},
        {1.0f,1.0f,-1.0f,1.0f},{1.0f,0.0f,0.0f,0.0f},
        {1.0f,1.0f,1.0f,1.0f},{1.0f,0.0f,0.0f,0.0f},
        {1.0f,-1.0f,1.0f,1.0f},{1.0f,0.0f,0.0f,0.0f},
        {1.0f,-1.0f,-1.0f,1.0f},{1.0f,0.0f,0.0f,0.0f},
        {1.0f,1.0f,-1.0f,1.0f},{1.0f,0.0f,0.0f,0.0f},

        {-1.0f,1.0f,-1.0f,1.0f},{0.0f,1.0f,0.0f,0.0f},
        {-1.0f,1.0f,1.0f,1.0f},{0.0f,1.0f,0.0f,0.0f},
        {1.0f,1.0f,1.0f,1.0f},{0.0f,1.0f,0.0f,0.0f},
        {-1.0f,1.0f,-1.0f,1.0f},{0.0f,1.0f,0.0f,0.0f},
        {1.0f,1.0f,1.0f,1.0f},{0.0f,1.0f,0.0f,0.0f},
        {1.0f,1.0f,-1.0f,1.0f},{0.0f,1.0f,0.0f,0.0f},

        {-1.0f,-1.0f,-1.0f,1.0f},{0.0f,-1.0f,0.0f,0.0f},
        {1.0f,-1.0f,1.0f,1.0f},{0.0f,-1.0f,0.0f,0.0f},
        {-1.0f,-1.0f,1.0f,1.0f},{0.0f,-1.0f,0.0f,0.0f},
        {-1.0f,-1.0f,-1.0f,1.0f},{0.0f,-1.0f,0.0f,0.0f},
        {1.0f,-1.0f,-1.0f,1.0f},{0.0f,-1.0f,0.0f,0.0f},
        {1.0f,-1.0f,1.0f,1.0f},{0.0f,-1.0f,0.0f,0.0f}
    };

    m_vertexView.nVertices = 36;

    D3D12_RESOURCE_DESC resourceDesc = {
        D3D12_RESOURCE_DIMENSION_BUFFER,
        0,
        1152,
        1,
        1,
        1,
        DXGI_FORMAT_UNKNOWN,
        {1,0},
        D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
        D3D12_RESOURCE_FLAG_NONE
    };

    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);

    device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_vertices));

    BYTE* pData;
    D3D12_RANGE readRange = { 0,0 };
    m_vertices->Map(0, &readRange, (void**)&pData);
    memcpy(pData, vertices, 1152);
    m_vertices->Unmap(0, nullptr);

    m_vertexView.view = {
        m_vertices->GetGPUVirtualAddress(),
        1152,
        32
    };
}

void StaticCube::LoadRandomInstances(ID3D12Device* device, UINT numInstances, RandomGenerator* randGen, float radius) {
    m_nInstances = std::min(numInstances,MAXINSTANCES);
    XMMATRIX* matrices = new XMMATRIX[m_nInstances];
    XMMATRIX tMat;
    XMVECTOR tVec;
    for (UINT i = 0; i < m_nInstances; i++) {
        tVec = {randGen->Get(),randGen->Get(),randGen->Get()};
        tMat = XMMatrixRotationAxis(tVec,XM_PI*(randGen->Get()));
        matrices[i] = XMMatrixMultiply(tMat,XMMatrixTranslation(randGen->Get()*radius, randGen->Get() * radius, randGen->Get() * radius));
    }
}