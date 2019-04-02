#pragma once

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

		virtual void Init() = 0;
		virtual void UnInit() = 0;
		virtual void RenderFrame() = 0;
		virtual bool CheckDeviceStatus() = 0;
		virtual void OnDeviceLost() = 0;
		virtual void OnDeviceReset() = 0;
	};
}
