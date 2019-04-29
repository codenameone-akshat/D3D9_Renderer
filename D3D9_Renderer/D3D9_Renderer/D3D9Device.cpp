#include <cassert>

#include "D3D9Device.h"

namespace d3dgfx
{
	D3D9Device::D3D9Device()
		:m_d3dDevice(nullptr)
	{
		ZeroMemory(&m_d3dPresentParams, sizeof(m_d3dPresentParams));
	}

	D3D9Device::~D3D9Device()
	{
        ComSafeRelease(m_d3dDevice);
	}

	HRESULT D3D9Device::CheckCoorparativeLevel() const
	{
		assert(m_d3dDevice);
		return m_d3dDevice->TestCooperativeLevel();
	}

	HRESULT D3D9Device::CreateVertexBuffer(UINT length, DWORD usage, DWORD FVF, D3DPOOL pool, StaticBuffer<IDirect3DVertexBuffer9> vertexBuffer, HANDLE* pSharedHandle)
	{
		assert(m_d3dDevice);
		HRESULT result = m_d3dDevice->CreateVertexBuffer(length, usage, FVF, pool, &vertexBuffer, pSharedHandle);
		return result;
	}

	HRESULT D3D9Device::CreateIndexBuffer(UINT length, DWORD usage, D3DFORMAT format, D3DPOOL pool, StaticBuffer<IDirect3DIndexBuffer9> indexBuffer, HANDLE* pSharedHandle)
	{
		assert(m_d3dDevice);
		HRESULT result = m_d3dDevice->CreateIndexBuffer(length, usage, format, pool, &indexBuffer, pSharedHandle);
		return result;
	}
    HRESULT D3D9Device::CreateVertexDeclaration(const D3DVERTEXELEMENT9* vElement, IDirect3DVertexDeclaration9 ** vDecl)
    {
        assert(m_d3dDevice);
        HRESULT result = m_d3dDevice->CreateVertexDeclaration(vElement, vDecl);
        return result;
    }
}
