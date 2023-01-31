#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shellapi.h> // For CommandLineToArgvW

// The min/max macros conflict with like-named member functions.
// Only use std::min and std::max defined in <algorithm>.
#if defined(min)
#undef min
#endif

#if defined(max)
#undef max
#endif

// Windows Runtime Library. Needed for Microsoft::WRL::ComPtr<> template class.
#include <wrl.h>
using namespace Microsoft::WRL;

// DirectX 12 specific headers.
#include <initguid.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <dxgidebug.h>
#include <directxcollision.h>

#pragma comment( lib, "user32" )          // link against the win32 library
#pragma comment( lib, "d3d12.lib" )       // direct3D library
#pragma comment( lib, "dxgi.lib" )        // directx graphics interface
#pragma comment( lib, "d3dcompiler.lib" ) // shader compiler
#pragma comment(lib, "dxguid.lib")

// D3D12 extension library.
#include "d3dx12.h"

// STL Headers
#include <algorithm>
#include <cassert>
#include <chrono>

//Miscellaneous
#include <stdio.h>
#include <exception>
#include <chrono>
#include <fstream>
#include <stdlib.h>
#include <cstdlib>
#include <random>

constexpr UINT MAXINSTANCES = 100;
constexpr float FOVY = DirectX::XM_PIDIV4;

// From DXSampleHelper.h 
// Source: https://github.com/Microsoft/DirectX-Graphics-Samples
inline void ThrowIfFailed(HRESULT hr)
{
    if (FAILED(hr))
    {
        printf("%x\n", hr);
        throw std::exception();
    }
}

constexpr WCHAR wndClassName[] = L"Window Class";

inline UINT GetStringByteSize(const char* string) {
    UINT ret = 0;
    while (*(string++))
        ret++;
    return ret;
}

using namespace DirectX;

struct CameraShaderConstants {
    XMMATRIX cameraMatrix;
    XMFLOAT4 viewDirection;
    XMFLOAT4 viewPosition;
};

struct PointLight {
    XMFLOAT4 position;
    XMFLOAT4 color;
};

struct SimpleVertex {
    XMFLOAT4 position;
    XMFLOAT4 normal;
};

struct ViewVolume {
    float r;
    float t;
    float n;
    float f;
};

class RandomGenerator {
    std::mt19937 m_gen;
    std::uniform_real_distribution<float> m_dist;
public:
    float Get() { return m_dist(m_gen); };
    RandomGenerator() : m_gen(std::random_device()()), m_dist(-1.0f, 1.0f) {};
};

struct VertexView {
    D3D12_VERTEX_BUFFER_VIEW view;
    UINT nVertices;
};

class RenderableObject {
protected:
    ComPtr<ID3D12Resource> m_vertices;
public:
    VertexView m_vertexView;
    virtual ~RenderableObject() {};
};

enum PIPELINESTATE {
    HEIGHTMAP_TERRAIN,
    LIGHT_PERSPECTIVE_DEPTHMAP,
    LAYERED_DEPTHMAP_DIRECTIONAL,
    LAYERED_DEPTHMAP_CUBICAL,
    CASCADED_SHADOW_MAP,
    SIMPLE
};

struct AspectRatio {
    float m_aspectRatio;
};

struct CameraView : public virtual AspectRatio {
    XMMATRIX m_viewMatrix = XMMatrixIdentity();
    float m_nearZ = 0.0f;
    float m_farZ = 0.0f;
    float m_fovY = 0.0f;
    XMVECTOR m_direction = {};
};

struct Time {
    std::chrono::time_point<std::chrono::high_resolution_clock> m_time;
};