#ifndef __SPACE_D3D11_RENDERING_HPP__
#define __SPACE_D3D11_RENDERING_HPP__

#include "RenderSystem/D3D11Prerequisites.hpp"
//#include "Material.hpp"
#include "RenderSystem/Rendering.hpp"
#include "RenderSystem/D3D11Device.hpp"
#include "RenderSystem/D3D11RenderSystem.hpp"

namespace Space
{

	class SPACE_D3D11_API D3D11CommandList : public CommandList
	{
	public:
		static D3D11CommandList* Create(D3D11DevicePtr device);
		
		CComPtr<ID3D11CommandList> GetList() const;
		CComPtr<ID3D11DeviceContext> GetContext() const;

		virtual void ClearDepth(DepthStencilView* pDepth, float depth);
		virtual void ClearStencil(DepthStencilView* pStencil, uint8 stencil);
		virtual void ClearRenderTargetView(RenderTarget* pTarget, Float4 clearColor);

		virtual void Reset();
		virtual void Close(); 
		
		virtual void SetIndexBuffer(IndexBuffer const* buffer);
		virtual void SetVertexBuffers(
			uint startSlot, uint numBuffers, VertexBuffer *const* buffer);
		virtual void SetPipelineState(PipelineState const* state);
		virtual void SetRenderTargets(RenderTarget *const* targets, uint32 numTargets, DepthStencilView const* pDepth);
		virtual void SetViewPorts(ViewPort const* pViewPorts, uint numViewPorts);
		virtual void SetScissorRects(Rect const* rects, uint32 numRects);

		virtual void DrawIndexed(uint startIndex, uint numPrimitive);
		virtual void DrawIndexedInstanced(
			uint startIndex, uint numPrimitive, uint startInstance, uint numInstance);

	protected:		

	private:
		D3D11CommandList(D3D11DevicePtr device);

		std::vector<D3D11_VIEWPORT> m_Viewports;
		std::vector<D3D11_RECT> m_ScissorRects;
		
		CComPtr<ID3D11DeviceContext> m_pDeferredContext = nullptr;
		CComPtr<ID3D11CommandList> m_pCommandList = nullptr;

		D3D11DevicePtr device;
	};
	typedef std::shared_ptr<D3D11CommandList> D3D11CommandListPtr;

	typedef std::vector<CComPtr<ID3D11Buffer>> D3D11BufferArray;
	typedef std::vector<CComPtr<ID3D11ShaderResourceView>> D3D11SRVArray;
	
	class SPACE_D3D11_API D3D11PipelineState : public PipelineState
	{
	public:
		static D3D11PipelineState* Create(D3D11DevicePtr device);

		D3D11PipelineState(D3D11PipelineState const& other) = default;
		D3D11PipelineState& operator=(D3D11PipelineState const& other) = default;

		virtual void SetShaders(Shader* shader);
		virtual void SetInputLayout(InputLayout* input,
			byte const* pInputSignature, uint32 signatureSizeInBytes);
	protected:
	
	private:
		D3D11PipelineState(D3D11DevicePtr device);
		virtual void _ClearAllState();

		virtual void _SetBlendState();
		virtual void _SetRasterizerState();
		virtual void _SetDepthStencilState();

		// DXGI_SAMPLE_DESC is setted per Texture2D in D3D11,
		// so this method has no effect
		virtual void _SetSample();

		void _CreateInputLayout(byte const* pInputSignature, uint32 signatureSizeInBytes);

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

		D3D11DevicePtr device;

		CComPtr<ID3D11BlendState> m_pBlendState = nullptr;
		
		std::vector<D3D11_INPUT_ELEMENT_DESC> m_ElemArray;
		CComPtr<ID3D11InputLayout> m_pInputLayout = nullptr;
		CComPtr<ID3D11RasterizerState> m_pRasterizerState = nullptr;
		CComPtr<ID3D11DepthStencilState> m_pDepthStencilState = nullptr;
		
		static std::vector<CComPtr<ID3D11InputLayout>> m_InputLayoutArray;
		
		friend class D3D11CommandList;
	};
	typedef std::shared_ptr<D3D11PipelineState> D3D11PipelineStatePtr;
}

#endif