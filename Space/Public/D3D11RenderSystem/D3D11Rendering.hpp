#pragma once

#include "D3D11RenderSystem/D3D11Prerequisites.hpp"
#include "D3D11RenderSystem/D3D11Device.hpp"
#include "D3D11RenderSystem/D3D11DeviceResource.hpp"
#include "D3D11RenderTarget.hpp"
#include "D3D11RenderSystem/D3D11ResourceView.hpp"
#include "D3D11RenderSystem/D3D11Shader.hpp"

namespace Space
{
	namespace Render {
		class InputLayout;
		/*
		
		*/
		class SPACE_D3D11_API D3D11CommandList : 
			public Uncopyable
		{
		public:
			CComPtr<ID3D11CommandList> GetList() const;
			CComPtr<ID3D11DeviceContext> GetContext() const;

			void ClearDepth(D3D11DepthStencilView* pDepth, float depth);
			void ClearStencil(D3D11DepthStencilView* pStencil, uint8 stencil);
			void ClearRenderTargetView(D3D11RenderTarget* pTarget, Float4 clearColor);

			void Reset();
			void Close();

			void SetIndexBuffer(
				D3D11DeviceBuffer* buffer, DataFormat format, int32 offset);
			void SetVertexBuffers(
				uint startSlot, std::vector<D3D11DeviceBuffer*> const& buffers, std::vector<int32> const& strides, std::vector<int32> const& offsets);
			void SetPipelineState(D3D11PipelineState const* state);
			void SetRenderTargets(std::vector<D3D11RenderTarget*> const& targets, D3D11DepthStencilView* pDepth);
			void SetViewPorts(ViewPort const* pViewPorts, uint numViewPorts);
			void SetScissorRects(Rect const* rects, uint32 numRects);
			/*

			void SetPrimitiveTopology(PrimitiveTopology topology);
			void SetVSShader(D3D11VertexShader::SharedPtr vsShader);
			void SetPSShader(D3D11VertexShader::SharedPtr psShader);*/
			//void SetVSShader();
			void SetVertexShader(D3D11VertexShader const* shader);
			void SetPixelShader(D3D11PixelShader const* shader);

			virtual void DrawIndexed(uint startIndex, uint numPrimitive);
			virtual void DrawIndexedInstanced(
				uint startIndex, uint numPrimitive, uint startInstance, uint numInstance);

			D3D11CommandList(D3D11Device* device);
		private:
			std::vector<D3D11_VIEWPORT> m_Viewports;
			std::vector<D3D11_RECT> m_ScissorRects;

			CComPtr<ID3D11DeviceContext> m_pDeferredContext = nullptr;
			CComPtr<ID3D11CommandList> m_pCommandList = nullptr;

			D3D11Device* device;
		};
		typedef std::shared_ptr<D3D11CommandList> D3D11CommandListPtr;

		typedef std::vector<CComPtr<ID3D11Buffer>> D3D11BufferArray;
		typedef std::vector<CComPtr<ID3D11ShaderResourceView>> D3D11SRVArray;

		class SPACE_D3D11_API D3D11PipelineState: public Uncopyable
		{
		public:
			//void Apply(D3D11CommandList::SharedPtr pList);

			D3D11PipelineState(D3D11Device* device);
			virtual void _ClearAllState();

			virtual void _SetBlendState();
			virtual void _SetRasterizerState();
			virtual void _SetDepthStencilState();

			// DXGI_SAMPLE_DESC is setted per Texture2D in D3D11,
			// so this method has no effect
			/*virtual void _SetSample();
			*/

			void SetInputLayout(InputElement const* input, size_t elementCount);

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

			D3D11Device* m_Device;

			CComPtr<ID3D11BlendState> m_pBlendState = nullptr;

			std::vector<D3D11_INPUT_ELEMENT_DESC> m_ElemArray;
			CComPtr<ID3D11InputLayout> m_pInputLayout = nullptr;
			CComPtr<ID3D11RasterizerState> m_pRasterizerState = nullptr;
			CComPtr<ID3D11DepthStencilState> m_pDepthStencilState = nullptr;

			static std::vector<CComPtr<ID3D11InputLayout>> m_InputLayoutArray;
		};
		typedef std::shared_ptr<D3D11PipelineState> D3D11PipelineStatePtr;
	}
}