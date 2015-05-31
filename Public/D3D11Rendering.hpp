#ifndef __SPACE_D3D11_RENDERING_HPP__
#define __SPACE_D3D11_RENDERING_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"
#include "Material.hpp"
#include "Rendering.hpp"
#include "D3D11Device.hpp"
#include "D3D11RenderSystem.hpp"

namespace Space
{

	class D3D11CommandList : public CommandList
	{
	public:
		static D3D11CommandList* Create(D3D11Device& device);
		
	protected:
	private:
		D3D11CommandList(D3D11Device& device);

		virtual void _ClearDepth(DepthStencilView* pDepth, float depth);
		virtual void _ClearStencil(DepthStencilView* pStencil, uint8 stencil);
		virtual void _ClearRenderTargetView(RenderTarget* pTarget, Float4 clearColor);
		virtual void _Reset();
		virtual void _Close();
		virtual void _SetIndexBuffer();
		virtual void _SetVertexBuffers(); 
		virtual void _SetViewPorts(ViewPort* pViewPorts, uint numViewPorts);
		virtual void _SetScissorRects(Rect* rects, uint32 numRects);

		virtual void _SetPipelineState(PipelineState* state);
		virtual void _SetRenderTargets(RenderTarget* targets, uint32 numTargets, DepthStencilView* pDepth);
		virtual void _DrawIndexed();
		virtual void _DrawInstanced();
		virtual void _DrawIndexedInstanced();

		std::vector<D3D11_VIEWPORT> m_Viewports;
		std::vector<D3D11_RECT> m_ScissorRects;
		
		CComPtr<ID3D11DeviceContext> m_pDeferredContext = nullptr;
		CComPtr<ID3D11CommandList> m_pCommandList = nullptr;

		D3D11Device& device;
		D3D11PipelineState* pPipelineState;
	};

	typedef std::vector<CComPtr<ID3D11Buffer>> D3D11BufferArray;
	typedef std::vector<CComPtr<ID3D11ShaderResourceView>> D3D11SRVArray;
	
	class D3D11PipelineState : public PipelineState
	{
	public:
		static D3D11PipelineState* Create(D3D11Device& device);

		D3D11PipelineState(D3D11PipelineState const& other) = default;
		D3D11PipelineState& operator=(D3D11PipelineState const& other) = default;

		virtual void SetShaders(Shader* shader);
		virtual void SetInputLayout(InputLayout* input); 
	protected:
	
	private:
		D3D11PipelineState(D3D11Device& device);
		virtual void _ClearAllState();

		virtual void _SetBlendState();
		virtual void _SetRasterizerState();
		virtual void _SetDepthStencilState();

		// DXGI_SAMPLE_DESC is setted per Texture2D in D3D11,
		// so this method has no effect
		virtual void _SetSample();

		CComPtr<ID3D11VertexShader> m_pVS = nullptr;
		CComPtr<ID3D11GeometryShader> m_pGS = nullptr;
		CComPtr<ID3D11HullShader> m_pHS = nullptr;
		CComPtr<ID3D11DomainShader> m_pDS = nullptr;
		CComPtr<ID3D11PixelShader> m_pPS = nullptr;
		
		D3D11BufferArray m_VSConstantBuffers;
		D3D11BufferArray m_GSConstantBuffers;
		D3D11BufferArray m_HSConstantBuffers;
		D3D11BufferArray m_DSConstantBuffers;
		D3D11BufferArray m_PSConstantBuffers;
		D3D11SRVArray m_VSSRVs;
		D3D11SRVArray m_GSSRVs;
		D3D11SRVArray m_HSSRVs;
		D3D11SRVArray m_DSSRVs;
		D3D11SRVArray m_PSSRVs;

		D3D11Device& device;

		CComPtr<ID3D11BlendState> m_pBlendState = nullptr;
		
		CComPtr<ID3D11InputLayout> m_pInputLayout = nullptr;
		CComPtr<ID3D11RasterizerState> m_pRasterizerState = nullptr;
		CComPtr<ID3D11DepthStencilState> m_pDepthStencilState = nullptr;
		
		friend class D3D11CommandList;
	};
}

#endif