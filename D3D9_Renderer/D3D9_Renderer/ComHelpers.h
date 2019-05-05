#pragma once
namespace renderer 
{
    constexpr void ComSafeRelease(IUnknown* comPtr)
    {
		if (comPtr)
		{
			comPtr->Release();
			comPtr = nullptr;
		}
    }
}