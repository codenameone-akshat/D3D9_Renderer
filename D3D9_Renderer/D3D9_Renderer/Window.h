#pragma once

#include<windows.h>
#include<windowsx.h>
namespace d3dgfx
{
	class Window
	{
	public:
		Window();
		~Window();

		void CreateD3DWindow(LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
		void SetUpAndRegesterWindow(HINSTANCE hInstance, LPCWSTR windowName);

		//>Getter
		inline HWND GetHandleToWindow() const { return m_hWindow; };

		inline void ShowD3DWindow(int nShowCmd)
		{
			if (m_hWindow)
				ShowWindow(m_hWindow, nShowCmd);
		}

	private:
		//Procedure for Microsoft Window callbacks
		static LRESULT CALLBACK WindowMessageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		HWND m_hWindow;
		WNDCLASSEX m_windowClass;
	};
}