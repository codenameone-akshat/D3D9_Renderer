#pragma once
#include <memory>
#include "Window.h"

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

		void InitEngineCreateWindow(HINSTANCE hInstance, int nShowCmd);
		void InitRenderer();
		void RenderFrame();
		void PollMessage();

	private:
		std::unique_ptr<Window> m_window;
        std::unique_ptr<Time> m_timer;
	};
}