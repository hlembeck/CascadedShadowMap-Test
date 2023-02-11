#pragma once
#include "DXBase.h"

class TerrainClipmap : public virtual DXBase {
	ComPtr<ID3D12Resource> m_virtualTexture;
	ComPtr<ID3D12Heap> m_heap;

	const UINT m_resolution;
public:
	TerrainClipmap(const UINT resolution = 4096);

	void Load();

	void MakeResident();
};