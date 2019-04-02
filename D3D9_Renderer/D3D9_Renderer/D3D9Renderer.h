#pragma once
#include "GfxRendererBase.h"
#include <d3d9.h>

namespace d3dgfx
{
	class D3D9Renderer :
		public GfxRendererBase
	{
	public:
		D3D9Renderer();
		virtual ~D3D9Renderer();

		D3D9Renderer(const D3D9Renderer&) = delete;
		D3D9Renderer& operator=(const D3D9Renderer&) = delete;

		void Init() override;
		void UnInit() override;
		void RenderFrame() override;
		bool CheckDeviceStatus() override;

	private:
		IDirect3D9* m_d3d9;
	};
}

