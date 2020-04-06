#pragma once

#include "Common.hpp"
#include "Basic.hpp"

#ifdef RENDERCORE_EXPORTS
#define RENDERCORE_API __declspec(dllexport)
#define RENDERCORE_EXTERN
#else
#define RENDERCORE_API __declspec(dllimport)
#define RENDERCORE_EXTERN extern
#endif

namespace Space 
{
namespace Render 
{
	enum class RENDERCORE_API BufferType : uint8
	{
		VertexBuffer = 0, IndexBuffer = 1, ConstantBuffer = 2, TextureBuffer = 3
	};

	enum class RENDERCORE_API ResourceUsage : uint8
	{
		Default = 0,
		Dynamic = 1,
		Immutable = 2,
		Staging = 3
	};

	enum class RENDERCORE_API ResourceBindFlag : uint8
	{
		None = 0,
		ShaderResource = (1 << 0),
		DepthStencil = (1 << 1),
		RenderTarget = (1 << 2),
	};

	enum class RENDERCORE_API TextureType : uint8
	{
		None = 0,
		Texture1D = 1, Texture1DArray = 2,
		Texture2D = 3, Texture2DArray = 4,
		Texture3D = 5, Texture3DArray = 6
	};

	enum class RENDERCORE_API DataFormat : uint8
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

	enum class RENDERCORE_API VertexElemType : uint8
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

	enum class RENDERCORE_API ElemSemantic : uint8
	{
		Unknown = 0,
		Position,
		Normal,
		TexCoord,
		Tangent,
	};

	enum class RENDERCORE_API PrimitiveTopology : uint8
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

	enum class RENDERCORE_API ShaderModelLevel : uint8
	{
		SM_3_0 = 1, SM_4_0 = 2, SM_5_0 = 3
	};

	struct RENDERCORE_API ViewPort
	{
		float TopLeftX;
		float TopLeftY;
		float Width;
		float Height;
		float MinDepth;
		float MaxDepth;
	};

	struct RENDERCORE_API Rect
	{
		long left;
		long top;
		long right;
		long bottom;
	};


	enum class RENDERCORE_API ShaderInputType : uint8
	{
		SIT_CBuffer = 0,
		SIT_TBuffer = (SIT_CBuffer + 1),
		SIT_Texture = (SIT_TBuffer + 1),
		SIT_Sampler = (SIT_Texture + 1)
	};


	enum class RENDERCORE_API ShaderVariableClass : uint8
	{
		SVC_Scalar = 0,
		SVC_Vector = (SVC_Scalar + 1),
		SVC_Matrix_Rows = (SVC_Vector + 1),
		SVC_Matrix_Columns = (SVC_Matrix_Rows + 1),
		SVC_Object = (SVC_Matrix_Columns + 1),
		SVC_Struct = (SVC_Object + 1),
	};

	enum class RENDERCORE_API ShaderVariableType : uint8
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


	enum class RENDERCORE_API ResourceReturnType : uint8
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

	enum class RENDERCORE_API ResourceDimension : uint8
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
	enum class RENDERCORE_API FillMode : uint8
	{
		WireFrame = 2,
		Solid = 3
	};

	enum class RENDERCORE_API CullMode : uint8
	{
		None = 1,
		Front = 2,
		Back = 3
	};

	struct RENDERCORE_API RasterizerStateDescriptor : HashableBits<RasterizerStateDescriptor>, ComparableBits<RasterizerStateDescriptor>
	{
		// total 105 bits, 14 bytes
		FillMode FillMode : 2;
		CullMode CullMode : 2;
		bool FrontCounterClockwise : 1;
		int32 DepthBias;
		float DepthBiasClamp;
		float SlopeScaledDepthBias;
		bool DepthClipEnable : 1;
		bool ScissorEnable : 1;
		bool MultisampleEnable : 1;
		bool AntialiasedLineEnable : 1;

		RasterizerStateDescriptor() :
			FillMode(FillMode::Solid),
			CullMode(CullMode::Back),
			FrontCounterClockwise(true),
			DepthBias(0),
			DepthBiasClamp(0.0f),
			SlopeScaledDepthBias(0.0f),
			DepthClipEnable(false),
			ScissorEnable(false),
			MultisampleEnable(false),
			AntialiasedLineEnable(false)
		{}
	};

	enum class RENDERCORE_API DepthWriteMask : uint8
	{
		Zero = 0,
		All = 1
	};

	enum class RENDERCORE_API ComparisonFunc : uint8
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

	enum class RENDERCORE_API StencilOperator : uint8
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

	struct RENDERCORE_API DepthStencilOpDescriptor
	{
		StencilOperator StencilFailOp : 4;
		StencilOperator StencilDepthFailOp : 4;
		StencilOperator StencilPassOp : 4;
		ComparisonFunc StencilFunc : 4;

