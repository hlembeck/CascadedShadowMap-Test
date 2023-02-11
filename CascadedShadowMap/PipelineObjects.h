#pragma once
#include "DXBase.h"
#include "RootSignatures.h"

class PipelineObjects : public virtual RootSignatures {
	void LoadHeightmapTerrain();
	void LoadLPDM();
	void LoadDirectionalLDM();
	void LoadCubicalLDM();
	void LoadCSM();
	void LoadSimplePSO();
	void LoadShadowPSO();
protected:
	ComPtr<ID3D12PipelineState> m_heightMapTerrain;
	ComPtr<ID3D12PipelineState> m_lpdm;
	ComPtr<ID3D12PipelineState> m_directionalLDM;
	ComPtr<ID3D12PipelineState> m_cubicalLDM;
	ComPtr<ID3D12PipelineState> m_mainRenderDirectionalLDM;
	ComPtr<ID3D12PipelineState> m_csm; //Cascaded shadow map PSO. Both directional and perspective share the same PSO.
	ComPtr<ID3D12PipelineState> m_simplePSO; //No textures, no shadow maps, one const static light in shader. Testing local illumination.

	ComPtr<ID3D12PipelineState> m_shadowPSO;
	void OnInit();
};