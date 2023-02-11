#pragma once
#include "DXBase.h"

class TerrainHeightmap :
	private virtual DXBase
{
public:
	TerrainHeightmap(UINT quality = 2);
	void Load();
	void UpdateTerrain(XMFLOAT4 position);
private:
	void LoadTexture();
	void LoadVertices();
	void Wait();
	UINT m_quality;
	ComPtr<ID3D12Resource> m_randomTexture;
	ComPtr<ID3D12Resource> m_vertices;
	ComPtr<ID3D12PipelineState> m_pipelineState;
	ComPtr<ID3D12RootSignature> m_rootSignature;

	XMFLOAT2 m_pos;

	UINT m_nVertices;
	D3D12_VERTEX_BUFFER_VIEW m_vertexView;
};