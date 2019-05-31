#pragma once

#include <memory>

#include "../window/Window.h"
#include "../renderer/d3d9/D3D9Renderer.h"

namespace renderer
{
    class Time;

	class EngineCore
	{
	public:
		EngineCore();
		~EngineCore();

		EngineCore(EngineCore& engineCore) = delete;
		EngineCore& operator=(EngineCore engineCore) = delete;

		void InitEngineCreateWindow(HINSTANCE hInstance, int32_t nShowCmd);
		void InitRenderer();
		void RenderFrame();
		void PollMessage();

	private:
		std::unique_ptr<Window> m_window;
        std::unique_ptr<Time> m_timer;
        std::unique_ptr<D3D9Renderer> m_renderer;
	};
}