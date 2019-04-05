#pragma once
#include <d3d9.h>
class D3D9Device
{
public:
	D3D9Device();
	~D3D9Device();

    D3D9Device(D3D9Device&&) = default;

    HRESULT CreateD3DDevice(D3DPRESENT_PARAMETERS d3dpp);
    inline void SetDeviceCharateristics(D3DPRESENT_PARAMETERS presentParams) { m_d3dPresentParams = presentParams; }

private:
    IDirect3DDevice9* m_d3dDevice;
    D3DPRESENT_PARAMETERS m_d3dPresentParams;
};

