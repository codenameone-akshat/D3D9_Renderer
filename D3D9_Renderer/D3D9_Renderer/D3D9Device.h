#pragma once
#include <d3d9.h>
#include <DirectXMath.h>
#include <memory>

#include "ComHelpers.h"
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
		inline IDirect3DDevice9* GetDeviceObject() const { return m_d3dDevice; }  //TODO: overload -> maybe?
		inline IDirect3DDevice9** GetDeviceObjectRef() { return &m_d3dDevice; } //TODO: overload & maybe?
		inline void SetDeviceObject(IDirect3DDevice9* device) { m_d3dDevice = device; }

		inline void ResetDevice() { m_d3dDevice->Reset(&m_d3dPresentParams); }
		inline void Clear(const DWORD count, const D3DRECT* pRects, const DWORD flags, const D3DCOLOR color, const float z, const DWORD stencil) const { m_d3dDevice->Clear(count, pRects, flags, color, z, stencil); }
        inline void Release() { ComSafeRelease(m_d3dDevice); }
		inline void BeginScene() const { m_d3dDevice->BeginScene(); }
		inline void EndScene() const { m_d3dDevice->EndScene(); }
		inline void Present(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion) const { m_d3dDevice->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion); }

		[[nodiscard]] HRESULT CheckCoorparativeLevel() const;
		
        //>Buffers
        [[nodiscard]] HRESULT CreateVertexBuffer(UINT length, DWORD usage, DWORD FVF, D3DPOOL pool, StaticBuffer<IDirect3DVertexBuffer9>& vertexBuffer, HANDLE* pSharedHandle);
		[[nodiscard]] HRESULT CreateIndexBuffer(UINT length, DWORD usage, D3DFORMAT format, D3DPOOL pool, StaticBuffer<IDirect3DIndexBuffer9>& indexBuffer, HANDLE* pSharedHandle);
        [[nodiscard]] HRESULT CreateVertexDeclaration(const D3DVERTEXELEMENT9* vElement, IDirect3DVertexDeclaration9** vDecl);
        inline HRESULT SetVertexDeclaration(IDirect3DVertexDeclaration9* decl)
        {
			m_d3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

            auto result = m_d3dDevice->SetVertexDeclaration(decl);
            return result;
        }
        inline HRESULT SetStreamSource(UINT streamNumber, StaticBuffer<IDirect3DVertexBuffer9>& vBuffer, UINT offsetInBytes, UINT stride)
        {
            auto result = m_d3dDevice->SetStreamSource(streamNumber, vBuffer.GetRawPtr(), offsetInBytes, stride);
            return result;
        }
        inline HRESULT SetIndices(StaticBuffer<IDirect3DIndexBuffer9>& indexBuffer)
        {
            auto result = m_d3dDevice->SetIndices(indexBuffer.GetRawPtr());
            return result;
        }
		inline void SetFVF(int fvf) { m_d3dDevice->SetFVF(fvf); }

        //>Drawing
        inline HRESULT DrawPrimitive(D3DPRIMITIVETYPE primitiveType, UINT startVertex, UINT primitiveCount) 
        {
            auto result = m_d3dDevice->DrawPrimitive(primitiveType, startVertex, primitiveCount);  
            return result;
        }
        inline HRESULT DrawIndexedPrimitive(D3DPRIMITIVETYPE primitiveType, INT baseVertexIndex, UINT minIndex, UINT numVertices, UINT startIndex, UINT primitiveCount) 
        {
            auto result = m_d3dDevice->DrawIndexedPrimitive(primitiveType, baseVertexIndex, minIndex, numVertices, startIndex, primitiveCount);
            return result;
        }
		inline HRESULT SetTransform(D3DTRANSFORMSTATETYPE transStateType, DirectX::XMMATRIX transMatrix)
		{
			DirectX::XMFLOAT4X4 dxcast_matrix;
			DirectX::XMStoreFloat4x4(&dxcast_matrix, transMatrix);

			auto result = m_d3dDevice->SetTransform(transStateType, reinterpret_cast<D3DMATRIX*>(&dxcast_matrix));
			return result;
		}


		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetRenderStates()
		{
			m_d3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
			m_d3dDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(80, 80, 80));
			m_d3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			m_d3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
			m_d3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
			
			D3DLIGHT9 light;
			D3DMATERIAL9 mat;

			ZeroMemory(&light, sizeof(light));
			light.Type = D3DLIGHT_DIRECTIONAL;
			light.Diffuse = { 1.0f, 0.0f, 0.0f, 1.0f };
			light.Direction = { -1.0f, -0.3f, 1.0f };
			m_d3dDevice->SetLight(0, &light);
			m_d3dDevice->LightEnable(0, TRUE);

			ZeroMemory(&mat, sizeof(mat));
			mat.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
			mat.Ambient = { 1.0f, 1.0f, 1.0f, 1.0f };

			m_d3dDevice->SetMaterial(&mat);
		}

	private:
		IDirect3DDevice9* m_d3dDevice;
		D3DPRESENT_PARAMETERS m_d3dPresentParams;
	};
}