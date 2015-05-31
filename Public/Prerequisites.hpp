#ifndef __SPACE_PREREQUISITES_HPP__
#define __SPACE_PREREQUISITES_HPP__

#ifdef SPACE_EXPORTS
#define SPACE_API __declspec(dllexport)
#else
#define SPACE_API __declspec(dllimport)
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

#if (defined _UNICODE) || (defined UNICODE)
#define SPACE_UNICODE
#endif

#include "rapidjson/rapidjson.h"
#include "rapidjson/writer.h"
#include "rapidjson/document.h"

#if SPACE_PLATFORM == SPACE_WIN32
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define CHECK_MEMORY_LEAKS_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new CHECK_MEMORY_LEAKS_NEW
#endif

namespace Space
{
#ifdef SPACE_UNICODE
	using tstring = std::wstring;
#else
	using tstring = std::string;
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
	
	class Core;
	class Game;
	class Window;
	class WindowUtility;

	class RenderSystem;
	class DeviceBuffer;
	class VertexBuffer;
	class IndexBuffer;
	class ConstantBuffer;
	class TextureBuffer;
	class DeviceTexture1D;
	class DeviceTexture2D;
	class DeviceTexture3D;
	class DepthStencilView; 
	class RenderTarget;
	class RenderWindow;
	class VertexShader;
	class PixelShader;
	class Shader;
	class ShaderReflection;
	class ShaderResource;
	class ShaderResourcePool;

	class InputLayout;
	class CommandList;
	class PipelineState;
	class Renderable;
	class Mesh;
	class Material;
	class UniformExpression;
	class ResourceTexture;
	class SceneManager;

	enum class BufferType : uint8
	{
		VertexBuffer = 0, IndexBuffer = 1, ConstantBuffer = 2, TextureBuffer = 3
	};
	
	enum class ResourceUsage : uint8
	{
		Default = 0,
		Dynamic = 1,
		Immutable = 2,
		Staging = 3
	};

	enum class ResourceBindFlag : uint8
	{
		None = 0,
		ShaderResource = (1 << 0),
		DepthStencil = (1 << 1),
		RenderTarget = (1 << 2),
	};

	enum class TextureType : uint8
	{
		None = 0,
		Texture1D = 1, Texture1DArray =2,
		Texture2D = 3, Texture2DArray = 4,
		Texture3D = 5, Texture3DArray = 6
	};

	enum class DataFormat : uint16
	{
		UNKNOWN = 0,
		//half2, short2
		R16G16_TYPELESS,
		R16G16_FLOAT,
		R16G16_UNORM,
		R16G16_UINT,
		R16G16_SNORM,
		R16G16_SINT,
		//hafl4, short4
		R16G16B16A16_TYPELESS,
		R16G16B16A16_FLOAT,
		R16G16B16A16_UNORM,
		R16G16B16A16_UINT,
		R16G16B16A16_SNORM,
		R16G16B16A16_SINT,
		//float1, uint1, int1
		R32_TYPELESS,
		R32_FLOAT,
		R32_UINT,
		R32_SINT,
		//float2, uint2, int2
		R32G32_TYPELESS, 
		R32G32_FLOAT,
		R32G32_UINT,
		R32G32_SINT,
		//float3, uint3, int3
		R32G32B32_TYPELESS,
		R32G32B32_FLOAT,
		R32G32B32_UINT,
		R32G32B32_SINT,
		//float4, uint4, int4
		R32G32B32A32_TYPELESS,
		R32G32B32A32_FLOAT,
		R32G32B32A32_UINT,
		R32G32B32A32_SINT,
		//UByte4, 
		B8G8R8A8_TYPELESS,
		B8G8R8A8_UNORM,
		B8G8R8A8_UNORM_SRGB,
		B8G8R8X8_TYPELESS,
		B8G8R8X8_UNORM,
		B8G8R8X8_UNORM_SRGB,
		//depth, stencil
		D16_UNORM,
		D24_UNORM_S8_UINT,
		D32_FLOAT,
		Max
	};
		
	enum class VertexElemType : uint16
	{
		None,
		Float1,
		Float2,
		Float3,
		Float4,
		Byte2,
		Byte4,
		ByteN2,
		ByteN4,
		UByte2,
		UByte4,
		UByteN2,
		UByteN4,
		UShort2,
		UShort4,
		UShortN2,		// 16 bit word normalized to (value/32767.0,value/32767.0,0,0,1)
		UShortN4,
		Short2,
		Short4,
		ShortN2,		// 16 bit word normalized to (value/32767.0,value/32767.0,0,0,1)
		ShortN4,
		Half2,			// 16 bit float using 1 bit sign, 5 bit exponent, 10 bit mantissa 
		Half4,
		Color,          //A 32-bit Alpha Red Green Blue (ARGB) color vector, where each color channel is specified as an unsigned 8 bit integer.
		MAX
	};

	enum class ElemSemantic : uint8
	{
		Unknown = 0,
		Position,
		Normal,
		TexCoord,
		Tangent,
	};
	
	enum class PrimitiveTopology : uint8
	{
		Undefined = 0,
		PointList,
		LineList,
		LineStripe,
		TriangleList,
		TriangleStrip = 5,
		LineList_Adj = 10,
		LineStripe_Adj,
		TriangleList_Adj,
		TriangleStrip_Adj,
	};

	enum class ShaderModelLevel : uint8
	{
		SM_3_0 = 1,SM_4_0 = 2,SM_5_0 = 3
	};
}

#endif