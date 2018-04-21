#include "Common/Log.h"
#include "D3D11RenderSystem/D3D11Shared.hpp"

namespace Space
{
	namespace Render {
		UINT GetD3D11BufferBindFlags(BufferType type)
		{
			switch (type)
			{
			case BufferType::VertexBuffer:
				return D3D11_BIND_VERTEX_BUFFER;
			case BufferType::IndexBuffer:
				return D3D11_BIND_INDEX_BUFFER;
			case BufferType::ConstantBuffer:
				return D3D11_BIND_CONSTANT_BUFFER;
			case BufferType::TextureBuffer:
				return D3D11_BIND_SHADER_RESOURCE;
			}
			return 0;
		}

		D3D11_USAGE GetD3D11Usage(ResourceUsage usage)
		{
			switch (usage)
			{
			case ResourceUsage::Default:
				return D3D11_USAGE_DEFAULT;
			case ResourceUsage::Dynamic:
				return D3D11_USAGE_DYNAMIC;
			case ResourceUsage::Immutable:
				return D3D11_USAGE_IMMUTABLE;
			case ResourceUsage::Staging:
				return D3D11_USAGE_STAGING;
			default:
				return D3D11_USAGE_DEFAULT;
			}
		}

		static DXGI_FORMAT g_FormatTable[] =
		{
			DXGI_FORMAT_UNKNOWN,
			//half, short
			DXGI_FORMAT_R16_TYPELESS,
			DXGI_FORMAT_R16_FLOAT,
			DXGI_FORMAT_R16_UNORM,
			DXGI_FORMAT_R16_UINT,
			DXGI_FORMAT_R16_SNORM,
			DXGI_FORMAT_R16_SINT,
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
			return g_FormatTable[(uint)format];
		}

		struct InverseFormatTable
		{
			std::map<DXGI_FORMAT, DataFormat> DXGIFormatTable;

			InverseFormatTable()
			{
				for (uint i = (uint)DataFormat::UNKNOWN; i < (uint)DataFormat::Max; ++i)
				{
					DXGIFormatTable[g_FormatTable[i]] = (DataFormat)i;
				}
			}

			DataFormat GetByDXGIFormat(DXGI_FORMAT format)
			{
				TRY_CATCH_OUT_OF_RANGE(return DXGIFormatTable.at(format), return DataFormat::UNKNOWN);
			}
		};

		static InverseFormatTable g_InverseFormatTable;

		DataFormat GetDataFormat(DXGI_FORMAT format)
		{
			return g_InverseFormatTable.GetByDXGIFormat(format);
		}


		D3D11_BIND_FLAG GetD3D11BindFlag(ResourceBindFlag flag)
		{
			auto ret = 0;
			if ((uint8)flag & (uint8)ResourceBindFlag::ShaderResource)
			{
				ret = ret | D3D11_BIND_SHADER_RESOURCE;
			}
			if ((uint8)flag & (uint8)ResourceBindFlag::DepthStencil)
			{
				ret = ret | D3D11_BIND_DEPTH_STENCIL;
			}
			if ((uint8)flag & (uint8)ResourceBindFlag::RenderTarget)
			{
				ret = ret | D3D11_BIND_RENDER_TARGET;
			}
			return (D3D11_BIND_FLAG)ret;
		}

		ResourceBindFlag GetBindFlag(UINT flag)
		{
			auto ret = (uint8)ResourceBindFlag::None;
			if (flag & D3D11_BIND_SHADER_RESOURCE)
			{
				ret = (ret | (uint)ResourceBindFlag::ShaderResource);
			}
			if (flag & D3D11_BIND_DEPTH_STENCIL)
			{
				ret = (ret | (uint)ResourceBindFlag::DepthStencil);
			}
			if (flag & D3D11_BIND_RENDER_TARGET)
			{
				ret = (ret | (uint)ResourceBindFlag::RenderTarget);
			}
			return (ResourceBindFlag)ret;
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
}