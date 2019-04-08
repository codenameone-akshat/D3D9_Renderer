#include "EngineCore.h"
#include "D3D9Renderer.h"
#include <chrono>
#include <sstream>

#define LogInfo( s )            \
{                             \
   std::wostringstream os_;    \
   os_ << s << std::endl;                   \
   OutputDebugString(os_.str().c_str());  \
}

namespace d3dgfx
{
	EngineCore::EngineCore()
		:m_window(std::make_unique<Window>()),
		m_time(0.0),
		m_numFrames(0)
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
			1280,
			720,
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
			using namespace std::chrono;
			auto tStart = high_resolution_clock::now();
			
			while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) //0,0 here represents to take any input possible
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			if (msg.message == WM_QUIT)
				break;
			
			renderer.RenderFrame();
			
			++m_numFrames; //increment num of frames
			
			auto tEnd = high_resolution_clock::now();
			auto tDiff = duration_cast<milliseconds>(tEnd - tStart);
			auto dt = tDiff.count();
			
			m_time += dt; //increment time
			
			if (m_time >= 1000)
			{
				auto fps = m_numFrames;
				m_time = 0.0;
				m_numFrames = 0;

				LogInfo(std::to_string(fps).c_str());
			}
		}
	}
}