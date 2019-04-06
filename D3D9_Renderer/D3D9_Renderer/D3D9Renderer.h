#pragma once
#include "GfxRendererBase.h"
#include "D3D9Device.h"
#include <d3d9.h>
#include <memory>
#pragma comment (lib, "d3d9.lib")

#define SCREEN_HEIGHT 720
#define SCREEN_WIDTH 1280

namespace d3dgfx
{
    //>Singleton Class
	class D3D9Renderer :
		public GfxRendererBase
	{
		D3D9Renderer();
	public:
		virtual ~D3D9Renderer();

		D3D9Renderer(const D3D9Renderer&) = delete;
		D3D9Renderer& operator=(const D3D9Renderer&) = delete;

		//>Overrides
        void Init(HWND hWindow) override;
		void UnInit() override;
		void RenderFrame() override;
  
		//>Events
		void OnDeviceLost() override;
        void OnDeviceReset() override;
        
		//>Checks for Sanity and Support
        [[nodiscard]] HRESULT CheckDeviceStatus() override;
        [[nodiscard]] DWORD GetSupportedFeaturesBehavioralFlags();
		[[nodiscard]] HRESULT CheckMultiSampleSupport(D3DMULTISAMPLE_TYPE type, DWORD* quality, bool isWindowed) const;
		[[nodiscard]] HRESULT CreateD3DDevice(D3DPRESENT_PARAMETERS * d3dpp);

        inline static D3D9Renderer& GetInstance() //>Meyers' Singleton | @GD: Should I use std::unique_ptr<> instead??
        {
            static D3D9Renderer instance;
            return instance;
        }

	private:
		void SetupDeviceConfiguration();

		IDirect3D9* m_d3d9;
		std::unique_ptr<D3D9Device> m_device;
		HWND m_hWindow;
	};
}

