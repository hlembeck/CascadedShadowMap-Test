#pragma once
#include "PipelineObjects.h"
#include "TerrainClipmapping.h"

class Scene :
	public PipelineObjects,
	public virtual TerrainClipmap
{
	std::vector<RenderableObject*> m_objects;
	Material m_materials[NMATERIALS] = {};
public:
	Scene();
	~Scene();
	void Load();

	void Draw(ID3D12GraphicsCommandList* commandList);
};