		DepthStencilOpDescriptor() :
			StencilFailOp(StencilOperator::Keep),
			StencilDepthFailOp(StencilOperator::Keep),
			StencilPassOp(StencilOperator::Keep),
			StencilFunc(ComparisonFunc::Always)
		{}
	};

	struct RENDERCORE_API DepthStencilStateDescriptor : HashableBits<DepthStencilStateDescriptor>, ComparableBits<DepthStencilStateDescriptor>
	{
		//total 55 bits, 7 bytes
		bool DepthEnable : 1;
		DepthWriteMask DepthWriteMask : 1;
		ComparisonFunc DepthFunc : 4;
		bool StencilEnable : 1;
		uint8 StencilReadMask;
		uint8 StencilWriteMask;
		DepthStencilOpDescriptor FrontFace;
		DepthStencilOpDescriptor BackFace;

		DepthStencilStateDescriptor() :
			DepthEnable(true),
			DepthWriteMask(DepthWriteMask::All),
			DepthFunc(ComparisonFunc::Less),
			StencilEnable(false),
			StencilReadMask(0xff),
			StencilWriteMask(0xff)
		{}
	};

	enum class RENDERCORE_API BlendFactor : uint8
	{
		BF_Zero = 1,
		BF_One = 2,
		BF_SrcColor = 3,
		BF_InvSrcColor = 4,
		BF_SrcAlpha = 5,
		BF_InvSrcAlpha = 6,
		BF_DestAlpha = 7,
		BF_InvDestAlpha = 8,
		BF_DestColor = 9,
		BF_InvDestColor = 10,
		BF_SrcAlphaSat = 11,
		BF_BlendFactor = 14,
		BF_InvBlendFactor = 15,
		BF_Src1Color = 16,
		BF_InvSrc1Color = 17,
		BF_Src1Alpha = 18,
		BF_InvSrc1Alpha = 19
	};

	enum class RENDERCORE_API BlendOperator : uint8
	{
		Add = 1,
		Subtract = 2,
		RevSubtract = 3,
		Min = 4,
		Max = 5
	};

	struct RENDERCORE_API RenderTargetBlendDescriptor
	{
		// total 35 bits, 5 bytes
		bool BlendEnable : 1;
		BlendFactor SrcBlend : 5;
		BlendFactor DestBlend : 5;
		BlendOperator BlendOp : 3;
		BlendFactor SrcBlendAlpha : 5;
		BlendFactor DestBlendAlpha : 5;
		BlendOperator BlendOpAlpha : 3;
		uint8 RenderTargetWriteMask : 8;
	};

	struct RENDERCORE_API BlendStateDescriptor : HashableBits<BlendStateDescriptor>, ComparableBits<BlendStateDescriptor>
	{
		// total 282 bits, 36 bytes
		bool AlphaToCoverageEnable : 1;
		bool IndependentBlendEnable : 1;
		RenderTargetBlendDescriptor RenderTarget[8];
	};

	struct RENDERCORE_API SampleDesc
	{
		uint Count;
		uint Quality;
	};

	enum class RENDERCORE_API ElementClass : uint8
	{
		PerVertex, PerInstance
	};
	struct InputElement
	{
		VertexElemType Type;
		ElemSemantic Semantic;
		uint32 SemanticIdx;
		uint32 InputSlot;
		ElementClass ElemClass;
		uint32 InstanceStep;
		uint32 AlignedByteOffset; //leave this field 0, will be calculated
	};

	typedef std::vector<InputElement> ElementArray;
	RENDERCORE_API size_t GetFormatSize(DataFormat format);
	RENDERCORE_API size_t GetElementSize(VertexElemType type);

	RENDERCORE_API char const* GetSemanticName(ElemSemantic semantic);

	namespace Details {
		template<class RenderSystem> class DeviceImpl;
		template<class RenderSystem> class BufferImpl;
		template<class RenderSystem, class Texture> class TextureImpl;
		/*template<class Impl> class DepthStencilViewImpl;
		template<class Impl> class ShaderResourceViewImpl;*/
		template<class RenderSystem> class CommandListImpl;
		template<class RenderSystem> class PipelineStateImpl;
		template<class RenderSystem> class RenderTargetImpl;
		template<class RenderSystem> class DepthStencilViewImpl;
		template<class RenderSystem> class RenderWindowImpl;
		template<class RenderSystem> class VertexShaderImpl;
		template<class RenderSystem> class PixelShaderImpl;
		/*template<class Impl> class ShaderReflectionImpl;
		template<class Impl> class ShaderReflectionConstantBufferImpl;
		template<class Impl> class ShaderReflectionBindResourceImpl;
		template<class Impl> class ShaderReflectionVariableImpl;*/

	}

}
}

DEFINE_STD_HASH_SPECIALIZATION(Space::Render::RasterizerStateDescriptor);
DEFINE_STD_HASH_SPECIALIZATION(Space::Render::DepthStencilStateDescriptor);
DEFINE_STD_HASH_SPECIALIZATION(Space::Render::BlendStateDescriptor);
