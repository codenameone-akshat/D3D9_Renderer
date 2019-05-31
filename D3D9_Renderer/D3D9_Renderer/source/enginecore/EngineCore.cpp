#include <chrono>
#include <thread>

#include "EngineCore.h"
#include "../utils/Time.h"
#include "../utils/Logger.h"
namespace renderer
{
    EngineCore::EngineCore()
        :m_window(std::make_unique<Window>()),
        m_timer(std::make_unique<Time>()),
        m_renderer(std::make_unique<D3D9Renderer>())
    {
    }
    EngineCore::~EngineCore()
    {
    }
    void EngineCore::InitEngineCreateWindow(HINSTANCE hInstance, int32_t nShowCmd)
    {
        const auto windowClassName = L"rendererWindow_Class";
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

        auto& logger = Logger::GetInstance();
        logger.SetupLogger(m_window->GetHandleToWindow());

        InitRenderer();
    }
    void EngineCore::InitRenderer()
    {
        //auto& dx9renderer = D3D9Renderer::GetInstance();
        m_renderer->Init(m_window->GetHandleToWindow());
    }
    void EngineCore::RenderFrame()
    {
        //auto& dx9renderer = D3D9Renderer::GetInstance();
        m_renderer->PreRender();
        m_renderer->RenderFrame();
        m_renderer->PostRender();
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
            if (msg.message == WM_QUIT || msg.message == WM_DESTROY)
                break;

            this->RenderFrame();

            //std::this_thread::sleep_for(std::chrono::microseconds(2));
            m_timer->EndTimer();
        }
    }
}