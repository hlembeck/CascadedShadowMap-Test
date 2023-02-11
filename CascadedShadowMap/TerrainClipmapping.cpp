#include "TerrainClipmapping.h"

TerrainClipmap::TerrainClipmap(const UINT resolution) : m_resolution(resolution) {}

void TerrainClipmap::Load() {
	D3D12_RESOURCE_DESC resourceDesc = {
		D3D12_RESOURCE_DIMENSION_TEXTURE2D,
		0,
		m_resolution,
		m_resolution,
		1,
		1,
		DXGI_FORMAT_R32_FLOAT,
		{1,0},
		D3D12_TEXTURE_LAYOUT_64KB_UNDEFINED_SWIZZLE, //D3D12_RESOURCE_DESC::Layout must be D3D12_TEXTURE_LAYOUT_64KB_UNDEFINED_SWIZZLE when creating reserved resources
		D3D12_RESOURCE_FLAG_NONE
	};

	ThrowIfFailed(m_device->CreateReservedResource(&resourceDesc, D3D12_RESOURCE_STATE_COMMON, NULL, IID_PPV_ARGS(&m_virtualTexture)));

	D3D12_HEAP_PROPERTIES heapProps = {
		D3D12_HEAP_TYPE_DEFAULT,
		D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
		D3D12_MEMORY_POOL_UNKNOWN,
		0,
		0
	};

	UINT64 resourceSize;
	m_device->GetCopyableFootprints(&resourceDesc, 0, 1, 0, NULL, NULL, NULL, &resourceSize);

	D3D12_HEAP_DESC heapDesc = {
		resourceSize,
		heapProps,
		0, //64KB alignment
		D3D12_HEAP_FLAG_DENY_BUFFERS | D3D12_HEAP_FLAG_DENY_RT_DS_TEXTURES
	};

	ThrowIfFailed(m_device->CreateHeap(&heapDesc, IID_PPV_ARGS(&m_heap)));
}

void TerrainClipmap::MakeResident() {
	UINT nResourceTiles;
	D3D12_PACKED_MIP_INFO packedMipInfo;
	D3D12_TILE_SHAPE standardTileShape;
	UINT nSubresourceTiles=0;
	D3D12_SUBRESOURCE_TILING subresourceTiling;
	m_device->GetResourceTiling(m_virtualTexture.Get(), &nResourceTiles, &packedMipInfo, &standardTileShape, &nSubresourceTiles, 0, &subresourceTiling);

	printf("%d\n", nResourceTiles);
	/*D3D12_TILED_RESOURCE_COORDINATE tiledCoord = {
		0,
		0,
		0,
		0
	};

	D3D12_TILE_REGION_SIZE tileSize = {
		m_resolution*m_resolution,
		TRUE,
		m_resolution,
		m_resolution,
		1
	};

	m_commandQueue->UpdateTileMappings(m_virtualTexture.Get(),1,&tiledCoord,&tileSize,m_heap.Get(),1,0,)*/
}