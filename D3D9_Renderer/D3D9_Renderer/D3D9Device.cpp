#include "D3D9Device.h"

D3D9Device::D3D9Device()
	:m_d3dDevice(nullptr)
{
	ZeroMemory(&m_d3dPresentParams, sizeof(m_d3dPresentParams));
}

D3D9Device::~D3D9Device()
{
}