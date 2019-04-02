#include "D3D9Renderer.h"
namespace d3dgfx
{
	D3D9Renderer::D3D9Renderer()
		:m_d3d9(Direct3DCreate9(D3D_SDK_VERSION))
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
}
