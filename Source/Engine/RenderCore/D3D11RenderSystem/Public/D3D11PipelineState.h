#pragma once

#include "D3D11Prerequisites.h"

namespace Space
{
namespace Render
{
	struct InputElement;
	struct BlendStateDescriptor;
	struct DepthStencilStateDescriptor;
	struct RasterizerStateDescriptor;

	class RENDERCORE_API D3D11PipelineState
	{
	public:
		D3D11PipelineState(D3D11Device* device);

		void _ClearAllState();

		void SetInputLayout(InputElement const* input, size_t elementCount);

		void SetBlendState(BlendStateDescriptor BlendState);

		void SetRasterizerState(RasterizerStateDescriptor RasterState);

		void SetDepthStencilState(DepthStencilStateDescriptor depthStencilState);

	private:
		D3D11Device* m_Device;

		CComPtr<ID3D11BlendState> m_pBlendState = nullptr;
		CComPtr<ID3D11InputLayout> m_pInputLayout = nullptr;
		CComPtr<ID3D11DepthStencilState> m_pDepthStencilState = nullptr;
		CComPtr<ID3D11RasterizerState> m_pRasterizerState = nullptr;
		std::vector<D3D11_INPUT_ELEMENT_DESC> m_ElemArray;

		friend class D3D11CommandList;
	};
}
}