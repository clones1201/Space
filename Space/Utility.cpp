#include "Utility.h" 

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
		MultiByteToWideChar(CP_ACP, 0, in.c_str(), (int32)in.size(), wstr, (int32)in.size());
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
		WideCharToMultiByte(CP_ACP, 0, in.c_str(), (int32)in.size(), str, (int32)in.size(), nullptr, nullptr);
		str[in.size()] = '\0';
		out = str;
		delete[] str;
		return out;
	}
	std::string wstr2str(const std::string &in)
	{
		return in;
	}

	int32 GetVarArgsW(wchar_t* dest, uint32 size, const wchar_t* fmt, va_list args){
		int32 result = _vsnwprintf_s(dest, size, size - 1, fmt, args);
		va_end(args);
		return result;
	}

	int32 GetVarArgsA(char* dest, uint32 size, const char* fmt, va_list args)
	{
		int32 result = _vsnprintf_s(dest, size, size - 1, fmt, args);
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
		archiver.write((sbyte*)str.c_str(), sizeof(std::string::value_type) * (size + 1));
		return archiver;
	}
	std::istream& Read(std::istream& archiver, std::string& str)
	{
		std::string::size_type size;
		archiver.read((sbyte*)&size, sizeof(std::string::size_type));
		char* buf = new char[size + 1];
		archiver.read((sbyte*)buf, sizeof(std::string::value_type) * (size + 1));
		str = std::string(buf);
		delete buf;
		return archiver;
	}

	std::ostream& Write(std::ostream& archiver, std::wstring const& str)
	{
		std::string::size_type size = str.size();
		archiver.write((sbyte*)&size, sizeof(std::wstring::size_type));
		archiver.write((sbyte*)str.c_str(), sizeof(std::wstring::value_type) * (size + 1));
		return archiver;
	}
	std::istream& Read(std::istream& archiver, std::wstring& str)
	{
		std::wstring::size_type size;
		archiver.read((sbyte*)&size, sizeof(std::wstring::size_type));
		wchar_t* buf = new wchar_t[size + 1];
		archiver.read((sbyte*)buf, sizeof(std::wstring::value_type) * (size + 1));
		str = std::wstring(buf);
		delete buf;
		return archiver;
	}

#ifdef X86_64
#	define ALIGNMENT 32
#else
#	define ALIGNMENT 16
#endif

	int32 Alignment(int32 n){
		return (ALIGNMENT * (1 + (n - 1) / ALIGNMENT));
	}

	int32 GetFormatSize(DataFormat format)
	{
		switch (format)
		{
		default:
		case DF_UNKNOWN:
			return 0;
			//half2, short2
		case DF_R16G16_TYPELESS:
		case DF_R16G16_FLOAT:
		case DF_R16G16_UNORM:
		case DF_R16G16_UINT:
		case DF_R16G16_SNORM:
		case DF_R16G16_SINT:
			return 4;
			//hafl4: short4
		case DF_R16G16B16A16_TYPELESS:
		case DF_R16G16B16A16_FLOAT:
		case DF_R16G16B16A16_UNORM:
		case DF_R16G16B16A16_UINT:
		case DF_R16G16B16A16_SNORM:
		case DF_R16G16B16A16_SINT:
			return 8;
			//float1: uint1: int1
		case DF_R32_TYPELESS:
		case DF_R32_FLOAT:
		case DF_R32_UINT:
		case DF_R32_SINT:
			return 4;
			//float2: uint2: int2
		case DF_R32G32_TYPELESS:
		case DF_R32G32_FLOAT:
		case DF_R32G32_UINT:
		case DF_R32G32_SINT:
			return 8;
			//float3: uint3: int3
		case DF_R32G32B32_TYPELESS:
		case DF_R32G32B32_FLOAT:
		case DF_R32G32B32_UINT:
		case DF_R32G32B32_SINT:
			return 12;
			//float4: uint4: int4
		case DF_R32G32B32A32_TYPELESS:
		case DF_R32G32B32A32_FLOAT:
		case DF_R32G32B32A32_UINT:
		case DF_R32G32B32A32_SINT:
			return 16;
			//UByte4: 
		case DF_B8G8R8A8_TYPELESS:
		case DF_B8G8R8A8_UNORM:
		case DF_B8G8R8A8_UNORM_SRGB:
		case DF_B8G8R8X8_TYPELESS:
		case DF_B8G8R8X8_UNORM:
		case DF_B8G8R8X8_UNORM_SRGB:
			return 4;
			//depth: stencil
		case DF_D16_UNORM:
			return 2;
		case DF_D24_UNORM_S8_UINT:
		case DF_D32_FLOAT:
			return 4;
		} 
	}

	int32 GetElementSize(VertexElemType type)
	{
		switch (type)
		{
		case VET_Float1:
			return sizeof(float);
		case VET_Float2:
			return sizeof(Float2);
		case VET_Float3:
			return sizeof(Float3);
		case VET_Float4:
			return sizeof(Float4);
		case VET_Byte2:
			return sizeof(Byte2);
		case VET_Byte4:
			return sizeof(Byte4);
		case VET_ByteN2:
			return sizeof(ByteN2);
		case VET_ByteN4:
			return sizeof(ByteN4);
		case VET_UByte2:
			return sizeof(UByte2);
		case VET_UByte4:
			return sizeof(UByte4);
		case VET_UByteN2:
			return sizeof(UByteN2);
		case VET_UByteN4:
			return sizeof(UByteN4);
		case VET_UShort2:
			return sizeof(UShort2);
		case VET_UShort4:
			return sizeof(UShort4);
		case VET_UShortN2:		// 16 bit word normalized to (value/32767.0,value/32767.0,0,0,1)
			return sizeof(UShortN2);
		case VET_UShortN4:
			return sizeof(UShortN4);
		case VET_Short2:
			return sizeof(Short2);
		case VET_Short4:
			return sizeof(Short4);
		case VET_ShortN2:		// 16 bit word normalized to (value/32767.0,value/32767.0,0,0,1)
			return sizeof(ShortN2);
		case VET_ShortN4:
			return sizeof(ShortN4);
		case VET_Half2:
			return sizeof(Half2);
		case VET_Half4:
			return sizeof(Half4);
		case VET_Color:
			return sizeof(Color);	
		case VET_None:
		case VET_MAX:
		default:
			return 0;
		}
	}
}