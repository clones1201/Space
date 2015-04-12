#ifndef __D3DCORE_UTILITY_H__
#define __D3DCORE_UTILITY_H__

#include "Prerequisites.hpp"

// libarchive
// Multi - format archive and compression library
// http://www.libarchive.org/
//#include "archive.h"
//#include "archive_entry.h"

namespace Space
{
	tstring toTs(const std::string &in);
	tstring toTs(const std::wstring &in);
	std::wstring str2wstr(const std::string &in);
	std::wstring str2wstr(const std::wstring &in);
	std::string wstr2str(const std::wstring &in);
	std::string wstr2str(const std::string &in);

	int32 GetVarArgsW(wchar_t* dest, size_t size, const wchar_t* fmt, va_list args);
	int32 GetVarArgsA(char* dest, size_t size, const char* fmt, va_list args);

#if (defined _UNICODE) || (defined UNICODE)
#define GetVarArgs GetVarArgsW
#else
#define GetVarArgs GetVarArgsA
#endif

	std::wstring StringFormatW(const wchar_t* format, ...);
	std::string StringFormatA(const char* format, ...);
#if (defined _UNICODE) || (defined UNICODE)
#define StringFormat StringFormatW
#else
#define StringFormat StringFormatW
#endif

	float TStoF(const std::wstring& str);
	float TStoF(const std::string& str);

	std::string FtoA(float f);
	std::wstring FtoW(float f);

	template < class T >
	int32 AddUnique(std::vector<T>& array, T elem){
		int32 index = 0;
		for (auto i = array.begin(); i != array.end(); ++i)
		{
			if (*i == elem)
				return index;
			++index;
		}
		array.push_back(elem);
		return index;
	}

	template< class T >
	uint32 FindPosition(std::vector<T>& array, T elem)
	{
		return distance(array.begin(), std::find(array.begin(), array.end(), elem));
	}

	template<class Key, class T>
	std::vector<T> MapToVector(const std::map<Key, T>& map)
	{
		std::vector<T> ret;
		std::for_each(map.begin(), map.end(),
			[&ret](std::map<Key, T>::value_type elem)
		{
			ret.push_back(elem.second);
		});
		return std::move(ret);
	}

	std::ostream& Write(std::ostream& out, std::string const& str);
	std::istream& Read(std::istream& in, std::string& str);

	std::ostream& Write(std::ostream& out, std::wstring const& str);
	std::istream& Read(std::istream& in, std::wstring& str);

#ifdef X86_64
#	define ALIGNMENT 32
#else
#	define ALIGNMENT 16
#endif

	int32 Alignment(int32 n);

	int32 GetFormatSize(DataFormat format);
}


#endif