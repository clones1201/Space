#include "Log.h"
#include "D3D11Rendering.hpp"
#include "D3D11InputLayout.hpp"
#include "D3D11Shader.hpp"
#include "D3D11RenderTarget.hpp"
#include "D3D11DepthStencilView.hpp"

namespace Space
{ 
	D3D11CommandList* D3D11CommandList::Create(D3D11Device& device)
	{
		TRY_CATCH_LOG(
			return new D3D11CommandList(device),
			return nullptr;
		);
	}

	D3D11CommandList::D3D11CommandList(D3D11Device& device)
		:device(device)
	{
		m_pDeferredContext = nullptr;
		HRESULT hr = device->CreateDeferredContext(0, &m_pDeferredContext);
		if (FAILED(hr))
		{
			throw std::exception("CreateDeferredContext failed.");
		}
		assert(m_pDeferredContext != nullptr &&
			m_pDeferredContext->GetType() == D3D11_DEVICE_CONTEXT_DEFERRED);
	}
	
	void D3D11CommandList::_ClearDepth(
		DepthStencilView* pDepth, float depth)
	{
		assert(nullptr != dynamic_cast<D3D11DepthStencilView*>(pDepth));
		m_pDeferredContext->ClearDepthStencilView(
			static_cast<D3D11DepthStencilView*>(pDepth)->GetDepthStencilView(),
			D3D11_CLEAR_DEPTH,
			depth, 0);
	}

	void D3D11CommandList::_ClearStencil(
		DepthStencilView* pStencil, uint8 stencil)
	{
		assert(nullptr != dynamic_cast<D3D11DepthStencilView*>(pStencil));
		m_pDeferredContext->ClearDepthStencilView(
			static_cast<D3D11DepthStencilView*>(pStencil)->GetDepthStencilView(),
			D3D11_CLEAR_STENCIL,
			0,stencil);
	}

	void D3D11CommandList::_ClearRenderTargetView(RenderTarget* pTarget, Float4 clearColor)
	{
		assert(nullptr != dynamic_cast<D3D11RenderTarget*>(pTarget));
		m_pDeferredContext->ClearRenderTargetView(
			static_cast<D3D11RenderTarget*>(pTarget)->GetRenderTargetView(),
			(float*)&clearColor);
	}
	
	void D3D11CommandList::_Reset()
	{
		HRESULT hr = m_pDeferredContext->FinishCommandList(
			TRUE,
			nullptr);
		if (FAILED(hr))
		{
			throw std::exception("FinishCommandList failed.");
		}

	}

	void D3D11CommandList::_Close()
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
	
	void D3D11CommandList::_SetIndexBuffer()
	{

	}
	void D3D11CommandList::_SetVertexBuffers()
	{

	}

	void D3D11CommandList::_SetViewPorts(ViewPort* pViewPorts, uint numViewPorts)
	{
		m_pDeferredContext->RSSetViewports(
			numViewPorts,
			(D3D11_VIEWPORT*)pViewPorts	
			);
	}

	void D3D11CommandList::_SetScissorRects(Rect* rects, uint numRects)
	{
		m_pDeferredContext->RSSetScissorRects(
			numRects,
			(D3D11_RECT*)rects
		);
	}
	
	void D3D11CommandList::_SetPipelineState(PipelineState* pState)
	{
		typedef std::vector<ID3D11Buffer*> ID3D11BufferPtrArray;
		typedef std::vector<ID3D11ShaderResourceView*> ID3D11ShaderResourceViewPtrArray;

		if (pState == nullptr)
			return ;

		assert(nullptr != dynamic_cast<D3D11PipelineState*>(pState));
 		pPipelineState = static_cast<D3D11PipelineState*>(pState);
		
		// Set Input-Assembly Stage
		m_pDeferredContext->IASetInputLayout(
			(pPipelineState->m_pInputLayout.p)
			);

		m_pDeferredContext->IASetPrimitiveTopology(
			(D3D11_PRIMITIVE_TOPOLOGY)pPipelineState->m_PrimTopology
			);

		// Set VertexShader Stage
		m_pDeferredContext->VSSetShader(pPipelineState->m_pVS,nullptr,0);
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

	void D3D11CommandList::_SetRenderTargets(
		RenderTarget* targets, uint32 numTargets, DepthStencilView* depth)
	{
		assert((RenderTarget*)(targets + numTargets - 1) != nullptr);
		
		std::vector<ID3D11RenderTargetView*> renderTargetArray;
		renderTargetArray.reserve(numTargets);
		for (; targets != targets + numTargets; ++targets)
		{
			assert(nullptr != dynamic_cast<D3D11RenderTarget*>(targets));
			auto pTarget = static_cast<D3D11RenderTarget*>(targets);
			renderTargetArray.push_back(pTarget->GetRenderTargetView());
		}
		//pDepth is optional
		ID3D11DepthStencilView* pDepth = nullptr;
		if (depth != nullptr)
		{
			assert(nullptr != dynamic_cast<D3D11DepthStencilView*>(depth));
			pDepth = static_cast<D3D11DepthStencilView*>(depth)->GetDepthStencilView();
		}
		m_pDeferredContext->OMSetRenderTargets(
			numTargets,renderTargetArray.data(),
			pDepth);
	}
	void D3D11CommandList::_DrawIndexed()
	{

	}
	void D3D11CommandList::_DrawInstanced()
	{

	}
	void D3D11CommandList::_DrawIndexedInstanced()
	{

	}

}