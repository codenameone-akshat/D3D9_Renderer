#pragma once
namespace d3dgfx 
{
    constexpr void ComSafeRelease(IUnknown* comPtr)
    {
        if (comPtr)
            comPtr->Release();
        comPtr = nullptr;
    }
}