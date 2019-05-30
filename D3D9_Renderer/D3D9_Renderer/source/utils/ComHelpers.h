#pragma once

#include <DxErr.h>
#include<d3d9.h>

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

	constexpr void ComResult(HRESULT result)
	{
		if (result != S_OK)
		{
			DXTrace(__FILE__, __LINE__, result, L"Error", TRUE);
		}
	}
}