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

		void CreateD3DWindow(const LPCWSTR lpClassName, const LPCWSTR lpWindowName, const DWORD dwStyle, 
            const int X, const int Y, const int nWidth, const int nHeight, const HWND hWndParent,
            const HMENU hMenu, const HINSTANCE hInstance, const LPVOID lpParam);

		void SetUpAndRegesterWindow(HINSTANCE hInstance, LPCWSTR windowName);

		//>Getter
		inline HWND GetHandleToWindow() const { return m_hWindow; };

		inline void ShowD3DWindow(const int nShowCmd) const
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