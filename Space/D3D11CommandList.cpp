#include "Log.h"
#include "D3D11Shared.hpp"
#include "D3D11DeviceBuffer.hpp"
#include "D3D11Rendering.hpp"
#include "D3D11Shader.hpp"
#include "D3D11RenderTarget.hpp"
#include "D3D11DepthStencilView.hpp"

namespace Space
{
	D3D11CommandList* D3D11CommandList::Create(D3D11DevicePtr device)
	{
		TRY_CATCH_LOG(
			return new D3D11CommandList(device),
			return nullptr;
		);
	}

	D3D11CommandList::D3D11CommandList(D3D11DevicePtr device)
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

	void D3D11CommandList::ClearDepth(
		DepthStencilView* pDepth, float depth)
	{
		assert(nullptr != dynamic_cast<D3D11DepthStencilView*>(pDepth));
		m_pDeferredContext->ClearDepthStencilView(
			static_cast<D3D11DepthStencilView*>(pDepth)->GetDepthStencilView(),
			D3D11_CLEAR_DEPTH,
			depth, 0);
	}

	void D3D11CommandList::ClearStencil(
		DepthStencilView* pStencil, uint8 stencil)
	{
		assert(nullptr != dynamic_cast<D3D11DepthStencilView*>(pStencil));
		m_pDeferredContext->ClearDepthStencilView(
			static_cast<D3D11DepthStencilView*>(pStencil)->GetDepthStencilView(),
			D3D11_CLEAR_STENCIL,
			0, stencil);
	}

