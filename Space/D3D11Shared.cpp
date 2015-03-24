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

	static DXGI_FORMAT g_iFormatMap[] = 
	{
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_R32G32B32A32_TYPELESS,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_R32G32B32A32_UINT,
		DXGI_FORMAT_R32G32B32A32_SINT,
		DXGI_FORMAT_R32G32B32_TYPELESS,
		DXGI_FORMAT_R32G32B32_FLOAT,
		DXGI_FORMAT_R32G32B32_UINT,
		DXGI_FORMAT_R32G32B32_SINT,
		DXGI_FORMAT_R16G16B16A16_TYPELESS,
		DXGI_FORMAT_R16G16B16A16_FLOAT,
		DXGI_FORMAT_R16G16B16A16_UNORM,
		DXGI_FORMAT_R16G16B16A16_UINT,
		DXGI_FORMAT_R16G16B16A16_SNORM,
		DXGI_FORMAT_R16G16B16A16_SINT,
		DXGI_FORMAT_B8G8R8A8_UNORM,
		DXGI_FORMAT_B8G8R8X8_UNORM,
		DXGI_FORMAT_B8G8R8A8_TYPELESS,
		DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,
		DXGI_FORMAT_B8G8R8X8_TYPELESS,
		DXGI_FORMAT_B8G8R8X8_UNORM_SRGB,
		DXGI_FORMAT_D24_UNORM_S8_UINT
	};

	DXGI_FORMAT GetDXGIFormat(DataFormat format)
	{
		return g_iFormatMap[format];
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
}