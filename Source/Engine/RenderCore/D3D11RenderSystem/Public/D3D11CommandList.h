#pragma once

#include "RenderCommon.h"
#include "D3D11Prerequisites.h"

namespace Space
{
namespace Render
{
	struct Rect;
	struct ViewPort;
	enum class DataFormat : unsigned char;

	class RENDERCORE_API D3D11CommandList
	{
	public:
		D3D11CommandList(D3D11Device* device);

		CComPtr<ID3D11CommandList> GetList() const;
		CComPtr<ID3D11DeviceContext> GetContext() const;

		void ClearDepth(
			D3D11DepthStencilView* pDepth, float depth);

		void ClearStencil(
			D3D11DepthStencilView* pStencil, uint8 stencil);

		void ClearRenderTargetView(D3D11RenderTarget* pTarget, Float4 clearColor);

		void Reset();

		void Close();
		void SetIndexBuffer(D3D11DeviceBuffer* buffer, DataFormat format, int32 offset);
		void SetVertexBuffers(
			uint startSlot, std::vector<D3D11DeviceBuffer*> const& buffers, std::vector<int32> const& strides, std::vector<int32> const& offsets);

		void SetViewPorts(ViewPort const* pViewPorts, uint numViewPorts);

		void SetScissorRects(Rect const* rects, uint numRects);

		void SetVertexShader(D3D11VertexShader const* shader);

		void SetPixelShader(D3D11PixelShader const* shader);

		void SetPipelineState(D3D11PipelineState const* pState);

		void SetRenderTargets(
			std::vector<D3D11RenderTarget*> const& targets, D3D11DepthStencilView* depth);

		void DrawIndexed(uint startIndex, uint numPrimitive);

		void DrawIndexedInstanced(
			uint startIndex, uint numPrimitive, uint startInstance, uint numInstance);
	private:
		D3D11Device* device;

		CComPtr<ID3D11DeviceContext> m_pDeferredContext;
		CComPtr<ID3D11CommandList> m_pCommandList;

		friend class D3D11PipelineState;

	};
}
}