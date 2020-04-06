#ifndef __SPACE_COMMON_PREREQUISITES_HPP__
#define __SPACE_COMMON_PREREQUISITES_HPP__

#if (defined _UNICODE) || (defined UNICODE)
#define SPACE_UNICODE
#endif

#define SPACE_WIN32 1

#if defined _WIN32
#define SPACE_PLATFORM SPACE_WIN32
#ifdef _M_IX86
#endif
#else
#error no other platform supported.
#endif

#if (SPACE_PLATFORM == SPACE_WIN32)
#include <Windows.h>
#include <windowsx.h>
#include <atlbase.h>
#endif

#include "Math.hpp"

#include <atomic>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <future>
#include <algorithm>
#include <functional>
#include <vector>
#include <map>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>
#include <exception>
#include <typeinfo>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <ctime>

namespace Space
{
#ifdef SPACE_UNICODE
	using tstring = std::wstring;
	using tchar = wchar_t;
#else
	using tstring = std::string;
	using tchar = char;
#endif
	using uint = unsigned int;
	using uchar = unsigned char;
	using ushort = unsigned short;
	using ulong = unsigned long;
	using ulonglong = unsigned long long;  

	using sbyte = char;
	using byte = uchar;
	using int8 = char;
	using int16 = ushort;
	using int32  = int;
	using int64 = long long;
	using uint8 = uchar;
	using uint16 = ushort;
	using uint32  = uint;
	using uint64 = ulonglong;	
}

#ifdef SPACE_COMMON_EXPORTS
#define SPACE_COMMON_API __declspec(dllexport)
#else
#define SPACE_COMMON_API __declspec(dllimport)
#endif

#ifdef WIN32
#define ALIGNED_ALLOCATED __declspec(align(32))
#elif defined GCC
#define ALIGNED_ALLOCATED __attribute__(aligned(16))
#endif

#endif