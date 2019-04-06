#pragma once
#include <memory>
#include "Window.h"
namespace d3dgfx
{
	class EngineCore
	{
	public:
		EngineCore();
		~EngineCore();

		EngineCore(EngineCore& engineCore) = delete;
		EngineCore& operator=(EngineCore engineCore) = delete;

		void InitEngineCreateWindow(HINSTANCE hInstance, int nShowCmd);
		void PollMessage();

	private:
		std::unique_ptr<Window> m_window;
	};
}