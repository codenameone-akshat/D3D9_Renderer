#pragma once

#pragma comment (lib, "d3dx9")
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "dxerr.lib")

#include <d3d9.h>
#include <memory>
#include <vector>

#include "../GfxRendererBase.h"
#include "D3D9Device.h"
#include "StaticBuffer.hpp"
#include "../Model.h"
#include "VertexDefs.h"
#include "../Camera.h"
#include "../../enginecore/ModelManager.h"
#include"../../enginecore/Batch.h"

constexpr int16_t SHADER_VERSION = 3;
constexpr auto SCREEN_HEIGHT = 720;
constexpr auto SCREEN_WIDTH = 1280;

namespace renderer
{
	//>Singleton Class
	class D3D9Renderer :
		public GfxRendererBase
	{
		using super = GfxRendererBase;

	private:
		D3D9Renderer();
	public:
		virtual ~D3D9Renderer();

		D3D9Renderer(const D3D9Renderer&) = delete;
		D3D9Renderer& operator=(const D3D9Renderer&) = delete;

		//>Overrides
        void Init(HWND hWindow) override;
		void UnInit() override;
		void PrepareForRendering() override;
		void PreRender() override;
		void RenderFrame() override;
		void PostRender() override;
  
        //>Lighting Modes
        enum class LightingMode
        {
            Diffuse,
            Specular
        };

		//>Events
		void OnDeviceLost() override;
        void OnDeviceAvailable() override;
		
        //>Checks for Sanity and Support
        [[nodiscard]] HRESULT CheckDeviceStatus() override;
        [[nodiscard]] DWORD GetSupportedFeaturesBehavioralFlags() const;
		[[nodiscard]] HRESULT CheckMultiSampleSupport(const D3DMULTISAMPLE_TYPE type, DWORD* quality, const bool isWindowed) const;
		[[nodiscard]] bool CheckShaderVersionSupport(int16_t version) const;
		
        [[nodiscard]] HRESULT CreateD3DDevice(D3DPRESENT_PARAMETERS * d3dpp);
        
        void ParseModels();
        void SetupStaticBuffers();
        void SetupEffect(LightingMode mode);

		[[maybe_unused]] inline static D3D9Renderer& GetInstance() //>Meyers' Singleton
        {
            static D3D9Renderer instance;
            return instance;
        }

	private:
        void SetupDeviceConfiguration();
        void SetupVertexDeclaration();
		void BuildMatrices();
		void UpdateMatrices();
        void RenderEffect(LightingMode mode, UINT numVertices, UINT startIndex, UINT primitiveCount);

		int32_t m_vBufferVertexCount;
		int32_t m_iBufferIndexCount;
		int32_t m_primitiveCount;

		D3DCAPS9 m_d3dCaps;
		IDirect3D9* m_d3d9;
		ID3DXEffect* m_effect;
		
		StaticBuffer<IDirect3DVertexBuffer9> m_vBuffer;
		StaticBuffer<IDirect3DIndexBuffer9> m_iBuffer;

        VertexDeclContainer m_vertexDeclarations;
		Camera m_camera;

		D3DXMATRIX m_viewMat;
		D3DXMATRIX m_projMat;
        D3DXMATRIX m_worldMat;
        D3DXMATRIX m_worldViewProjMat;

		std::unique_ptr<D3D9Device> m_device;
		HWND m_hWindow;
        ModelManager m_modelManager;
	};
}