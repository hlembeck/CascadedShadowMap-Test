#pragma once
#include "PipelineObjects.h"

struct ObjectInstances {
	RenderableObject* pObject;
	ComPtr<ID3D12Resource> instancesBuffer;
	UINT nInstances;
};

class Scene :
	public virtual DXBase,
	public PipelineObjects
{
	std::vector<ObjectInstances> m_pObjectsInstances;
public:
	~Scene();
	void Load();

	void DrawDepth(ID3D12GraphicsCommandList* commandList);
	void DrawFinal(ID3D12GraphicsCommandList* commandList);
};