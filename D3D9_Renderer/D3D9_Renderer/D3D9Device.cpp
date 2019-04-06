#include "D3D9Device.h"

D3D9Device::D3D9Device()
	:m_d3dPresentParams()
{
	ZeroMemory(m_d3dDevice, sizeof(m_d3dDevice));
}

D3D9Device::~D3D9Device()
{
}