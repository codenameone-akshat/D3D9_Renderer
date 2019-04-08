#include "D3D9Renderer.h"
#include <cassert>
#include <windows.h>

namespace d3dgfx
{
	D3D9Renderer::D3D9Renderer()
		:m_d3d9(Direct3DCreate9(D3D_SDK_VERSION)),
		m_device(std::make_unique<D3D9Device>()),
		m_hWindow()
	{
	}
	D3D9Renderer::~D3D9Renderer()
	{
		m_d3d9 = nullptr;
	}
	void D3D9Renderer::Init(HWND hWindow)
	{
		memcpy(&m_hWindow, &hWindow, sizeof(HWND)); //@GD is this fine?
		SetupDeviceConfiguration();
	}
	void D3D9Renderer::UnInit()
	{
		if (m_d3d9)
		{
			m_d3d9->Release();
            m_device->Release();
		}
	}
	void D3D9Renderer::RenderFrame()
	{
		for (unsigned long long i = 0; i < 99999999 ; i++)
		{
			long double j = 100000.0;
			long double k = j / 1500;
		}

        HRESULT result = CheckDeviceStatus();
        if (result != S_OK)
            return;

        m_device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(120, 120, 120), 1.0f, 0);
        m_device->BeginScene();
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
            MessageBox(nullptr, L"Interal Driver Error... Quitting Program.", nullptr, NULL);
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
		params.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

		m_device->SetDeviceCharateristics(params);

		auto result = CreateD3DDevice(&params);
		assert(result == S_OK);
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
	DWORD D3D9Renderer::GetSupportedFeaturesBehavioralFlags()
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
	HRESULT D3D9Renderer::CheckMultiSampleSupport(D3DMULTISAMPLE_TYPE type, DWORD * quality, bool isWindowed) const
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
}
