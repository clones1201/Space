#ifndef __SPACE_COMMON_LOG_H__
#define __SPACE_COMMON_LOG_H__

#include "Common/Prerequisites.hpp"

namespace Space
{
	class SPACE_COMMON_API LogWriter
	{
	public:
		virtual void Printf(const wchar_t* format, va_list args) = 0;
		virtual void Printf(const char* format, va_list args) = 0;
		virtual void DebugPrintf(int32 line, const char* filename, const char* format, va_list args) = 0;
		virtual void DebugPrintf(int32 line, const wchar_t* filename, const wchar_t* format,va_list args) = 0;
		virtual void Flush() = 0;
	};

	class SPACE_COMMON_API FileLogWriter : public LogWriter
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

	class SPACE_COMMON_API StdLogWriter : public LogWriter
	{
	public:
		virtual void Printf(const wchar_t* format, va_list args);
		virtual void Printf(const char* format, va_list args);
		virtual void DebugPrintf(int32 line, const char* filename, const char* format, va_list args);
		virtual void DebugPrintf(int32 line, const wchar_t* filename, const wchar_t* format, va_list args);
		virtual void Flush();
	};

	enum SPACE_COMMON_API LogType
	{
		FileLog, StdLog
	};

	class SPACE_COMMON_API Logger
	{
	private:
		std::unique_ptr<LogWriter> m_pWriter;
		Logger(LogWriter* writer);

		static std::unique_ptr<Logger> Instance;
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