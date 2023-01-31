#include "Game.h"

Game::Game(UINT width, UINT height) : DXWindowBase(width,height) {}

Game::~Game() {}

void Game::OnInit() {
    DXWindowBase::OnInit();
    PipelineObjects::OnInit();
    DescriptorHeaps::OnInit(m_device.Get());
    DXWindowBase::CreateRenderTargets();
    DXWindowBase::CreateFence();
    Camera::LoadConstantBuffer(m_device.Get());
    Player::OnInit(FOVY, m_aspectRatio, 0.1f, 100.0f);
    CreateCommandList();

    Scene::Load();
    m_time = std::chrono::high_resolution_clock::now();
}

void Game::OnUpdate() {
    std::chrono::time_point<std::chrono::high_resolution_clock> curr = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> diff = curr - m_time;
    float elapsedTime = diff.count();
    m_time = curr;
    Player::Move(elapsedTime);
    if (m_inputCaptured) {
        POINT cursorPos = {};
        GetCursorPos(&cursorPos);
        Player::Pitch(((cursorPos.y - m_cursorPos.y) / m_mouseScaleFactor) * elapsedTime);
        Player::RotateWorldY(((cursorPos.x - m_cursorPos.x) / m_mouseScaleFactor) * elapsedTime);
        SetCursorPos(m_cursorPos.x, m_cursorPos.y);
    }
}

void Game::DrawFinal() {
    {
        m_commandList->RSSetViewports(1, &m_viewport);
        m_commandList->RSSetScissorRects(1, &m_scissorRect);
        CD3DX12_RESOURCE_BARRIER resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
        m_commandList->ResourceBarrier(1, &resourceBarrier);

        //D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = m_dsvHeap->GetCPUDescriptorHandleForHeapStart();
        m_commandList->OMSetRenderTargets(1, &DXWindowBase::m_rtvHandles[m_frameIndex], FALSE, nullptr);
        const float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        m_commandList->ClearRenderTargetView(DXWindowBase::m_rtvHandles[m_frameIndex], clearColor, 0, nullptr);
        //m_commandList->ClearDepthStencilView(m_dsvHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

        //ID3D12DescriptorHeap* pHeaps[] = {DescriptorHeaps::GetCBVHeap()};
        //m_commandList->SetDescriptorHeaps(1, pHeaps);
        //m_commandList->SetGraphicsRootDescriptorTable(0, Camera::GetConstantsHandle());
        m_commandList->SetGraphicsRootConstantBufferView(1, Camera::GetConstantsAddress());
        Scene::DrawFinal(m_commandList.Get());

        //Setup for presenting
        resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
        m_commandList->ResourceBarrier(1, &resourceBarrier);
        ThrowIfFailed(m_commandList->Close());
    }

    // Execute the command list.
    ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
    DXWindowBase::m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
}

void Game::CreateCommandList() {
    ThrowIfFailed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator)));
    ThrowIfFailed(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(), NULL, IID_PPV_ARGS(&m_commandList)));
    ThrowIfFailed(m_commandList->Close());

    ThrowIfFailed(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
    m_fenceValue = 1;

    // Create an event handle to use for frame synchronization.
    m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (m_fenceEvent == nullptr) {
        ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
    }

    Wait();
}

void Game::Wait() {
    const UINT64 fence = m_fenceValue;
    ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), fence));
    m_fenceValue++;
    if (m_fence->GetCompletedValue() < fence) {
        ThrowIfFailed(m_fence->SetEventOnCompletion(fence, m_fenceEvent));
        WaitForSingleObject(m_fenceEvent, INFINITE);
    }
}

void Game::ResetCommandList() {
    ThrowIfFailed(m_commandAllocator->Reset());
    ThrowIfFailed(m_commandList->Reset(m_commandAllocator.Get(), PipelineObjects::m_simplePSO.Get()));
}

void Game::OnRender() {
    Camera::UpdateConstants();
    ResetCommandList(); //Sets to final PSO. Change in future.
    m_commandList->SetGraphicsRootSignature(m_gRootSignature.Get());
    m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    DrawFinal();
    // Present the frame.
    Present();
    WaitForPreviousFrame();
}

void Game::OnKeyDown(WPARAM wParam) {
    if (wParam == VK_ESCAPE) {
        if (m_inputCaptured) {
            m_inputCaptured = FALSE;
            SetCursor(m_hCursor);
        }
        else {
            m_inputCaptured = TRUE;
            SetCursor(NULL);
        }
        return;
    }
    if (m_inputCaptured) {
        Player::OnKeyDown(wParam);
    }
}

void Game::OnKeyUp(WPARAM wParam) {
    if (m_inputCaptured) {
        Player::OnKeyUp(wParam);
    }
}

void Game::SetCursorPoint(POINT p, HCURSOR hCursor) {
    m_cursorPos = p;
    m_mouseScaleFactor = 20.0f;
    m_hCursor = hCursor;
    SetCursor(m_hCursor);
}

void Game::OnDestroy() {
    //Renderer::OnDestroy();
}

void Game::OnResize(const UINT width, const UINT height) {
    m_width = width;
    m_height = height;
    m_aspectRatio = (float)width / height;
    Camera::SetLens(FOVY, m_aspectRatio, 0.1f, 100.0f);
}