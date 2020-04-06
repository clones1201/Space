#pragma once

#include "RenderCommon.hpp"
#include "D3D11Prerequisites.hpp"

namespace Space {
namespace Render {
	SPACE_D3D11_API UINT GetD3D11BufferBindFlags(BufferType type);

	SPACE_D3D11_API D3D11_USAGE GetD3D11Usage(ResourceUsage usage);

	SPACE_D3D11_API DXGI_FORMAT GetDXGIFormat(DataFormat format);

	SPACE_D3D11_API DataFormat GetDataFormat(DXGI_FORMAT format);

	SPACE_D3D11_API D3D11_BIND_FLAG GetD3D11BindFlag(ResourceBindFlag flags);

	SPACE_D3D11_API ResourceBindFlag GetBindFlag(UINT flags);

	extern DXGI_FORMAT g_ElemDXGIFormatTable[];

	SPACE_D3D11_API inline DXGI_FORMAT GetElemDXGIFormat(VertexElemType type)
	{
		return g_ElemDXGIFormatTable[(uint16)type];
	}
}
}
