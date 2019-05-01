#pragma once
#include <d3d9.h>
#include <memory>

namespace renderer
{
	constexpr int FullBufferLock = 0;

    template<typename T, typename = typename std::enable_if_t<std::is_same<T, IDirect3DVertexBuffer9>::value || std::is_same<T, IDirect3DIndexBuffer9>::value>>
    class StaticBuffer
    {
    public:
		StaticBuffer()
			:m_buffer(nullptr)
		{
		}
		~StaticBuffer() 
		{
			if (m_buffer)
			{
				m_buffer->Release();
				m_buffer = nullptr;
			}
		}
        
        T* GetRawPtr() { return m_buffer; }
        T** operator&() { return &m_buffer; }

        inline constexpr auto GetBufferDesc() const { return m_bufferDesc.GetBufferDesc(); }

		void AddDataToBuffer(void* data, DWORD lockFlags, size_t dataSize)
		{
            void* bufferData;
            Lock(FullBufferLock, FullBufferLock, &bufferData, lockFlags);
            memcpy(bufferData, data, dataSize);
            Unlock();
		}

	private:
		
		template<typename U>
		struct StaticBufferDesc
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

        inline void Lock(UINT offsetToLock, UINT sizeToLock, void** ppbufferData, DWORD flags) 
        {
            m_buffer->Lock(offsetToLock, sizeToLock, ppbufferData, flags);
        }

		inline void Unlock() { m_buffer->Unlock(); }

        T* m_buffer;
        StaticBufferDesc<T> m_bufferDesc;
    };
}   