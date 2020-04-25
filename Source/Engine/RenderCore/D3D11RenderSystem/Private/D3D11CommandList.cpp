#include "D3D11Shared.h"
#include "D3D11DeviceResource.h"
#include "D3D11PipelineState.h"
#include "D3D11CommandList.h"
#include "D3D11Shader.h" 
#include "D3D11RenderTarget.h"

namespace Space {
namespace Render {
	D3D11CommandList::D3D11CommandList(D3D11Device* device)
		:device(device)
	{
		m_pDeferredContext = nullptr;
		HRESULT hr = device->Get()->CreateDeferredContext(0, &m_pDeferredContext);
		if (FAILED(hr))
		{
			throw std::exception("CreateDeferredContext failed.");
		}
		assert(m_pDeferredContext != nullptr &&
			m_pDeferredContext->GetType() == D3D11_DEVICE_CONTEXT_DEFERRED);
	}
	
	CComPtr<ID3D11CommandList> D3D11CommandList::GetList() const
	{
		return m_pCommandList;
	}

	CComPtr<ID3D11DeviceContext> D3D11CommandList::GetContext() const
	{
		return m_pDeferredContext;
	}

	void D3D11CommandList::ClearDepth(
		D3D11DepthStencilView* pDepth, float depth)
	{
		m_pDeferredContext->ClearDepthStencilView(
			pDepth->GetDepthStencilView(),
			D3D11_CLEAR_DEPTH,
			depth, 0);
	}

	void D3D11CommandList::ClearStencil(
		D3D11DepthStencilView* pStencil, uint8 stencil)
	{
		m_pDeferredContext->ClearDepthStencilView(
			pStencil->GetDepthStencilView(),
			D3D11_CLEAR_STENCIL,
			0, stencil);
	}

	void D3D11CommandList::ClearRenderTargetView(D3D11RenderTarget* pTarget, Float4 clearColor)
	{
		m_pDeferredContext->ClearRenderTargetView(
			static_cast<D3D11RenderTarget*>(pTarget)->GetRenderTargetView(),
			(float*)&clearColor);
	}

	void D3D11CommandList::Reset()
	{
		HRESULT hr = m_pDeferredContext->FinishCommandList(
			TRUE,
			nullptr);
		if (FAILED(hr))
		{
			throw std::exception("FinishCommandList failed.");
		}
	}

	void D3D11CommandList::Close()
	{
		m_pCommandList = nullptr;
		HRESULT hr = m_pDeferredContext->FinishCommandList(
			TRUE,
			&m_pCommandList);
		if (FAILED(hr))
		{
			hr = device->Get()->GetDeviceRemovedReason();
			throw std::exception("FinishCommandList failed.");
		}
	}

	void D3D11CommandList::SetIndexBuffer(D3D11DeviceBuffer* buffer, DataFormat format, int32 offset)
	{
		if (buffer == nullptr) return;

		m_pDeferredContext->IASetIndexBuffer(
			buffer->GetRawPtr(), GetDXGIFormat(format), (UINT)offset);
	}
	void D3D11CommandList::SetVertexBuffers(
		uint startSlot, std::vector<D3D11DeviceBuffer*> const& buffers, std::vector<int32> const& strides, std::vector<int32> const& offsets)
	{
		if (buffers.size() == 0) return;
		if (buffers.size() != strides.size() || buffers.size() != offsets.size()) return;
		std::vector<ID3D11Buffer*> vbuffers;
		vbuffers.reserve(buffers.size());
		//m_VertexBufferArray.reserve(numBuffers);
		for (auto iter = buffers.cbegin(); iter != buffers.cend(); iter++)
		{
			assert(nullptr != *iter);
			//m_VertexBufferArray.push_back(*iter);
			vbuffers.push_back((*iter)->GetRawPtr());
		}

		m_pDeferredContext->IASetVertexBuffers(
			(UINT)startSlot, (UINT)vbuffers.size(), vbuffers.data(), (UINT*)strides.data(), (UINT*)offsets.data());

	}

	void D3D11CommandList::SetViewPorts(ViewPort const* pViewPorts, uint numViewPorts)
	{
		m_pDeferredContext->RSSetViewports(
			numViewPorts,
			(D3D11_VIEWPORT*)pViewPorts
		);
	}

	void D3D11CommandList::SetScissorRects(Rect const* rects, uint numRects)
	{
		m_pDeferredContext->RSSetScissorRects(
			numRects,
			(D3D11_RECT*)rects
		);
	}

