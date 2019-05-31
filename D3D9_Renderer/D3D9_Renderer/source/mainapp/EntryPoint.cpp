#include "../enginecore/EngineCore.h"

int32_t WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32_t nShowCmd)
{
	using namespace renderer;

	std::unique_ptr<EngineCore> engineCore = std::make_unique<EngineCore>();

	engineCore->InitEngineCreateWindow(hInstance, nShowCmd);
	engineCore->PollMessage();
	
	return 0;
}