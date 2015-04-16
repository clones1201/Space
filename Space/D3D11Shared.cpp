#include "Prerequisites.hpp"
#include "D3D11Shared.hpp"

namespace Space
{
	D3D11DeviceCreateFailedExcepion::D3D11DeviceCreateFailedExcepion()
		:std::exception("D3D11 Device Create Failed"){
	}

	UINT GetD3D11BufferBindFlags(BufferType type)
	{
		switch (type)
		{
		case BufferType::BT_VertexBuffer:
			return D3D11_BIND_VERTEX_BUFFER;
		case BufferType::BT_IndexBuffer:
			return D3D11_BIND_INDEX_BUFFER;
		case BufferType::BT_ConstantBuffer:
			return D3D11_BIND_CONSTANT_BUFFER;
		case BufferType::BT_TextureBuffer:
			return D3D11_BIND_SHADER_RESOURCE;
		}
		return 0;
	}
	
	D3D11_USAGE GetD3D11Usage(ResourceUsage usage)
	{
		switch (usage)
		{
		case RU_Default:
			return D3D11_USAGE_DEFAULT;
		case RU_Dynamic:
			return D3D11_USAGE_DYNAMIC;
		case RU_Immutable:
			return D3D11_USAGE_IMMUTABLE;
		case RU_Staging:
			return D3D11_USAGE_STAGING;
		default:
			return D3D11_USAGE_DEFAULT;
		}
	}

	static DXGI_FORMAT g_FormatTable[] = 
	{
		DXGI_FORMAT_UNKNOWN,
		//half2, short2
		DXGI_FORMAT_R16G16_TYPELESS,
		DXGI_FORMAT_R16G16_FLOAT,
		DXGI_FORMAT_R16G16_UNORM,
		DXGI_FORMAT_R16G16_UINT,
		DXGI_FORMAT_R16G16_SNORM,
		DXGI_FORMAT_R16G16_SINT,
		//hafl4, short4
		DXGI_FORMAT_R16G16B16A16_TYPELESS,
		DXGI_FORMAT_R16G16B16A16_FLOAT,
		DXGI_FORMAT_R16G16B16A16_UNORM,
		DXGI_FORMAT_R16G16B16A16_UINT,
		DXGI_FORMAT_R16G16B16A16_SNORM,
		DXGI_FORMAT_R16G16B16A16_SINT,
		//float1, uint1, int1
		DXGI_FORMAT_R32_TYPELESS,
		DXGI_FORMAT_R32_FLOAT,
		DXGI_FORMAT_R32_UINT,
		DXGI_FORMAT_R32_SINT,
		//float2, uint2, int2
		DXGI_FORMAT_R32G32_TYPELESS,
		DXGI_FORMAT_R32G32_FLOAT,
		DXGI_FORMAT_R32G32_UINT,
		DXGI_FORMAT_R32G32_SINT,
		//float3, uint3, int3
		DXGI_FORMAT_R32G32B32_TYPELESS,
		DXGI_FORMAT_R32G32B32_FLOAT,
		DXGI_FORMAT_R32G32B32_UINT,
		DXGI_FORMAT_R32G32B32_SINT,
		//float4, uint4, int4
		DXGI_FORMAT_R32G32B32A32_TYPELESS,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_R32G32B32A32_UINT,
		DXGI_FORMAT_R32G32B32A32_SINT,
		//UByte4, 
		DXGI_FORMAT_B8G8R8A8_TYPELESS,
		DXGI_FORMAT_B8G8R8A8_UNORM,
		DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,
		DXGI_FORMAT_B8G8R8X8_TYPELESS,
		DXGI_FORMAT_B8G8R8X8_UNORM,
		DXGI_FORMAT_B8G8R8X8_UNORM_SRGB,
		//depth, stencil
		DXGI_FORMAT_D16_UNORM,
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		DXGI_FORMAT_D32_FLOAT,
	};

	DXGI_FORMAT GetDXGIFormat(DataFormat format)
	{
		return g_FormatTable[format];
	}

	struct InverseFormatTable
	{
		std::map<DXGI_FORMAT, DataFormat> DXGIFormatTable;

		InverseFormatTable()
		{
			for (uint i = DF_UNKNOWN; i < DF_Max; ++i)
			{
				DXGIFormatTable[g_FormatTable[i]] = (DataFormat)i;
			}
		}

		DataFormat GetByDXGIFormat(DXGI_FORMAT format)
		{
			try
			{
				return DXGIFormatTable.at(format);
			}
			catch (std::out_of_range)
			{
				return DF_UNKNOWN;
			}
		}
	};

