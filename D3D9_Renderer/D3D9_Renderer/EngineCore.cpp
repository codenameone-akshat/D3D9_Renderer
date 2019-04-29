#include "EngineCore.h"
#include "D3D9Renderer.h"
#include "Time.h"

#include <chrono>
#include <thread>

namespace d3dgfx
{
	EngineCore::EngineCore()
		:m_window(std::make_unique<Window>()),
        m_timer(std::make_unique<Time>())
	{
	}
	EngineCore::~EngineCore()
	{
	}
	void EngineCore::InitEngineCreateWindow(HINSTANCE hInstance, int nShowCmd)
	{
		const auto windowClassName = L"DX9Window_Class";
		const auto windowName = L"DirectX 9 Window";

		m_window->SetUpAndRegesterWindow(hInstance, windowClassName);

		m_window->CreateD3DWindow(windowClassName,
			windowName,
			WS_OVERLAPPEDWINDOW,
			300,
			300,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			nullptr,
			nullptr,
			hInstance,
			nullptr);
		m_window->ShowD3DWindow(nShowCmd);

		InitRenderer();
	}
	void EngineCore::InitRenderer()
	{
		auto& renderer = D3D9Renderer::GetInstance();
		renderer.Init(m_window->GetHandleToWindow());
	}
	void EngineCore::RenderFrame()
	{
		auto& renderer = D3D9Renderer::GetInstance();
		renderer.RenderFrame();
	}
	void EngineCore::PollMessage()
	{
		MSG msg;
		while (TRUE) //Main Loop
		{
            m_timer->BeginTimer();
			
			while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) //0,0 here represents to take any input possible
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			if (msg.message == WM_QUIT)
				break;
			
			this->RenderFrame();
            
			std::this_thread::sleep_for(std::chrono::microseconds(2));
            m_timer->EndTimer();
		}
	}
}