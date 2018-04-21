#pragma once

#include "Common.hpp"

namespace Space {
	namespace Render {
		enum class SPACE_COMMON_API BufferType : uint8
		{
			VertexBuffer = 0, IndexBuffer = 1, ConstantBuffer = 2, TextureBuffer = 3
		};

		enum class SPACE_COMMON_API ResourceUsage : uint8
		{
			Default = 0,
			Dynamic = 1,
			Immutable = 2,
			Staging = 3
		};

		enum class SPACE_COMMON_API ResourceBindFlag : uint8
		{
			None = 0,
			ShaderResource = (1 << 0),
			DepthStencil = (1 << 1),
			RenderTarget = (1 << 2),
		};

		enum class SPACE_COMMON_API TextureType : uint8
		{
			None = 0,
			Texture1D = 1, Texture1DArray = 2,
			Texture2D = 3, Texture2DArray = 4,
			Texture3D = 5, Texture3DArray = 6
		};

		enum class SPACE_COMMON_API DataFormat : uint8
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

		enum class SPACE_COMMON_API VertexElemType : uint8
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
			UShortN2,  // 16 bit word normalized to (value/32767.0,value/32767.0,0,0,1)
			UShortN4,
			Short2,
			Short4,
			ShortN2,  // 16 bit word normalized to (value/32767.0,value/32767.0,0,0,1)
			ShortN4,
			Half2,  	// 16 bit float using 1 bit sign, 5 bit exponent, 10 bit mantissa 
			Half4,
			Color,          //A 32-bit Alpha Red Green Blue (ARGB) color vector, where each color channel is specified as an unsigned 8 bit integer.
			MAX
		};

		enum class SPACE_COMMON_API ElemSemantic : uint8
		{
			Unknown = 0,
			Position,
			Normal,
			TexCoord,
			Tangent,
		};

		enum class SPACE_COMMON_API PrimitiveTopology : uint8
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

		enum class SPACE_COMMON_API ShaderModelLevel : uint8
		{
			SM_3_0 = 1, SM_4_0 = 2, SM_5_0 = 3
		};

		struct SPACE_COMMON_API ViewPort
		{
			float TopLeftX;
			float TopLeftY;
			float Width;
			float Height;
			float MinDepth;
			float MaxDepth;
		};

		struct SPACE_COMMON_API Rect
		{
			long left;
			long top;
			long right;
			long bottom;
		};


		enum class SPACE_COMMON_API ShaderInputType : uint8
		{
			SIT_CBuffer = 0,
			SIT_TBuffer = (SIT_CBuffer + 1),
			SIT_Texture = (SIT_TBuffer + 1),
			SIT_Sampler = (SIT_Texture + 1)
		};


		enum class SPACE_COMMON_API ShaderVariableClass : uint8
		{
			SVC_Scalar = 0,
			SVC_Vector = (SVC_Scalar + 1),
			SVC_Matrix_Rows = (SVC_Vector + 1),
			SVC_Matrix_Columns = (SVC_Matrix_Rows + 1),
			SVC_Object = (SVC_Matrix_Columns + 1),
			SVC_Struct = (SVC_Object + 1),
		};

		enum class SPACE_COMMON_API ShaderVariableType : uint8
		{
			SVT_Void = 0,
			SVT_Bool,
			SVT_Int,
			SVT_Float,
			SVT_String,
			SVT_Texture,
			SVT_Texture1D,
			SVT_Texture2D,
			SVT_Texture3D,
			//SVT_TextureCube,
			SVT_Sampler,
			SVT_Sampler1D,
			SVT_Sampler2D,
			SVT_Sampler3D,
			//SVT_SamplerCube,
			SVT_Uint,
			SVT_Uint8,
			//SVT_DepthStencil = 23,
			SVT_Blend,
			SVT_Buffer,
			SVT_CBuffer,
			SVT_TBuffer,
			SVT_Texture1DArray,
			SVT_Texture2DArray,
			//SVT_RenderTargetView = 30,
			//SVT_DepthStencilView = 31,
			//SVT_Texture2DMS,
			//SVT_Texture2DMSArray,
			//SVT_TextureCubeArray,
			SVT_Double,
			//SVT_RWTexture1D = 40,
			//SVT_RWTexture1Darray = 41,
			//SVT_RWTexture2D = 42,
			//SVT_RWTexture2Darray = 43,
			//SVT_RWTexture3D = 44,
			//SVT_RWBuffer = 45,
			//SVT_ByteAddress_Buffer = 46,
			//SVT_RWByteAddress_Buffer = 47,
			//SVT_Structured_Buffer = 48,
			//SVT_RWStructured_Buffer = 49,
			//SVT_Append_Structured_Buffer = 50,
			//SVT_Consume_Structured_Buffer = 51,
			//SVT_Min8Float = 52,
			//SVT_Min10Float = 53,
			//SVT_Min16Float = 54,
			//SVT_Min12Int = 55,
			//SVT_Min16Int = 56,
			//SVT_Min16Uint = 57,
		};


