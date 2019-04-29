#include <cassert>

#include "D3D9Renderer.h"
#include "ComHelpers.h"

namespace d3dgfx
{
	D3D9Renderer::D3D9Renderer()
		:m_d3d9(Direct3DCreate9(D3D_SDK_VERSION)),
		m_device(std::make_shared<D3D9Device>()),
		m_modelList(),
		m_hWindow(),
		m_vBuffer(),
		m_iBuffer(),
        m_vertexDeclarations()
	{
	}
	D3D9Renderer::~D3D9Renderer()
	{
        ComSafeRelease(m_d3d9);
	}
	void D3D9Renderer::Init(HWND hWindow)
	{
		memcpy(&m_hWindow, &hWindow, sizeof(HWND));
		SetupDeviceConfiguration();
		PrepareForRendering();
	}
	void D3D9Renderer::UnInit()
	{
        ComSafeRelease(m_d3d9);
        ComSafeRelease(m_vertexDeclarations.positionVertexDecl);
	}
	void D3D9Renderer::PrepareForRendering()
	{
		ParseModels();
		SetupVertexDeclaration();
		SetupStaticBuffers();
	}
	void D3D9Renderer::PreRender()
	{

	}
	void D3D9Renderer::RenderFrame()
	{
        HRESULT result = CheckDeviceStatus();
        if (result != S_OK)
            return;
        m_device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(50, 0, 200), 1.0f, 0);
        m_device->BeginScene();
        m_device->EndScene();
        m_device->Present(nullptr, nullptr, nullptr, nullptr);
	}
	void D3D9Renderer::PostRender()
	{
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
		if (CheckMultiSampleSupport(D3DMULTISAMPLE_4_SAMPLES, &quality, true) == S_OK)
		{
			params.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
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
		params.EnableAutoDepthStencil = true; //for now let's just dx9 take care of this
		params.AutoDepthStencilFormat = D3DFMT_D24S8;
		params.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		params.Flags = NULL;
		params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		m_device->SetDeviceCharateristics(params);

		auto result = CreateD3DDevice(&params);
		assert(result == S_OK);
	}
    void D3D9Renderer::SetupVertexDeclaration()
    {
        constexpr int defaultVal = 0;
        
        D3DVERTEXELEMENT9 positionVertexElements[] = 
        {
            { defaultVal, defaultVal, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
			D3DDECL_END()
        };

        auto result = m_device->CreateVertexDeclaration(positionVertexElements, &m_vertexDeclarations.positionVertexDecl);
        assert(result == S_OK);
        assert(m_vertexDeclarations.positionVertexDecl);

        m_device->SetVertexDeclaration(m_vertexDeclarations.positionVertexDecl);
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

		HRESULT HALResult = E_NOTIMPL;

		//Checking HAL support
#ifdef FULLSCREEN
		HALResult = m_d3d9->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8, false);
#else
		HALResult = m_d3d9->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, displayMode.Format, displayMode.Format, true);
#endif
		assert(HALResult == S_OK);

		//Checking for HW Vertex Processing
		D3DCAPS9 d3dCaps;
		m_d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps);

		DWORD flags = 0;
		if (d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
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
        std::string filename = "Z:/Projects/D3D9_Renderer/D3D9_Renderer/D3D9_Renderer/data/Cube.FBX"; //get files to load from somewhere else
        std::unique_ptr<Model> model = std::make_unique<Model>(); 
        
		model->LoadModelAndParseData(filename);
        
		m_modelList.push_back(std::move(model));
    }
    void D3D9Renderer::SetupStaticBuffers()
    {
		int vBufferVertexCount = 0;
		int iBufferVertexCount = 0;

		for (auto& model : m_modelList)
		{
			vBufferVertexCount += model->GetTotalVertices();
			iBufferVertexCount += model->GetTotalIndices();
		}

		auto result = m_device->CreateVertexBuffer(vBufferVertexCount * sizeof(PositionVertex), NULL, NULL, D3DPOOL_MANAGED, m_vBuffer, nullptr);
		assert(result == S_OK);

		result = m_device->CreateIndexBuffer(iBufferVertexCount, NULL, D3DFMT_INDEX16, D3DPOOL_DEFAULT, m_iBuffer, nullptr);
		assert(result == S_OK);
	}
}