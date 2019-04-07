#pragma once
#include <d3d9.h>
class D3D9Device
{
public:
	D3D9Device();
	~D3D9Device();

    D3D9Device(D3D9Device&&) = default;

    inline void SetDeviceCharateristics(D3DPRESENT_PARAMETERS presentParams) { memcpy(&m_d3dPresentParams, &presentParams, sizeof(D3DPRESENT_PARAMETERS)); }

	//>Getters
	inline IDirect3DDevice9* GetDeviceObject() const { return m_d3dDevice; }
	inline IDirect3DDevice9** GetDeviceObjectRef() { return &m_d3dDevice; }
	inline void SetDeviceObject(IDirect3DDevice9* device) { m_d3dDevice = device; }

	HRESULT CheckCoorparativeLevel();
private:
    IDirect3DDevice9* m_d3dDevice;
    D3DPRESENT_PARAMETERS m_d3dPresentParams;
};

