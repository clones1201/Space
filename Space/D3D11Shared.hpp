#ifndef __SPACE_D3D11_SHARED_HPP__
#define __SPACE_D3D11_SHARED_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"
#include "D3D11Prerequisites.hpp"

namespace Space
{	
	UINT GetD3D11BufferBindFlags(BufferType type);

	D3D11_USAGE GetD3D11Usage(ResourceUsage usage);

	DXGI_FORMAT GetDXGIFormat(DataFormat format);

	DataFormat GetDataFormat(DXGI_FORMAT format);

	D3D11_BIND_FLAG GetD3D11BindFlag(ResourceBindFlag flags);
	
	ResourceBindFlag GetBindFlag(UINT flags);

	extern DXGI_FORMAT g_ElemDXGIFormatTable[];

	inline DXGI_FORMAT GetElemDXGIFormat(VertexElemType type)
	{
		return g_ElemDXGIFormatTable[(uint16)type];
	}

}

#endif