#include <cassert>
#include <windows.h>
#include <string>
#include <sstream>

#include "Logger.h"

namespace renderer
{
	Logger::Logger()
		:m_window(nullptr)
	{
	}
	Logger::~Logger()
	{
		m_window = nullptr;
	}
	void Logger::SetupLogger(HWND window)
	{
		m_window = window;
	}
	void Logger::LogInfo(const char* outputStr)
	{
		std::wostringstream os_;
		os_ << outputStr << std::endl;
		OutputDebugString(os_.str().c_str());
	}
	void Logger::LogWarning(bool condition, const char* outputStr)
	{
		::MessageBoxA(m_window, "Warning!!! Please check the output window!!!", "Warning!", MB_OK);
		std::wostringstream os_;
		os_ << outputStr << std::endl;
		OutputDebugString(os_.str().c_str());
	}
	void Logger::LogError(bool condition, const char* outputStr)
	{
		::MessageBoxA(m_window, "Error!!! Please check the output window!!!", "Error!", MB_OK);
		assert(condition);
		std::wostringstream os_;
		os_ << outputStr << std::endl;
		OutputDebugString(os_.str().c_str());
	}
}