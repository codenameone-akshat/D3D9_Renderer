#pragma once
#include<windows.h>

class D3DSystem
{
public:
	D3DSystem();
	~D3DSystem();
	// Init DirectX System
	bool Init();
	// Uninitialize DirectX System
	void UnInit();
	// System Main Loop
	void MainLoop();
private:
	LPCSTR m_appName;
	HINSTANCE m_hInstance;
	HWND m_hWnd;
};

