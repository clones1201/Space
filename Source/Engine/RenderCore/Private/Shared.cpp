#include "Shared.h"
#include "RenderCommon.h"

namespace Space
{
namespace Render
{
	size_t GetFormatSize(DataFormat format)
	{
		switch (format)
		{
		default:
		case DataFormat::UNKNOWN:
			return 0;
		case DataFormat::R16_UINT:
			return 1;
			//half2, short2
		case DataFormat::R16G16_TYPELESS:
		case DataFormat::R16G16_FLOAT:
		case DataFormat::R16G16_UNORM:
		case DataFormat::R16G16_UINT:
		case DataFormat::R16G16_SNORM:
		case DataFormat::R16G16_SINT:
			return 4;
			//hafl4: short4
		case DataFormat::R16G16B16A16_TYPELESS:
		case DataFormat::R16G16B16A16_FLOAT:
		case DataFormat::R16G16B16A16_UNORM:
		case DataFormat::R16G16B16A16_UINT:
		case DataFormat::R16G16B16A16_SNORM:
		case DataFormat::R16G16B16A16_SINT:
			return 8;
			//float1: uint1: int1
		case DataFormat::R32_TYPELESS:
		case DataFormat::R32_FLOAT:
		case DataFormat::R32_UINT:
		case DataFormat::R32_SINT:
			return 4;
			//float2: uint2: int2
		case DataFormat::R32G32_TYPELESS:
		case DataFormat::R32G32_FLOAT:
		case DataFormat::R32G32_UINT:
		case DataFormat::R32G32_SINT:
			return 8;
			//float3: uint3: int3
		case DataFormat::R32G32B32_TYPELESS:
		case DataFormat::R32G32B32_FLOAT:
		case DataFormat::R32G32B32_UINT:
		case DataFormat::R32G32B32_SINT:
			return 12;
			//float4: uint4: int4
		case DataFormat::R32G32B32A32_TYPELESS:
		case DataFormat::R32G32B32A32_FLOAT:
		case DataFormat::R32G32B32A32_UINT:
		case DataFormat::R32G32B32A32_SINT:
			return 16;
			//UByte4: 
		case DataFormat::B8G8R8A8_TYPELESS:
		case DataFormat::B8G8R8A8_UNORM:
		case DataFormat::B8G8R8A8_UNORM_SRGB:
		case DataFormat::B8G8R8X8_TYPELESS:
		case DataFormat::B8G8R8X8_UNORM:
		case DataFormat::B8G8R8X8_UNORM_SRGB:
			return 4;
			//depth: stencil
		case DataFormat::D16_UNORM:
			return 2;
		case DataFormat::D24_UNORM_S8_UINT:
		case DataFormat::D32_FLOAT:
			return 4;
		}
	}

#define ElementSize(type) case VertexElemType::type: return sizeof(type)

	size_t GetElementSize(VertexElemType type)
	{
		switch (type)
		{
			ElementSize(Float1);
			ElementSize(Float2);
			ElementSize(Float3);
			ElementSize(Float4);
			ElementSize(Byte2);
			ElementSize(Byte4);
			ElementSize(ByteN2);
			ElementSize(ByteN4);
			ElementSize(UByte2);
			ElementSize(UByte4);
			ElementSize(UByteN2);
			ElementSize(UByteN4);
			ElementSize(UShort2);
			ElementSize(UShort4);
			ElementSize(UShortN2);
			ElementSize(UShortN4);
			ElementSize(Short2);
			ElementSize(Short4);
			ElementSize(ShortN2);
			ElementSize(ShortN4);
			ElementSize(Half2);
			ElementSize(Half4);
			ElementSize(Color);
		case VertexElemType::None:
		case VertexElemType::MAX:
		default:
			return 0;
		}
	}
#undef ElementSize

	static char const* g_SemanticNames[] =
	{
		"UNKNOWN",
		"POSITION",
		"NORMAL",
		"TEXCOORD",
		"TANGENT"
	};

	char const* GetSemanticName(ElemSemantic semantic)
	{
		return g_SemanticNames[(int32)semantic];
	}
}
}