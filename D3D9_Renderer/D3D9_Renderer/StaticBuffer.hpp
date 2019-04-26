#pragma once
#define NOMINMAX
#include <d3d9.h>
#include <memory>

namespace d3dgfx
{
	constexpr int FullBufferLock = 0;

    template<typename U>
    class StaticBufferDesc
    {
        template<typename U, typename = typename std::enable_if_t<std::is_same<U, IDirect3DVertexBuffer9>::value>>
        D3DVERTEXBUFFER_DESC GetBufferDesc()
        {
            D3DVERTEXBUFFER_DESC bufferDesc;
            return bufferDesc;
        }

        template<typename U, typename = typename std::enable_if_t<std::is_same<U, IDirect3DIndexBuffer9>::value>>
        D3DINDEXBUFFER_DESC GetBufferDesc() 
        {
            D3DINDEXBUFFER_DESC bufferDesc;
            return bufferDesc;
        }
    };

    template<typename T>
    class StaticBuffer
    {
    public:
		StaticBuffer()
			:m_buffer(nullptr)
		{
		}
		~StaticBuffer() 
		{
			m_buffer = nullptr;
		}
        
        T* GetRawPtr() { return m_buffer; }
        T** operator&() { return &m_buffer; }

        inline constexpr auto GetBufferDesc() const { return m_bufferDesc.GetBufferDesc(); }

		void AddDataToBuffer(void* data)
		{
            //stub
		}

	private:
        inline void Lock(UINT offsetToLock, UINT sizeToLock, BYTE** ppbufferData, DWORD flags) 
        {
            m_buffer->Lock(offsetToLock, sizeToLock, ppbufferData, flags);
        }

		inline void Unlock() { m_buffer->Unlock(); }

        T* m_buffer;
        StaticBufferDesc<T> m_bufferDesc;
    };
}   