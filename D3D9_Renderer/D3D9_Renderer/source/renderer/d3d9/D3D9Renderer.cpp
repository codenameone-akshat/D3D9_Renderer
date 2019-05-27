#include <cassert>
#include <iostream>

#include "D3D9Renderer.h"
#include "../../utils/ComHelpers.h"
#include "../../utils/Logger.h"

namespace renderer
{
    D3D9Renderer::D3D9Renderer()
        :m_d3d9(Direct3DCreate9(D3D_SDK_VERSION)),
        m_device(std::make_unique<D3D9Device>()),
        m_effect(nullptr),
        m_d3dCaps(),
        m_modelManager(),
        m_hWindow(),
        m_vBuffer(),
        m_iBuffer(),
        m_vertexDeclarations(),
        m_camera(),
        m_vBufferVertexCount(0),
        m_iBufferIndexCount(0),
        m_primitiveCount(0)
    {
        D3DXMatrixIdentity(&m_viewMat);
        D3DXMatrixIdentity(&m_projMat);
        D3DXMatrixIdentity(&m_worldMat);
    }
    D3D9Renderer::~D3D9Renderer()
    {
        ComSafeRelease(m_d3d9);
        ComSafeRelease(m_effect);
        ComSafeRelease(m_vertexDeclarations.positionVertexDecl);
    }
    void D3D9Renderer::Init(HWND hWindow)
    {
        memcpy(&m_hWindow, &hWindow, sizeof(HWND));
        m_d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_d3dCaps);