	void D3D11CommandList::ClearRenderTargetView(RenderTarget* pTarget, Float4 clearColor)
	{
		assert(nullptr != dynamic_cast<D3D11RenderTarget*>(pTarget));
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
			throw std::exception("FinishCommandList failed.");
		}
	}

	void D3D11CommandList::SetIndexBuffer(IndexBuffer const* buffer)
	{
		if (buffer == nullptr) return;

		assert(nullptr != dynamic_cast<D3D11DeviceBuffer*>(buffer->GetBuffer()));
		m_pIndexBuffer = buffer;

		auto pIndexBuffer = static_cast<D3D11DeviceBuffer*>(buffer->GetBuffer());
		m_pDeferredContext->IASetIndexBuffer(
			pIndexBuffer->GetRawBuffer(), GetDXGIFormat(buffer->GetFormat()), buffer->GetOffest());
	}
	void D3D11CommandList::SetVertexBuffers(
		uint startSlot, uint numBuffers, VertexBuffer *const* buffer)
	{
		if (numBuffers == 0) return;
		assert(buffer != nullptr && buffer + numBuffers != nullptr);

		std::vector<ID3D11Buffer*> vbuffers;
		std::vector<uint> striders;
		std::vector<uint> offsets;
		vbuffers.reserve(numBuffers);
		striders.reserve(numBuffers);
		offsets.reserve(numBuffers);
		m_VertexBufferArray.clear();
		m_VertexBufferArray.reserve(numBuffers);
		for (; buffer != buffer + numBuffers; buffer++)
		{
			assert(nullptr != buffer);
			assert(nullptr != dynamic_cast<D3D11DeviceBuffer*>((*buffer)->GetBuffer()));
			m_VertexBufferArray.push_back(*buffer);
			vbuffers.push_back(static_cast<D3D11DeviceBuffer*>((*buffer)->GetBuffer())->GetRawBuffer());
			striders.push_back((*buffer)->GetStride());
			striders.push_back((*buffer)->GetOffest());
		}

		m_pDeferredContext->IASetVertexBuffers(
			startSlot, numBuffers, vbuffers.data(), striders.data(), offsets.data());

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

	void D3D11CommandList::SetPipelineState(PipelineState const* pState)
	{
		typedef std::vector<ID3D11Buffer*> ID3D11BufferPtrArray;
		typedef std::vector<ID3D11ShaderResourceView*> ID3D11ShaderResourceViewPtrArray;

		if (pState == nullptr)
			return;

		assert(nullptr != dynamic_cast<D3D11PipelineState const*>(pState));
		auto pPipelineState = static_cast<D3D11PipelineState const*>(pState);

		// Set Input-Assembly Stage
		m_pDeferredContext->IASetInputLayout(
			(pPipelineState->m_pInputLayout.p)
			);

		m_pDeferredContext->IASetPrimitiveTopology(
			(D3D11_PRIMITIVE_TOPOLOGY)pPipelineState->m_PrimTopology
			);

		// Set VertexShader Stage
		m_pDeferredContext->VSSetShader(pPipelineState->m_pVS, nullptr, 0);
		ID3D11BufferPtrArray vsContantBufferPtrArray(
			pPipelineState->m_VSConstantBuffers.begin(),
			pPipelineState->m_VSConstantBuffers.end());
		m_pDeferredContext->VSSetConstantBuffers(
			0,
			vsContantBufferPtrArray.size(),
			vsContantBufferPtrArray.data());
		ID3D11ShaderResourceViewPtrArray vsSRVArray(
			(pPipelineState->m_VSSRVs.begin()),
			(pPipelineState->m_VSSRVs.end()));
		m_pDeferredContext->VSSetShaderResources(
			0,
			vsSRVArray.size(),
			vsSRVArray.data());

		// Set Rasterizer Stage
		m_pDeferredContext->RSSetState(
			(pPipelineState->m_pRasterizerState.p)
			);

		// Set PixelShader Stage
		m_pDeferredContext->PSSetShader(pPipelineState->m_pPS, nullptr, 0);
		ID3D11BufferPtrArray psCBPtrArray(
			(pPipelineState->m_PSConstantBuffers.begin()),
			(pPipelineState->m_PSConstantBuffers.end()));
		m_pDeferredContext->PSSetConstantBuffers(
			0,
			vsContantBufferPtrArray.size(),
			vsContantBufferPtrArray.data());
		ID3D11ShaderResourceViewPtrArray psSRVArray(
			(pPipelineState->m_PSSRVs.begin()),
			(pPipelineState->m_PSSRVs.end()));
		m_pDeferredContext->PSSetShaderResources(
			0,
			psSRVArray.size(),
			psSRVArray.data());

		// Set Output-Merge Stage
		m_pDeferredContext->OMSetBlendState(
			(pPipelineState->m_pBlendState.p),
			(float*)&pPipelineState->m_BlendFactor,
			pPipelineState->m_SampleMask
			);

		m_pDeferredContext->OMSetDepthStencilState(
			(pPipelineState->m_pDepthStencilState.p),
			pPipelineState->m_StencilRef
			);
	}

	void D3D11CommandList::SetRenderTargets(
		RenderTarget *const* targets, uint32 numTargets, DepthStencilView const* depth)
	{
		assert((RenderTarget*)(targets + numTargets - 1) != nullptr);

		std::vector<ID3D11RenderTargetView*> renderTargetArray;
		renderTargetArray.reserve(numTargets);
		for (; targets != targets + numTargets; ++targets)
		{
			assert(nullptr != dynamic_cast<D3D11RenderTarget const*>(*targets));
			auto pTarget = static_cast<D3D11RenderTarget const*>(*targets);
			renderTargetArray.push_back(pTarget->GetRenderTargetView());
		}
		//pDepth is optional
		ID3D11DepthStencilView* pDepth = nullptr;
		if (depth != nullptr)
		{
			assert(nullptr != dynamic_cast<D3D11DepthStencilView const*>(depth));
			pDepth = static_cast<D3D11DepthStencilView const*>(depth)->GetDepthStencilView();
		}
		m_pDeferredContext->OMSetRenderTargets(
			numTargets, renderTargetArray.data(),
			pDepth);
	}
	void D3D11CommandList::DrawIndexed(uint startIndex, uint numPrimitive)
	{

	}
	void D3D11CommandList::DrawIndexedInstanced(
		uint startIndex, uint numPrimitive, uint startInstance, uint numInstance)
	{

	}

}