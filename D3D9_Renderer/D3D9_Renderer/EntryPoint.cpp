#define _CRTDBG_MAP_ALLOC

#include <cstdlib>
#include <crtdbg.h>
#include "safewindows.h"

#include "EngineCore.h"

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	using namespace d3dgfx;
	std::unique_ptr<EngineCore> engineCore = std::make_unique<EngineCore>();

	engineCore->InitEngineCreateWindow(hInstance, nShowCmd);
	engineCore->PollMessage();

	_CrtDumpMemoryLeaks();
	return 0;
}