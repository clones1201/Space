#ifndef __SPACE_RENDERSYSTEM_SHARED_HPP__
#define __SPACE_RENDERSYSTEM_SHARED_HPP__

#include "RenderSystem/Prerequisites.hpp"

namespace Space
{
	enum class SPACE_RENDERSYSTEM_API BufferType : uint8
	{
		VertexBuffer = 0, IndexBuffer = 1, ConstantBuffer = 2, TextureBuffer = 3
	};

	enum class SPACE_RENDERSYSTEM_API ResourceUsage : uint8
	{
		Default = 0,
		Dynamic = 1,
		Immutable = 2,
		Staging = 3
	};

	enum class SPACE_RENDERSYSTEM_API ResourceBindFlag : uint8
	{
		None = 0,
		ShaderResource = (1 << 0),
		DepthStencil = (1 << 1),
		RenderTarget = (1 << 2),
	};

	enum class SPACE_RENDERSYSTEM_API TextureType : uint8
	{
		None = 0,
		Texture1D = 1, Texture1DArray = 2,
		Texture2D = 3, Texture2DArray = 4,
		Texture3D = 5, Texture3DArray = 6
	};

	enum class SPACE_RENDERSYSTEM_API DataFormat : uint16
	{
		UNKNOWN = 0,
		//half, short
		R16_TYPELESS,
		R16_FLOAT,
		R16_UNORM,
		R16_UINT,
		R16_SNORM,
		R16_SINT,
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

	enum class SPACE_RENDERSYSTEM_API VertexElemType : uint16
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

	enum class SPACE_RENDERSYSTEM_API ElemSemantic : uint8
	{
		Unknown = 0,
		Position,
		Normal,
		TexCoord,
		Tangent,
	};

	enum class SPACE_RENDERSYSTEM_API PrimitiveTopology : uint8
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

	enum class SPACE_RENDERSYSTEM_API ShaderModelLevel : uint8
	{
		SM_3_0 = 1, SM_4_0 = 2, SM_5_0 = 3
	};

	SPACE_RENDERSYSTEM_API int32 GetFormatSize(DataFormat format);
	SPACE_RENDERSYSTEM_API int32 GetElementSize(VertexElemType type);

	SPACE_RENDERSYSTEM_API char const* GetSemanticName(ElemSemantic semantic);
}

#endif