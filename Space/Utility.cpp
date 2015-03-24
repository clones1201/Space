#include "Utility.h"
//
//#pragma comment(lib,"archive.lib")

namespace Space
{

	tstring toTs(const std::string &in)
	{
#if (defined _UNICODE) ||  (defined UNICODE)
		return str2wstr(in);
#else
		return wstr2str(in);
#endif
	}
	tstring toTs(const std::wstring &in)
	{
#if (defined _UNICODE) ||  (defined UNICODE)
		return str2wstr(in);
#else
		return wstr2str(in);
#endif
	}
	std::wstring str2wstr(const std::string &in){
		std::wstring out;
		LPWSTR wstr = new WCHAR[in.size() + 1];
		MultiByteToWideChar(CP_ACP, 0, in.c_str(), in.size(), wstr, in.size());
		wstr[in.size()] = '\0';
		out = wstr;
		delete[] wstr;
		return out;
	}
	std::wstring str2wstr(const std::wstring &in)
	{
		return in;
	}
	std::string wstr2str(const std::wstring &in){
		std::string out;
		LPSTR str = new CHAR[in.size() + 1];
		WideCharToMultiByte(CP_ACP, 0, in.c_str(), in.size(), str, in.size(), nullptr, nullptr);
		str[in.size()] = '\0';
		out = str;
		delete[] str;
		return out;
	}
	std::string wstr2str(const std::string &in)
	{
		return in;
	}

	int GetVarArgsW(wchar_t* dest, size_t size, const wchar_t* fmt, va_list args){
		int result = _vsnwprintf_s(dest, size, size - 1, fmt, args);
		va_end(args);
		return result;
	}

	int GetVarArgsA(char* dest, size_t size, const char* fmt, va_list args)
	{
		int result = _vsnprintf_s(dest, size, size - 1, fmt, args);
		va_end(args);
		return result;
	}
	std::wstring StringFormatW(const wchar_t* format, ...)
	{
		va_list args;
		va_start(args, format);
		wchar_t buffer[2048];
		GetVarArgsW(buffer, 2048, format, args);
		return buffer;
	}
	std::string StringFormatA(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		char buffer[2048];
		GetVarArgsA(buffer, 2048, format, args);
		return buffer;
	}

	float TStoF(const std::wstring& str){
		return (float)_wtof(str.c_str());
	}

	float TStoF(const std::string& str){
		return (float)atof(str.c_str());
	}

	std::string FtoA(float f)
	{
		return StringFormatA("%f", f);
	}
	std::wstring FtoW(float f)
	{
		return StringFormatW(TEXT("%f"), f);
	}	
	std::ostream& Write(std::ostream& archiver, std::string const& str)
	{
		std::string::size_type size = str.size();
		archiver.write((sbyte*)&size, sizeof(std::string::size_type));
		archiver.write((sbyte*)str.c_str(), 2 * (size + 1));
		return archiver;
	}
	std::istream& Read(std::istream& archiver, std::string& str)
	{
		std::string::size_type size;
		archiver.read((sbyte*)&size, sizeof(std::string::size_type));
		char* buf = new char[size + 1];
		archiver.read((sbyte*)buf, (size + 1));
		str = std::string(buf);
		delete buf;
		return archiver;
	}

#ifdef X86_64
#	define ALIGNMENT 32
#else
#	define ALIGNMENT 16
#endif

	int Alignment(int n){
		return (ALIGNMENT * (1 + (n - 1) / ALIGNMENT));
	}
}