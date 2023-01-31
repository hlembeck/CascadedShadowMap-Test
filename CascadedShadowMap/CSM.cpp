#include "CSM.h"
#include "DescriptorHeaps.h"

CSMDirectional::CSMDirectional() : m_nVolumes(1) {}

CSMDirectional::CSMDirectional(const UINT m, const FXMVECTOR v) : m_nVolumes(m), m_projections(std::make_unique<XMMATRIX[]>(m)), m_ratio(std::pow(m_farZ / m_nearZ, 1.0f / m)) {
	UpdateView(v,1.0f,1.0f);
}

void CSMDirectional::LoadShadowMaps(UINT resolution) {
	m_clearVal.Format = DXGI_FORMAT_D32_FLOAT;
	m_clearVal.DepthStencil.Depth = 1.0f;
	D3D12_RESOURCE_DESC csmDesc = {
		D3D12_RESOURCE_DIMENSION_TEXTURE2D,
		0,
		resolution,
		resolution,
		m_nVolumes,
		1,
		DXGI_FORMAT_D32_FLOAT,
		{1,0},
		D3D12_TEXTURE_LAYOUT_UNKNOWN,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	};

	CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_DEFAULT);

	ThrowIfFailed(m_device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &csmDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &m_clearVal, IID_PPV_ARGS(&m_shadowMaps)));
}

void CSMDirectional::LoadDSV(UINT resolution, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle) {
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {
		DXGI_FORMAT_D32_FLOAT,
		D3D12_DSV_DIMENSION_TEXTURE2DARRAY
	};

	dsvDesc.Texture2DArray = {
		0,
		0,
		m_nVolumes
	};

	m_device->CreateDepthStencilView(m_shadowMaps.Get(), &dsvDesc, dsvHandle);

}

void CSMDirectional::LoadSRV(UINT resolution, D3D12_CPU_DESCRIPTOR_HANDLE srvHandle) {
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {
		DXGI_FORMAT_R32_FLOAT,
		D3D12_SRV_DIMENSION_TEXTURE2DARRAY,
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING
	};

	srvDesc.Texture2DArray = {
		0,
		1,
		0,
		m_nVolumes,
		0,
		0.0f
	};

	m_device->CreateShaderResourceView(m_shadowMaps.Get(), &srvDesc, srvHandle);
}

void CSMDirectional::Load(const UINT resolution) {
	HandlePair handles = DescriptorHeaps::BatchHandles(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	m_dsvHandle = DescriptorHeaps::BatchHandles(D3D12_DESCRIPTOR_HEAP_TYPE_DSV).cpuHandle;
	m_srvHandle = handles.gpuHandle;
	LoadShadowMaps(resolution);
	LoadDSV(resolution, m_dsvHandle);
	LoadSRV(resolution, handles.cpuHandle);
}

void CSMDirectional::UpdateMaps(XMMATRIX invCameraView) {

	//vertices of near plane
	XMVECTOR n1 = { m_viewVol.r,m_viewVol.t,m_viewVol.n };
	XMVECTOR n2 = { -m_viewVol.r,m_viewVol.t,m_viewVol.n };
	XMVECTOR n3 = { -m_viewVol.r,-m_viewVol.t,m_viewVol.n };
	XMVECTOR n4 = { m_viewVol.r,-m_viewVol.t,m_viewVol.n };

	//vectors from near plane corner to corresponding far plane corner
	XMVECTOR d1 = { m_viewVol.r * m_ratio, m_viewVol.t * m_ratio, m_viewVol.f - m_viewVol.n };
	XMVECTOR d2 = { -m_viewVol.r * m_ratio, m_viewVol.t * m_ratio, m_viewVol.f - m_viewVol.n };
	XMVECTOR d3 = { -m_viewVol.r * m_ratio, -m_viewVol.t * m_ratio, m_viewVol.f - m_viewVol.n };
	XMVECTOR d4 = { m_viewVol.r * m_ratio, -m_viewVol.t * m_ratio, m_viewVol.f - m_viewVol.n };

	invCameraView = XMMatrixMultiply(invCameraView, m_viewMatrix); //Transform from camera space to shadow view space

	d1 = XMVector4Transform(d1, invCameraView);
	d2 = XMVector4Transform(d2, invCameraView);
	d3 = XMVector4Transform(d3, invCameraView);
	d4 = XMVector4Transform(d4, invCameraView);

	n1 = XMVector4Transform(n1, invCameraView);
	n2 = XMVector4Transform(n2, invCameraView);
	n3 = XMVector4Transform(n3, invCameraView);
	n4 = XMVector4Transform(n4, invCameraView);

	XMVECTOR min, max;
	XMFLOAT3 fMin, fMax;
	for (UINT i = 0; i < m_nVolumes; i++) {
		{ //Get bounds of box
			min = XMVectorMin(n1, n2);
			max = XMVectorMax(n1, n2);
			min = XMVectorMin(min, n3);
			max = XMVectorMax(max, n3);
			min = XMVectorMin(min, n4);
			max = XMVectorMax(max, n4);
			n1 += d1;
			n2 += d2;
			n2 += d3;
			n2 += d4;
			min = XMVectorMin(min, n1);
			max = XMVectorMax(max, n1);
			min = XMVectorMin(min, n2);
			max = XMVectorMax(max, n2);
			min = XMVectorMin(min, n3);
			max = XMVectorMax(max, n3);
			min = XMVectorMin(min, n4);
			max = XMVectorMax(max, n4);
		}
		XMStoreFloat3(&fMin, min);
		XMStoreFloat3(&fMax, max);
		m_projections[i] = XMMatrixOrthographicOffCenterLH(fMin.x, fMax.x, fMin.y, fMax.y, fMin.z, fMax.z);
	}
}

void CSMDirectional::UpdateView(const FXMVECTOR v, const float fovY, const float ar) {
	m_viewVol.r = tan(fovY * ar * 0.5f) * m_nearZ;
	m_viewVol.t = tan(fovY * 0.5f) * m_nearZ;
	m_viewMatrix = XMMatrixLookToLH({ 0.0f,0.0f,0.0f,0.0f }, v, XMVector3Orthogonal(v));
}

void CSMDirectional::UpdateDepth(const float n, const float f) {
	m_nearZ = n;
	m_ratio = std::pow(f / n, 1.0f / m_nVolumes);
}