	static InverseFormatTable g_InverseFormatTable;

	DataFormat GetDataFormat(DXGI_FORMAT format)
	{
		return g_InverseFormatTable.GetByDXGIFormat(format);
	}


	D3D11_BIND_FLAG GetD3D11BindFlag(ResourceBindFlag flag)
	{
		D3D11_BIND_FLAG ret = (D3D11_BIND_FLAG)0;
		if (flag & RB_ShaderResource)
		{
			ret = (D3D11_BIND_FLAG) (ret | D3D11_BIND_SHADER_RESOURCE);
		}
		if (flag & RB_DepthStencil)
		{
			ret = (D3D11_BIND_FLAG)(ret | D3D11_BIND_DEPTH_STENCIL);
		}
		if (flag & RB_RenderTarget)
		{
			ret = (D3D11_BIND_FLAG)(ret | D3D11_BIND_RENDER_TARGET);
		}
		return ret;
	}

	ResourceBindFlag GetBindFlag(UINT flag)
	{
		ResourceBindFlag ret = RB_None;
		if (flag & D3D11_BIND_SHADER_RESOURCE)
		{
			ret = (ResourceBindFlag)(ret | RB_ShaderResource);
		}
		if (flag & D3D11_BIND_DEPTH_STENCIL)
		{
			ret = (ResourceBindFlag)(ret | RB_DepthStencil);
		}
		if (flag & D3D11_BIND_RENDER_TARGET)
		{
			ret = (ResourceBindFlag)(ret | RB_RenderTarget);
		}
		return ret;
	}
	
#define MAP_ELEM_FORMAT(type, format) format
	DXGI_FORMAT g_ElemDXGIFormatTable[] = {
		MAP_ELEM_FORMAT(None, DXGI_FORMAT_UNKNOWN),
		MAP_ELEM_FORMAT(Float1, DXGI_FORMAT_R32_FLOAT),
		MAP_ELEM_FORMAT(Float2, DXGI_FORMAT_R32G32_FLOAT),
		MAP_ELEM_FORMAT(Float3, DXGI_FORMAT_R32G32B32_FLOAT),
		MAP_ELEM_FORMAT(Float4, DXGI_FORMAT_R32G32B32A32_FLOAT),
		MAP_ELEM_FORMAT(Byte2, DXGI_FORMAT_R8G8_SINT),
		MAP_ELEM_FORMAT(Byte4, DXGI_FORMAT_R8G8B8A8_SINT),
		MAP_ELEM_FORMAT(ByteN2, DXGI_FORMAT_R8G8_SNORM),
		MAP_ELEM_FORMAT(ByteN4, DXGI_FORMAT_R8G8B8A8_SNORM),
		MAP_ELEM_FORMAT(UByte2, DXGI_FORMAT_R8G8_UINT),
		MAP_ELEM_FORMAT(UByte4, DXGI_FORMAT_R8G8B8A8_UINT),
		MAP_ELEM_FORMAT(UByteN2, DXGI_FORMAT_R8G8_UNORM),
		MAP_ELEM_FORMAT(UByteN4, DXGI_FORMAT_R8G8B8A8_UNORM),
		MAP_ELEM_FORMAT(UShort2, DXGI_FORMAT_R16G16_UINT),
		MAP_ELEM_FORMAT(UShort4, DXGI_FORMAT_R16G16B16A16_UINT),
		MAP_ELEM_FORMAT(UShortN2, DXGI_FORMAT_R16G16_UNORM),
		MAP_ELEM_FORMAT(UShortN4, DXGI_FORMAT_R16G16B16A16_UNORM),
		MAP_ELEM_FORMAT(Short2, DXGI_FORMAT_R16G16_SINT),
		MAP_ELEM_FORMAT(Short4, DXGI_FORMAT_R16G16B16A16_SINT),
		MAP_ELEM_FORMAT(ShortN2, DXGI_FORMAT_R16G16_SNORM),
		MAP_ELEM_FORMAT(ShortN4, DXGI_FORMAT_R16G16B16A16_SNORM),
		MAP_ELEM_FORMAT(Half2, DXGI_FORMAT_R16G16_FLOAT),
		MAP_ELEM_FORMAT(Half4, DXGI_FORMAT_R16G16B16A16_FLOAT),
		MAP_ELEM_FORMAT(Color, DXGI_FORMAT_B8G8R8A8_UNORM_SRGB),
		MAP_ELEM_FORMAT(MAX, DXGI_FORMAT_UNKNOWN),
	};
#undef MAP_ELEM_FORMAT 


}