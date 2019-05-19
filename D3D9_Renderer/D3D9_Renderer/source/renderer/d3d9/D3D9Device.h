#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <memory>

#include "../../utils/ComHelpers.h"
#include "StaticBuffer.hpp"

namespace renderer
{
	class D3D9Device
	{
	public:
		D3D9Device();
		~D3D9Device();

		D3D9Device(D3D9Device&&) = default;

		inline void SetDeviceCharateristics(D3DPRESENT_PARAMETERS presentParams) { memcpy(&m_d3dPresentParams, &presentParams, sizeof(D3DPRESENT_PARAMETERS)); }

		//>Getters
		[[maybe_unused]] inline IDirect3DDevice9* GetDeviceObject() const { return m_d3dDevice; }  //TODO: overload -> maybe?
		[[maybe_unused]] inline IDirect3DDevice9** GetDeviceObjectRef() { return &m_d3dDevice; } //TODO: overload & maybe?
		inline void SetDeviceObject(IDirect3DDevice9* device) { m_d3dDevice = device; }

		inline void ResetDevice() { m_d3dDevice->Reset(&m_d3dPresentParams); }
		inline void Clear(const DWORD count, const D3DRECT* pRects, const DWORD flags, const D3DCOLOR color, const float z, const DWORD stencil) const { m_d3dDevice->Clear(count, pRects, flags, color, z, stencil); }
		inline void BeginScene() const { m_d3dDevice->BeginScene(); }
		inline void EndScene() const { m_d3dDevice->EndScene(); }
		inline void Present(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion) const { m_d3dDevice->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion); }

		[[nodiscard]] HRESULT CheckCoorparativeLevel() const;
		
        //>Buffers
        [[nodiscard]] HRESULT CreateVertexBuffer(UINT length, DWORD usage, DWORD FVF, D3DPOOL pool, StaticBuffer<IDirect3DVertexBuffer9>& vertexBuffer, HANDLE* pSharedHandle);
		[[nodiscard]] HRESULT CreateIndexBuffer(UINT length, DWORD usage, D3DFORMAT format, D3DPOOL pool, StaticBuffer<IDirect3DIndexBuffer9>& indexBuffer, HANDLE* pSharedHandle);
        [[nodiscard]] HRESULT CreateVertexDeclaration(const D3DVERTEXELEMENT9* vElement, IDirect3DVertexDeclaration9** vDecl);
		[[maybe_unused]] inline HRESULT SetVertexDeclaration(IDirect3DVertexDeclaration9* decl)
        {
			m_d3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

            auto result = m_d3dDevice->SetVertexDeclaration(decl);
            return result;
        }
		[[maybe_unused]] inline HRESULT SetStreamSource(UINT streamNumber, StaticBuffer<IDirect3DVertexBuffer9>& vBuffer, UINT offsetInBytes, UINT stride)
        {
            auto result = m_d3dDevice->SetStreamSource(streamNumber, vBuffer.GetRawPtr(), offsetInBytes, stride);
            return result;
        }
		[[maybe_unused]] inline HRESULT SetIndices(StaticBuffer<IDirect3DIndexBuffer9>& indexBuffer)
        {
            auto result = m_d3dDevice->SetIndices(indexBuffer.GetRawPtr());
            return result;
        }
		inline void SetFVF(int fvf) { m_d3dDevice->SetFVF(fvf); }

        //>Drawing
		[[maybe_unused]] inline HRESULT DrawPrimitive(D3DPRIMITIVETYPE primitiveType, UINT startVertex, UINT primitiveCount)
        {
            auto result = m_d3dDevice->DrawPrimitive(primitiveType, startVertex, primitiveCount);  
            return result;
        }
		[[maybe_unused]] inline HRESULT DrawIndexedPrimitive(D3DPRIMITIVETYPE primitiveType, INT baseVertexIndex, UINT minIndex, UINT numVertices, UINT startIndex, UINT primitiveCount)
        {
            auto result = m_d3dDevice->DrawIndexedPrimitive(primitiveType, baseVertexIndex, minIndex, numVertices, startIndex, primitiveCount);
            return result;
        }
		[[maybe_unused]] inline HRESULT SetTransform(D3DTRANSFORMSTATETYPE transStateType, D3DXMATRIX transMatrix)
		{
			auto result = m_d3dDevice->SetTransform(transStateType, &transMatrix);
			return result;
		}

	private:
		IDirect3DDevice9* m_d3dDevice;
		D3DPRESENT_PARAMETERS m_d3dPresentParams;
	};
}