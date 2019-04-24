#include "D3D9Device.h"
#include <cassert>
namespace d3dgfx
{
	D3D9Device::D3D9Device()
		:m_d3dDevice(nullptr)
	{
		ZeroMemory(&m_d3dPresentParams, sizeof(m_d3dPresentParams));
	}

	D3D9Device::~D3D9Device()
	{
	}

	HRESULT D3D9Device::CheckCoorparativeLevel() const
	{
		assert(m_d3dDevice);
		return m_d3dDevice->TestCooperativeLevel();
	}
}
