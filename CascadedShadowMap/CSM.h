#pragma once
#include "DXBase.h"

class CSMDirectional :
	public virtual DXBase,
	public virtual AspectRatio,
	private virtual CameraView
{
	ViewVolume m_viewVol;

	D3D12_CPU_DESCRIPTOR_HANDLE m_dsvHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE m_srvHandle;
	const UINT m_nVolumes;
	float m_ratio; //Const ratio of far/near planes in the camera's view. Each shadow map is based on those near/far planes.
	std::unique_ptr<XMMATRIX[]> m_projections;
	ComPtr<ID3D12Resource> m_shadowMaps; //2D textue array
	D3D12_CLEAR_VALUE m_clearVal;
	XMMATRIX m_viewMatrix; //transform to frame in which light direction is towards +z.

	void LoadShadowMaps(UINT resolution);
	void LoadDSV(UINT resolution, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle);
	void LoadSRV(UINT resolution, D3D12_CPU_DESCRIPTOR_HANDLE srvHandle);
public:
	CSMDirectional();
	CSMDirectional(const UINT m, const FXMVECTOR v);
	void Load(const UINT resolution = 1024U);
	void UpdateMaps(XMMATRIX invCameraView);
	void UpdateView(const FXMVECTOR v, const float fovY, const float ar); //Update view matrix of light. Position and orientation (except for the viewing direction) are not relevant - only the direction is relevant, as we use an orthographic projection
	void UpdateDepth(const float n, const float f);

	//void DrawToCSM(RenderableObject* pObjects, );
};