        SetupDeviceConfiguration();
        PrepareForRendering();
    }
    void D3D9Renderer::UnInit()
    {
    }
    void D3D9Renderer::PrepareForRendering()
    {
        BuildMatrices();
        ParseModels();
        SetupVertexDeclaration();
        SetupStaticBuffers();
        SetupEffect(LightingMode::Specular);
    }
    void D3D9Renderer::PreRender()
    {
        UpdateMatrices();

        m_device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
        HRESULT result = CheckDeviceStatus();
        if (result != S_OK)
            return;
        m_device->BeginScene();
    }
    void D3D9Renderer::RenderFrame()
    {
        m_device->SetIndices(m_iBuffer);
        auto batchList = m_modelManager.GetBatchList();
        for (uint32_t itr = 0; itr < batchList.size(); ++itr)
        {
            m_device->SetStreamSource(0, m_vBuffer, 0, sizeof(PositionVertex));
            m_worldViewProjMat = m_worldMat * m_viewMat * m_projMat;
            RenderEffect(LightingMode::Specular, batchList[itr].vertexCount, batchList[itr].indexStart, batchList[itr].primitiveCount, itr);
        }
    }
    void D3D9Renderer::PostRender()
    {
        m_device->EndScene();
        m_device->Present(nullptr, nullptr, nullptr, nullptr);
    }
    HRESULT D3D9Renderer::CheckDeviceStatus()
    {
        //check every-frame for lost devices
        HRESULT result = m_device->CheckCoorparativeLevel();

        if (result == D3DERR_DEVICELOST)
        {
            OnDeviceLost();
            return E_FAIL;
        }
        else if (result == D3DERR_DRIVERINTERNALERROR)
        {
            MessageBox(nullptr, L"Internal Driver Error... Quitting Program.", nullptr, NULL);
            return E_UNEXPECTED;
        }
        else if (result == D3DERR_DEVICENOTRESET)
        {
            OnDeviceAvailable();
            return E_FAIL;
        }
        else
            return S_OK;

        return E_NOTIMPL;
    }
    //>Present Params Setup
    void D3D9Renderer::SetupDeviceConfiguration()
    {
        D3DPRESENT_PARAMETERS params;
        ZeroMemory(&params, sizeof(params));

        params.BackBufferWidth = SCREEN_WIDTH;
        params.BackBufferHeight = SCREEN_HEIGHT;
        params.BackBufferFormat = D3DFMT_X8R8G8B8;
        params.BackBufferCount = 1;

        DWORD quality;
        //Multi-Sampling sanity check
        if (CheckMultiSampleSupport(D3DMULTISAMPLE_8_SAMPLES, &quality, true) == S_OK)
        {
            params.MultiSampleType = D3DMULTISAMPLE_8_SAMPLES;
            params.MultiSampleQuality = 0; //quality 1 not supported
            params.SwapEffect = D3DSWAPEFFECT_DISCARD;
        }
        else
        {
            params.MultiSampleType = D3DMULTISAMPLE_NONE;
            params.MultiSampleQuality = 0;
            params.SwapEffect = D3DSWAPEFFECT_DISCARD; //just in case we want something else when Multi-Sampling is off.
        }

        params.hDeviceWindow = m_hWindow;
        params.Windowed = true;
        params.EnableAutoDepthStencil = true; //for now let's just renderer take care of this
        params.AutoDepthStencilFormat = D3DFMT_D24S8;
        params.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
        params.Flags = NULL;
        params.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

        m_device->SetDeviceCharateristics(params);

        ComResult(CreateD3DDevice(&params));
    }
    void D3D9Renderer::SetupVertexDeclaration()
    {
        constexpr int32_t defaultVal = 0;

        D3DVERTEXELEMENT9 positionVertexElements[] =
        {
            { defaultVal, defaultVal, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
            { defaultVal, sizeof(float) * 3, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
            { defaultVal, sizeof(float) * 2, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
            D3DDECL_END()
        };

        ComResult(m_device->CreateVertexDeclaration(positionVertexElements, &m_vertexDeclarations.positionVertexDecl));
        assert(m_vertexDeclarations.positionVertexDecl);

        m_device->SetVertexDeclaration(m_vertexDeclarations.positionVertexDecl);
    }
    void D3D9Renderer::BuildMatrices()
    {
        m_viewMat = m_camera.GetViewMatrix();
        auto aspectRatio = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);
        //>Projection Matrix
        D3DXMatrixPerspectiveFovLH(&m_projMat, D3DXToRadian(45), aspectRatio, 1.0f, 100000.0f);
        //>World Matrix
        D3DXMatrixIdentity(&m_worldMat);

        m_device->SetTransform(D3DTS_VIEW, m_viewMat* m_worldMat);
        m_device->SetTransform(D3DTS_PROJECTION, m_projMat);
        m_device->SetTransform(D3DTS_WORLD, m_worldMat);
    }
    void D3D9Renderer::UpdateMatrices()
    {
        m_camera.HandleCameraInput();
        m_viewMat = m_camera.GetViewMatrix();
        m_device->SetTransform(D3DTS_VIEW, m_viewMat);
        m_device->SetTransform(D3DTS_PROJECTION, m_projMat);
        m_device->SetTransform(D3DTS_WORLD, m_worldMat);
    }
    void D3D9Renderer::RenderEffect(LightingMode mode, UINT numVertices, UINT startIndex, UINT primitiveCount, UINT matIndex)
    {
        UINT numPasses(0);
        switch (mode)
        {
        case LightingMode::Specular:
        {
            ComResult(m_effect->SetTechnique("BlinnPhongTech"));
            ComResult(m_effect->Begin(&numPasses, NULL));
            
            auto material = m_modelManager.GetModel()->GetMaterialAtIndex(matIndex);
            auto tex = material->GetTextureOfType(Material::TextureType::Diffuse);

            for (UINT passItr = 0; passItr < numPasses; ++passItr)
            {
                m_effect->BeginPass(passItr);
                m_effect->SetTexture("g_DiffuseTex", tex);
                m_effect->SetMatrix("g_WorldMat", &m_worldMat);
                m_effect->SetMatrix("g_worldViewProjMatrix", &m_worldViewProjMat);
                m_effect->SetVector("g_dirLightDir", &D3DXVECTOR4(0.0f, 1.0f, -2.2f, 1.0f));
                m_effect->SetVector("g_dirLightColor", &D3DXVECTOR4(0.3f, 0.3f, 0.3f, 1.0f));
                m_effect->SetVector("g_ambientLight", &D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f));
                m_effect->SetVector("g_viewDirection", &D3DXVECTOR4(m_camera.GetCamPosition(), 1.0f));
                m_effect->SetVector("g_specularLightColor", &D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f));
                m_effect->SetFloat("g_specIntensity", 20.0f);

                m_effect->CommitChanges();
                m_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, numVertices, startIndex, primitiveCount);
                m_effect->EndPass();
            }
            m_effect->End();
        }
        break;
        default:
            break;
        }
    }
    void D3D9Renderer::OnDeviceLost()
    {
        Sleep(200);
    }
    void D3D9Renderer::OnDeviceAvailable()
    {
        m_device->ResetDevice();
    }
    //>Handle this in the mainloop of the game, define "FULLSCREEN" for fullscreen support in the game
    DWORD D3D9Renderer::GetSupportedFeaturesBehavioralFlags() const
    {
        D3DDISPLAYMODE displayMode;
        m_d3d9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode);

        //Checking HAL support
