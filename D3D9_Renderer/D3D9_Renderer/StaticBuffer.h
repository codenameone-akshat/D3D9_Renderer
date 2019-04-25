#pragma once
#include <d3d9.h>

namespace d3dgfx
{
    template<typename T>
    class StaticBuffer
    {
    public:
        StaticBuffer();
        ~StaticBuffer();
        
    private:
        inline void Lock() { buffer->Lock(); }
        inline void Unlock() { buffer->Unlock(); }

        T * buffer;
    };
}