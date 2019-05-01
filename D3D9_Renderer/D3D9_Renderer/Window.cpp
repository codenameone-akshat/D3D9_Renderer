#include "Window.h"
#include <cassert>
#include <stdio.h>

namespace renderer
{
	Window::Window()
		:m_hWindow(nullptr),
		m_windowClass{ 0 }
	{
	}

	Window::~Window()
	{
	}

	void Window::CreateD3DWindow(LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
	{
		m_hWindow = CreateWindowEx(NULL,
			lpClassName,
			lpWindowName,
			dwStyle,
			X,
			Y,
			nWidth,
			nHeight,
			hWndParent,
			hMenu,
			hInstance,
			lpParam);
		assert(m_hWindow);
	}

	void Window::SetUpAndRegesterWindow(HINSTANCE hInstance, LPCWSTR windowClassName)
	{
		ZeroMemory(&m_windowClass, sizeof(WNDCLASSEX));

		m_windowClass.hInstance = hInstance;
		m_windowClass.lpszClassName = windowClassName;
		m_windowClass.cbSize = sizeof(WNDCLASSEX);
		m_windowClass.style = CS_HREDRAW | CS_VREDRAW;
		m_windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		m_windowClass.lpfnWndProc = WindowMessageProc;

		RegisterClassEx(&m_windowClass);
	}

	LRESULT Window::WindowMessageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			exit(0);
			return 0;
			break;
		default:
			break;
		}

		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}