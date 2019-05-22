#pragma once

#include <windows.h>
#include <windowsx.h>
#include <stdint.h>

namespace renderer
{
	class Window
	{
	public:
		Window();
		~Window();

		void CreateD3DWindow(const LPCWSTR lpClassName, const LPCWSTR lpWindowName, const DWORD dwStyle, 
            const int32_t X, const int32_t Y, const int32_t nWidth, const int32_t nHeight, const HWND hWndParent,
            const HMENU hMenu, const HINSTANCE hInstance, const LPVOID lpParam);

		void SetUpAndRegesterWindow(HINSTANCE hInstance, LPCWSTR windowName);

		//>Getter
		inline HWND GetHandleToWindow() const { return m_hWindow; };

		inline void ShowD3DWindow(const int32_t nShowCmd) const
		{
			if (m_hWindow)
				::ShowWindow(m_hWindow, nShowCmd);
		}

	private:
		//>Procedure for Microsoft Window callbacks
		static LRESULT CALLBACK WindowMessageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		HWND m_hWindow;
		WNDCLASSEX m_windowClass;
	};
}