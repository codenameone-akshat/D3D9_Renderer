#include "D3D9Device.h"

D3D9Device::D3D9Device()
    :m_d3dDevice(nullptr)
{
}

D3D9Device::~D3D9Device()
{
}

HRESULT D3D9Device::CreateD3DDevice(D3DPRESENT_PARAMETERS d3dpp)
{
    return E_NOTIMPL;
}