#ifdef FULLSCREEN
        ComResult(m_d3d9->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8, false));
#else
        ComResult(m_d3d9->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, displayMode.Format, displayMode.Format, true));
#endif
        //Checking for HW Vertex Processing
        DWORD flags = 0;
        if (m_d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
        {
            flags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
        }
        return flags;
    }
    HRESULT D3D9Renderer::CheckMultiSampleSupport(const D3DMULTISAMPLE_TYPE type, DWORD * quality, const bool isWindowed) const
    {
        HRESULT result = m_d3d9->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_D24S8, isWindowed, type, quality);
        return result;
    }
    bool D3D9Renderer::CheckShaderVersionSupport(int16_t version) const
    {
        if (m_d3dCaps.VertexShaderVersion < D3DVS_VERSION(version, 0) || m_d3dCaps.PixelShaderVersion < D3DPS_VERSION(version, 0))
            return false;

        return true;
    }
    HRESULT D3D9Renderer::CreateD3DDevice(D3DPRESENT_PARAMETERS * d3dpp)
    {
        assert(m_hWindow != nullptr);

        HRESULT result = m_d3d9->CreateDevice(D3DADAPTER_DEFAULT,
            D3DDEVTYPE_HAL,
            m_hWindow,
            GetSupportedFeaturesBehavioralFlags(),
            d3dpp,
            m_device->GetDeviceObjectRef());
        return result;
    }
    void D3D9Renderer::ParseModels()
    {
		std::string filename = "data/Content/Sponza.fbx";
		m_modelManager.AddModelToWorld(m_device->GetDeviceObject(), filename);
    }
    void D3D9Renderer::SetupStaticBuffers()
    {
        ComResult(m_device->CreateVertexBuffer((sizeof(PositionVertex) * m_modelManager.GetVBufferCount()), NULL, NULL, D3DPOOL_MANAGED, m_vBuffer, nullptr));
        ComResult(m_device->CreateIndexBuffer(m_modelManager.GetIBufferCount() * sizeof(uint32_t), NULL, D3DFMT_INDEX32, D3DPOOL_MANAGED, m_iBuffer, nullptr));

        m_vBuffer.AddDataToBuffer(m_modelManager.GetVertexBufferData().data(), NULL, sizeof(PositionVertex) * m_modelManager.GetVBufferCount());
        m_iBuffer.AddDataToBuffer(m_modelManager.GetIndexBufferData().data(), NULL, sizeof(uint32_t) * m_modelManager.GetIBufferCount());
    }
    void D3D9Renderer::SetupEffect(LightingMode mode)
    {
        ID3DXBuffer* errorBuffer = nullptr;

        switch (mode) 
        {
        case LightingMode::Diffuse:
            ComResult(D3DXCreateEffectFromFileA(m_device->GetDeviceObject(),
                "source/renderer/d3d9/shaders/Lambert.hlsl", nullptr, nullptr,
                D3DXSHADER_DEBUG, nullptr, &m_effect, &errorBuffer));
            break;
        case LightingMode::Specular:
            ComResult(D3DXCreateEffectFromFileA(m_device->GetDeviceObject(),
                "source/renderer/d3d9/shaders/BlinnPhong.hlsl", nullptr, nullptr,
                D3DXSHADER_DEBUG, nullptr, &m_effect, &errorBuffer));
            break;
        default:
            break;
        }

        if (errorBuffer)
        {
            MessageBoxA(0, (char*)errorBuffer->GetBufferPointer(), 0, 0);
            Logger::GetInstance().LogInfo((char*)errorBuffer->GetBufferPointer());
        }
    }
}