#pragma once
#include "GfxRendererBase.h"
#include "D3D9Device.h"
#include <d3d9.h>

#pragma comment (lib, "d3d9.lib")

namespace d3dgfx
{
    //>Singleton Class
	class D3D9Renderer :
		public GfxRendererBase
	{
	public:
		D3D9Renderer();
		virtual ~D3D9Renderer();

		D3D9Renderer(const D3D9Renderer&) = delete;
		D3D9Renderer& operator=(const D3D9Renderer&) = delete;

		//>Overrides
        void Init() override;
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

		void SetupDeviceConfiguration();

        inline D3D9Renderer& GetInstance() const //>Meyers' Singleton
        {
            static D3D9Renderer instance;
            return instance;
        }
	private:
		IDirect3D9* m_d3d9;
        D3D9Device* m_device;
	};
}