	void D3D11CommandList::SetVertexShader(D3D11VertexShader const* shader)
	{
		CComPtr<ID3D11VertexShader> _vs = shader->GetShader();
		m_pDeferredContext->VSSetShader(_vs.p, nullptr, 0);
	}
	
	void D3D11CommandList::SetPixelShader(D3D11PixelShader const* shader)
	{
		CComPtr<ID3D11PixelShader> _ps = shader->GetShader();
		m_pDeferredContext->PSSetShader(_ps.p, nullptr, 0);
	}
	
	void D3D11CommandList::SetPipelineState(D3D11PipelineState const* pState)
	{
		typedef std::vector<ID3D11Buffer*> ID3D11BufferPtrArray;
		typedef std::vector<ID3D11ShaderResourceView*> ID3D11ShaderResourceViewPtrArray;

		if (pState == nullptr)
			return;
		auto pPipelineState = pState;
		// Set Input-Assembly Stage
		m_pDeferredContext->IASetInputLayout(
			(pPipelineState->m_pInputLayout.p)
		);

		m_pDeferredContext->IASetPrimitiveTopology(
			D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST
		);
//
//			// Set VertexShader Stage
//			m_pDeferredContext->VSSetShader(pPipelineState->m_pVS, nullptr, 0);
//			ID3D11BufferPtrArray vsContantBufferPtrArray(
//				pPipelineState->m_VSConstantBuffers.begin(),
//				pPipelineState->m_VSConstantBuffers.end());
//			m_pDeferredContext->VSSetConstantBuffers(
//				0,
//				vsContantBufferPtrArray.size(),
//				vsContantBufferPtrArray.data());
//			ID3D11ShaderResourceViewPtrArray vsSRVArray(
//				(pPipelineState->m_VSSRVs.begin()),
//				(pPipelineState->m_VSSRVs.end()));
//			m_pDeferredContext->VSSetShaderResources(
//				0,
//				vsSRVArray.size(),
//				vsSRVArray.data());
//
		// Set Rasterizer Stage
		m_pDeferredContext->RSSetState(
			(pPipelineState->m_pRasterizerState.p)
		);
//
//			// Set PixelShader Stage
//			m_pDeferredContext->PSSetShader(pPipelineState->m_pPS, nullptr, 0);
//			ID3D11BufferPtrArray psCBPtrArray(
//				(pPipelineState->m_PSConstantBuffers.begin()),
//				(pPipelineState->m_PSConstantBuffers.end()));
//			m_pDeferredContext->PSSetConstantBuffers(
//				0,
//				vsContantBufferPtrArray.size(),
//				vsContantBufferPtrArray.data());
//			ID3D11ShaderResourceViewPtrArray psSRVArray(
//				(pPipelineState->m_PSSRVs.begin()),
//				(pPipelineState->m_PSSRVs.end()));
//			m_pDeferredContext->PSSetShaderResources(
//				0,
//				psSRVArray.size(),
//				psSRVArray.data());
//
		// Set Output-Merge Stage
		m_pDeferredContext->OMSetBlendState(
			(pPipelineState->m_pBlendState.p),
			nullptr,
			0xffffffff
		);

		m_pDeferredContext->OMSetDepthStencilState(
			(pPipelineState->m_pDepthStencilState.p),
			0xff
		);
	}		

	void D3D11CommandList::SetRenderTargets(
		std::vector<D3D11RenderTarget*> const& targets, D3D11DepthStencilView* depth)
	{
		std::vector<ID3D11RenderTargetView*> renderTargetArray;
		renderTargetArray.reserve(targets.size());
		for (auto iter = targets.cbegin(); iter != targets.cend(); ++iter)
		{
			auto pTarget = *iter;
			renderTargetArray.push_back(pTarget->GetRenderTargetView());
		}
		//pDepth is optional
		ID3D11DepthStencilView* pDepth = nullptr;
		if (depth != nullptr)
		{
			pDepth = depth->GetDepthStencilView();
		}
		m_pDeferredContext->OMSetRenderTargets(
			(UINT)targets.size(), renderTargetArray.data(),
			pDepth);
	}
	void D3D11CommandList::DrawIndexed(uint startIndex, uint numPrimitive)
	{
		m_pDeferredContext->DrawIndexed(3 * numPrimitive, startIndex, 0);
	}
	void D3D11CommandList::DrawIndexedInstanced(
		uint startIndex, uint numPrimitive, uint startInstance, uint numInstance)
	{
		throw std::exception("Not Implemented");
	}
}
}