#include <cstdlib>
#include <crtdbg.h>

#include "../enginecore/EngineCore.h"

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);

	using namespace renderer;
	std::unique_ptr<EngineCore> engineCore = std::make_unique<EngineCore>();

	engineCore->InitEngineCreateWindow(hInstance, nShowCmd);
	engineCore->PollMessage();
	
	_CrtDumpMemoryLeaks();
	
	return 0;
}