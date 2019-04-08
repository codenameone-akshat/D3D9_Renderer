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

    inline void ResetDevice() { m_d3dDevice->Reset(&m_d3dPresentParams); }
    inline void Clear(DWORD count, const D3DRECT* pRects, DWORD flags, D3DCOLOR color, float z, DWORD stencil) { m_d3dDevice->Clear(count, pRects, flags, color, z, stencil); }
    inline void Release() { m_d3dDevice->Release(); }
    inline void BeginScene() { m_d3dDevice->BeginScene(); }
    inline void EndScene() { m_d3dDevice->EndScene(); }
    inline void Present(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion) { m_d3dDevice->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion); }

	HRESULT CheckCoorparativeLevel();
private:
    IDirect3DDevice9* m_d3dDevice;
    D3DPRESENT_PARAMETERS m_d3dPresentParams;
};

