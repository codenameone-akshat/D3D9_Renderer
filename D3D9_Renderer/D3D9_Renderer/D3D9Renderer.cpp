#include "D3D9Renderer.h"
#include <cassert>
namespace d3dgfx
{
    D3D9Renderer::D3D9Renderer()
        :m_d3d9(Direct3DCreate9(D3D_SDK_VERSION)),
        m_device(nullptr)
    {
    }
    D3D9Renderer::~D3D9Renderer()
    {
        m_d3d9 = nullptr;
    }
    void D3D9Renderer::Init()
    {
       
    }
    void D3D9Renderer::UnInit()
    {
        if (m_d3d9)
        {
            m_d3d9->Release();
        }
    }
    void D3D9Renderer::RenderFrame()
    {
    }
    bool D3D9Renderer::CheckDeviceStatus()
    {
        return false;
    }
    void D3D9Renderer::OnDeviceLost()
    {
    }
    void D3D9Renderer::OnDeviceReset()
    {
    }
    //>Handle this in the mainloop of the game, define "FULLSCREEN" for fullscreen support in the game
    HRESULT D3D9Renderer::CheckDeviceSupportFor_HALandVertexProcessing()
    {
        D3DDISPLAYMODE displayMode;
        m_d3d9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode);

        HRESULT HALResult = E_NOTIMPL;

        //Checking HAL support
#ifdef FULLSCREEN
        HALResult = m_d3d9->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8, false);
#else
        HALResult = m_d3d9->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, displayMode.Format, displayMode.Format, true);
#endif

        HRESULT VPResult = E_NOTIMPL;

        //Checking for HW Vertex Processing
        D3DCAPS9 d3dCaps;
        m_d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps);

        DWORD flags = 0;
        if (d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
        {
            flags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;

            if (d3dCaps.DevCaps & D3DDEVCAPS_PUREDEVICE)
                flags |= D3DDEVCAPS_PUREDEVICE;
        }

        if ((flags | D3DDEVCAPS_PUREDEVICE) == flags)
            VPResult = S_OK;
        else
            VPResult = E_FAIL;

        if (HALResult == S_OK && VPResult == S_OK)
            return S_OK;
        else
            return E_FAIL;
    }
}
