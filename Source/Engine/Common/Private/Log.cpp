#include "Log.h"
#include "Utility.hpp"

#include <mutex>

namespace Space
{
	std::once_flag logOnceFlag; 
	std::unique_ptr<Logger> Logger::Instance;
	Logger* Logger::GetInstance(LogType type){
		std::call_once(logOnceFlag, [type](){
			switch (type){
			case FileLog:
				Instance.reset(new Logger(new FileLogWriter()));
				break;
			case StdLog:
				Instance.reset(new Logger(new StdLogWriter()));
				break;
			}
		});
		return Instance.get();
	}
	Logger::Logger(LogWriter* writer) : m_pWriter(writer)
	{}
	Logger::~Logger()
	{
	}
	void Logger::Printf(const wchar_t* format, ...)
	{
		va_list args;
		va_start(args, format);
		m_pWriter->Printf(format, args);
		va_end(args);
	}
	void Logger::Printf(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		m_pWriter->Printf(format, args);
		va_end(args);
	}
	void Logger::DebugPrintf(int32 line, const char* filename, const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		m_pWriter->DebugPrintf(line, filename, format, args);
		va_end(args);
	}
	void Logger::DebugPrintf(int32 line, const wchar_t* filename, const wchar_t* format, ...)
	{
		va_list args;
		va_start(args, format);
		m_pWriter->DebugPrintf(line, filename, format, args);
		va_end(args);
	}
	void Logger::Flush()
	{
		m_pWriter->Flush();
	}

	void FileLogWriter::Printf(const wchar_t* format, va_list args)
	{
		log += toTs(StringFormatW(format, args));
	}
	void FileLogWriter::Printf(const char* format, va_list args)
	{
		log += toTs(StringFormatA(format, args));
	}
	void FileLogWriter::DebugPrintf(int32 line, const char* filename, const char* format, va_list args)
	{
#if defined _DEBUG || defined DEBUG
		char buffer[1024];
		sprintf_s((char*)buffer, 1024, "%s line %d : %s", filename, line, format);
		log += toTs(StringFormatA(buffer, args));
#else
#endif
	}
	void FileLogWriter::DebugPrintf(int32 line, const wchar_t* filename, const wchar_t* format, va_list args)
	{
#if defined _DEBUG || defined DEBUG
		wchar_t buffer[1024];
		swprintf_s((wchar_t*)buffer, 1024, L"%s line %d : %s", filename, line, format);
		log += toTs(StringFormatW(buffer,args));
#else
#endif
	}

	void FileLogWriter::Flush()
	{
		time_t t = time(0);
		char strTime[1024];
		ctime_s(strTime,1024,&t);
		std::ofstream outfile(StringFormat(TEXT("Log-%s.log"), strTime));
		outfile << wstr2str(log);
		outfile.close();
#if defined _DEBUG || defined DEBUG
		outfile.open(StringFormat(TEXT("DebugLog-%s.log"), strTime));
		outfile << wstr2str(debugLog);
		outfile.close();
#else //no debug output
#endif
	}
	void StdLogWriter::Printf(const wchar_t* format, va_list args)
	{
		vwprintf_s(format, args);
	}
	void StdLogWriter::Printf(const char* format, va_list args)
	{
		vprintf_s(format, args);
	}
	void StdLogWriter::DebugPrintf(int32 line, const char* filename, const char* format, va_list args)
	{
#if defined _DEBUG || defined DEBUG
		char buffer[1024];
		sprintf_s((char*)buffer,1024,"%s line %d  : %s",filename,line,format);
		vprintf_s(buffer,args);
#else
#endif
	}
	void StdLogWriter::DebugPrintf(int32 line, const wchar_t* filename, const wchar_t* format, va_list args)
	{
#if defined _DEBUG || defined DEBUG
		wchar_t buffer[1024];
		swprintf_s((wchar_t*)buffer, 1024, L"%s line %d : %s", filename, line, format);
		vwprintf_s(buffer,args);
#else
#endif
	}
	void StdLogWriter::Flush()
	{
	}
}