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
#else
#error no other platform supported.
#endif

#if (SPACE_PLATFORM == SPACE_WIN32)
#include <Windows.h>
#include <windowsx.h>

#include <atlbase.h>

#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include <DirectXPackedVector.h>
#endif

#include <algorithm>
#include <functional>
#include <vector>
#include <map>
#include <list>
#include <unordered_map>
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
#if (defined _UNICODE) || (defined UNICODE)
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
	class ShaderReflection;
	class ShaderResource;
	class ShaderResourcePool;

	class Renderable;
	class Mesh;
	class Material;
	class UniformExpression;
	class ResourceTexture;
	class SceneManager;

	typedef enum _BufferType
	{
		BT_VertexBuffer = 0, BT_IndexBuffer = 1, BT_ConstantBuffer = 2, BT_TextureBuffer = 3
	}BufferType;
	
	typedef enum _ResourceUsage
	{
		RU_Default = 0,
		RU_Dynamic = 1,
		RU_Immutable = 2,
		RU_Staging = 3
	}ResourceUsage;

	typedef enum _ResourceBindFlag
	{
		RB_None = 0,
		RB_ShaderResource = (1 << 0),
		RB_DepthStencil = (1 << 1),
		RB_RenderTarget = (1 << 2),
	}ResourceBindFlag;

	typedef enum _TextureType
	{
		TT_None = 0,
		TT_Texture1D = 1, TT_Texture1DArray =2,
		TT_Texture2D = 3, TT_Texture2DArray = 4,
		TT_Texture3D = 5, TT_Texture3DArray = 6
	}TextureType;

	typedef enum _DataFormat
	{
		DF_UNKNOWN = 0,
		//half2, short2
		DF_R16G16_TYPELESS,
		DF_R16G16_FLOAT,
		DF_R16G16_UNORM,
		DF_R16G16_UINT,
		DF_R16G16_SNORM,
		DF_R16G16_SINT,
		//hafl4, short4
		DF_R16G16B16A16_TYPELESS,
		DF_R16G16B16A16_FLOAT,
		DF_R16G16B16A16_UNORM,
		DF_R16G16B16A16_UINT,
		DF_R16G16B16A16_SNORM,
		DF_R16G16B16A16_SINT,
		//float1, uint1, int1
		DF_R32_TYPELESS,
		DF_R32_FLOAT,
		DF_R32_UINT,
		DF_R32_SINT,
		//float2, uint2, int2
		DF_R32G32_TYPELESS, 
		DF_R32G32_FLOAT,
		DF_R32G32_UINT,
		DF_R32G32_SINT,
		//float3, uint3, int3
		DF_R32G32B32_TYPELESS,
		DF_R32G32B32_FLOAT,
		DF_R32G32B32_UINT,
		DF_R32G32B32_SINT,
		//float4, uint4, int4
		DF_R32G32B32A32_TYPELESS,
		DF_R32G32B32A32_FLOAT,
		DF_R32G32B32A32_UINT,
		DF_R32G32B32A32_SINT,
		//UByte4, 
		DF_B8G8R8A8_TYPELESS,
		DF_B8G8R8A8_UNORM,
		DF_B8G8R8A8_UNORM_SRGB,
		DF_B8G8R8X8_TYPELESS,
		DF_B8G8R8X8_UNORM,
		DF_B8G8R8X8_UNORM_SRGB,
		//depth, stencil
		DF_D16_UNORM,
		DF_D24_UNORM_S8_UINT,
		DF_D32_FLOAT,
		DF_Max
	}DataFormat;
		
	typedef enum _VertexElemType
	{
		VET_None,
		VET_Float1,
		VET_Float2,
		VET_Float3,
		VET_Float4,
//		VET_PackedNormal,	// FPackedNormal
		VET_UByte4,
//		VET_UByte4N,
		VET_Color,
		VET_Short2,
		VET_Short4,
//		VET_Short2N,		// 16 bit word normalized to (value/32767.0,value/32767.0,0,0,1)
		VET_Half2,			// 16 bit float using 1 bit sign, 5 bit exponent, 10 bit mantissa 
		VET_Half4,
		VET_MAX
	}VertexElemType;

	typedef enum _ElemSemantic
	{
		ES_UNKNOWN = 0,
		ES_Position,
		ES_Normal,
		ES_Texcoord,
		ES_Tangent,
		ES_Bitangent
	}ElemSematic;

	typedef enum _ShaderModelLevel
	{
		SM_3_0 = 1,SM_4_0 = 2,SM_5_0 = 3
	}ShaderModelLevel;
}

#endif