		enum class SPACE_COMMON_API ResourceReturnType : uint8
		{
			RRT_Unknown = 0,
			RRT_Unorm = 1,
			RRT_Snorm = 2,
			RRT_Sint = 3,
			RRT_Uint = 4,
			RRT_Float = 5,
			RRT_Mixed = 6,
			RRT_Double = 7,
		};

		enum class SPACE_COMMON_API ResourceDimension : uint8
		{
			RD_Unknown = 0,
			RD_Buffer = 1,
			RD_Texture1D = 2,
			RD_Texture1DArray = 3,
			RD_Texture2D = 4,
			RD_Texture2DArray = 5,
			RD_Texture2DMS = 6,
			RD_Texture2DMSArray = 7,
			RD_Texture3D = 8,
			RD_TextureCube = 9,
			RD_TextureCubeArray = 10,
			RD_BufferEx = 11,
		};
		enum class SPACE_COMMON_API FillMode : uint8
		{
			WireFrame = 2,
			Solid = 3
		};

		enum class SPACE_COMMON_API CullMode : uint8
		{
			None = 1,
			Front = 2,
			Back = 3
		};

		struct SPACE_COMMON_API RasterizerDesc
		{
			FillMode FillMode;
			CullMode CullMode;
			bool FrontCounterClockwise;
			int32 DepthBias;
			float DepthBiasClamp;
			float SlopeScaledDepthBias;
			bool DepthClipEnable;
			bool ScissorEnable;
			bool MultisampleEnable;
			bool AntialiasedLineEnable;
		};

		enum class SPACE_COMMON_API DepthWriteMask : uint8
		{
			Zero = 0,
			All = 1
		};

		enum class SPACE_COMMON_API ComparisonFunc : uint8
		{
			Never = 1,
			Less = 2,
			Equal = 3,
			LessEqual = 4,
			Greater = 5,
			NotEqual = 6,
			GreaterEqual = 7,
			Always = 8
		};

		enum class SPACE_COMMON_API StencilOp : uint8
		{
			Keep = 1,
			Zero = 2,
			Replace = 3,
			IncrSat = 4,
			DecrSat = 5,
			Invert = 6,
			Incr = 7,
			Decr = 8
		};

		struct SPACE_COMMON_API DepthStincilOpDesc
		{
			StencilOp StencilFailOp;
			StencilOp StencilDepthFailOp;
			StencilOp StencilPassOp;
			ComparisonFunc StencilFunc;
		};

		struct SPACE_COMMON_API DepthStencilDesc
		{
			bool DepthEnable;
			DepthWriteMask DepthWriteMask;
			ComparisonFunc DepthFunc;
			bool StencilEnable;
			uint8 StencilReadMask;
			uint8 StencilWriteMask;
			DepthStincilOpDesc FrontFace;
			DepthStincilOpDesc BackFace;
		};

		enum class SPACE_COMMON_API Blend : uint8
		{
			Zero = 1,
			One = 2,
			SrcColor = 3,
			InvSrcColor = 4,
			SrcAlpha = 5,
			InvSrcAlpha = 6,
			DestAlpha = 7,
			InvDestAlpha = 8,
			DestColor = 9,
			InvDestColor = 10,
			SrcAlphaSat = 11,
			BlendFactor = 14,
			InvBlendFactor = 15,
			Src1Color = 16,
			InvSrc1Color = 17,
			Src1Alpha = 18,
			InvSrc1Alpha = 19
		};

		enum class SPACE_COMMON_API BlendOperator : uint8
		{
			Add = 1,
			Subtract = 2,
			RevSubtract = 3,
			Min = 4,
			Max = 5
		};

		struct SPACE_COMMON_API RenderTargetBlendDesc
		{
			bool BlendEnable;
			Blend SrcBlend;
			Blend DestBlend;
			BlendOperator BlendOp;
			Blend SrcBlendAlpha;
			Blend DestBlendAlpha;
			BlendOperator BlendOpAlpha;
			uint8 RenderTargetWriteMask;
		};

		struct SPACE_COMMON_API SampleDesc
		{
			uint Count;
			uint Quality;
		};

		struct SPACE_COMMON_API BlendDesc
		{
			bool AlphaToCoverageEnable;
			bool IndependentBlendEnable;
			RenderTargetBlendDesc RenderTarget[8];
		};
	}
}