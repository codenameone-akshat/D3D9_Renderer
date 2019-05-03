#pragma once
#include <string>
#include <windows.h>

namespace renderer
{
	class Logger
	{
		Logger()
			:m_window(nullptr)
		{
		}
	public:
		~Logger()
		{
			m_window = nullptr;
		}
		inline static Logger& GetInstance()
		{
			static Logger instance;
			return instance;
		}

		void SetupLogger(HWND window);
		void LogInfo(char* outputStr);
		void LogWarning(bool condition, char* outputStr);
		void LogError(bool condition, char* outputStr);

	private:
		HWND m_window;
	};
}