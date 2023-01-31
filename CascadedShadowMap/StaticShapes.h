#pragma once
#include "Shared.h"

//Holds geometric data (positions, normals) of a cube of lenth 2, centered at the origin. It is NOT a unit cube, which has unit length
class StaticCube : public RenderableObject {
    ComPtr<ID3D12Resource> m_texture;
    ComPtr<ID3D12Resource> m_instanceData;
    UINT m_nInstances;
    D3D12_CPU_DESCRIPTOR_HANDLE m_cbvHandle; //Handle to CBV for the instance data.
    D3D12_INPUT_LAYOUT_DESC m_layoutDesc;
public:
    const PIPELINESTATE m_pipelineState;
    StaticCube(const PIPELINESTATE pipelineState);
    virtual ~StaticCube();
    void LoadObject(ID3D12Device* device);
    void LoadRandomInstances(ID3D12Device* device, UINT numInstances, RandomGenerator* randGen, float radius); //radius is just half width of the cube (centered at the origin) that instances are located within.
};