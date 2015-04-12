#ifndef __D3DCORE_LOG_H__
#define __D3DCORE_LOG_H__

#include "Prerequisites.hpp"

namespace Space
{
	class LogWriter
	{
	public:
		virtual void Printf(const wchar_t* format, va_list args) = 0;
		virtual void Printf(const char* format, va_list args) = 0;
		virtual void DebugPrintf(int32 line, const char* filename, const char* format, va_list args) = 0;
		virtual void DebugPrintf(int32 line, const wchar_t* filename, const wchar_t* format,va_list args) = 0;
		virtual void Flush() = 0;
	};

	class FileLogWriter : public LogWriter
	{
	private:
		tstring log;
		tstring debugLog;
	public:
		virtual void Printf(const wchar_t* format, va_list args);
		virtual void Printf(const char* format, va_list args);
		virtual void DebugPrintf(int32 line, const char* filename, const char* format, va_list args);
		virtual void DebugPrintf(int32 line, const wchar_t* filename, const wchar_t* format, va_list args);
		virtual void Flush();
	};

	class StdLogWriter : public LogWriter
	{
	public:
		virtual void Printf(const wchar_t* format, va_list args);
		virtual void Printf(const char* format, va_list args);
		virtual void DebugPrintf(int32 line, const char* filename, const char* format, va_list args);
		virtual void DebugPrintf(int32 line, const wchar_t* filename, const wchar_t* format, va_list args);
		virtual void Flush();
	};

	enum LogType
	{
		FileLog, StdLog
	};

	class Logger
	{
	private:
		LogWriter* writer;
		Logger(LogWriter* writer);

		static std::unique_ptr<Logger> instance;
	public:
		//type only affects once
		static Logger* GetInstance(LogType type = StdLog);
		~Logger();

		void Printf(const wchar_t* format, ...);
		void Printf(const char* format, ...);
		void DebugPrintf(int32 line, const char* filename, const char* format, ...);
		void DebugPrintf(int32 line, const wchar_t* filename, const wchar_t* format, ...);
		void Flush();
	};

#define Log(...) Space::Logger::GetInstance()->Printf(__VA_ARGS__)
#define DebugLog(...) Space::Logger::GetInstance()->DebugPrintf(__LINE__,__FILE__,__VA_ARGS__)
}

#endif