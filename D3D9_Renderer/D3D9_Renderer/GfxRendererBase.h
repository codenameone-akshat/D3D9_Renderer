#pragma once
#include <windows.h>
namespace d3dgfx
{
	class GfxRendererBase
	{
	public:
		GfxRendererBase();
		virtual ~GfxRendererBase();

		//prevent copies of the renderer
		GfxRendererBase(const GfxRendererBase&) = delete;
		GfxRendererBase& operator=(const GfxRendererBase&) = delete;

		virtual void Init(HWND hWindow) = 0;
		virtual void UnInit() = 0;
		virtual void RenderFrame() = 0;
		
		//>Events
		virtual void OnDeviceLost() = 0;
		virtual void OnDeviceAvailable() = 0;

		//>Device Handler | long <-> HRESULT
		[[nodiscard]] virtual long CheckDeviceStatus() = 0;
	};
}
