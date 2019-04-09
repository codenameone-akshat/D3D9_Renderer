#include "EngineCore.h"
#include "D3D9Renderer.h"
#include "Time.h"

#include <chrono>

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
	}

	void EngineCore::PollMessage()
	{
		MSG msg;

		auto& renderer = D3D9Renderer::GetInstance();
		renderer.Init(m_window->GetHandleToWindow());

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
			
			renderer.RenderFrame();
			
            m_timer->EndTimer();
		}
	}
}