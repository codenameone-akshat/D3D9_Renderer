#pragma once
#include <string>
#include <windows.h>

namespace renderer
{
	class Logger
	{
		Logger();
	public:
		~Logger();
		
		inline static Logger& GetInstance()
		{
			static Logger instance;
			return instance;
		}

		void SetupLogger(HWND window);
		void LogInfo(const char* outputStr);
		void LogWarning(bool condition, const char* outputStr);
		void LogError(bool condition, const char* outputStr);

	private:
		HWND m_window;
	};
}