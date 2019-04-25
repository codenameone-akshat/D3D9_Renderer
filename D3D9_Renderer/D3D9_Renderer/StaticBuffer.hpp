#pragma once
#include <d3d9.h>
#include <memory>

namespace d3dgfx
{
	constexpr int FullBufferLock = 0;

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
		
		inline T* GetRawBuffer() const { return m_buffer; }
		inline T** GetPtrToRawBuffer() { return &m_buffer; }

		void AddDataToBuffer(void* data)
		{
		}

	private:
		inline void Lock() { m_buffer->Lock(); }
		inline void Unlock() { m_buffer->Unlock(); }

        T* m_buffer;
    };
}