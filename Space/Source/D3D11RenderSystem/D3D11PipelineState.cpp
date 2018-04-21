#include "D3D11RenderSystem/D3D11Shared.hpp"
#include "D3D11RenderSystem/D3D11Device.hpp"
#include "D3D11RenderSystem/D3D11DeviceResource.hpp"
#include "D3D11RenderSystem/D3D11ResourceView.hpp"
#include "D3D11RenderSystem/D3D11Shader.hpp"
#include "D3D11RenderSystem/D3D11Rendering.hpp"
#include <RenderSystem/InputLayout.hpp>

namespace Space
{
	namespace Render {
		D3D11PipelineState::D3D11PipelineState(D3D11Device* device)
			:m_Device(device)
		{
		}

		/*void GetAllD3D11ConstantBufferAndShaderResource(
			D3D11BufferArray& buffers, D3D11SRVArray& SRVs, ShaderBase* shader
			)
		{
			for (uint i = 0; i < buffers.size(); ++i)
			{
				auto pCB = shader->GetConstantBuffer(i);
				if (pCB == nullptr)
				{
					buffers.at(i) = nullptr;
				}
				else
				{
					auto pDBuffer = pCB->GetBuffer();
					assert(nullptr != dynamic_cast<D3D11DeviceBuffer*>(pDBuffer));
					buffers.at(i) =
						static_cast<D3D11DeviceBuffer*>(pDBuffer)->GetRawBuffer();
				}
			}

			for (uint i = 0; i < SRVs.size(); ++i)
			{
				auto pSR = shader->GetShaderResource(i);
				if (pSR == nullptr)
				{
					SRVs.at(i) = nullptr;
				}
				else
				{
					assert(nullptr != dynamic_cast<D3D11ShaderResource*>(pSR));
					SRVs.at(i) =
						static_cast<D3D11ShaderResource*>(pSR)->GetShaderResourceView();
				}
			}
		}*/

		//void D3D11PipelineState::SetShaders(Shader* shader)
		//{
		//	if (shader == nullptr)
		//		return;
		//	
		//	m_InputSignature = shader->GetVertexShader()->GetByteCodes();
		//	m_LengthOfSignature = shader->GetVertexShader()->GetSizeInBytes();

		//	m_pVS = static_cast<D3D11VertexShader*>(shader->GetVertexShader())
		//		->GetShader();
		//	m_pPS = static_cast<D3D11PixelShader*>(shader->GetPixelShader())
		//		->GetShader();

		//	m_VSConstantBuffers.resize(shader->GetVertexShader()->GetConstantBufferCount());
		//	m_PSConstantBuffers.resize(shader->GetPixelShader()->GetShaderResourceCount());

		//	GetAllD3D11ConstantBufferAndShaderResource(
		//		m_VSConstantBuffers, m_VSSRVs, shader->GetVertexShader());
		//	GetAllD3D11ConstantBufferAndShaderResource(
		//		m_PSConstantBuffers, m_PSSRVs, shader->GetPixelShader());

		//}

		void D3D11PipelineState::_ClearAllState()
		{
			m_pVS = nullptr; m_VSConstantBuffers.clear(); m_VSSRVs.clear();
			m_pGS = nullptr; m_GSConstantBuffers.clear(); m_GSSRVs.clear();
			m_pHS = nullptr; m_HSConstantBuffers.clear(); m_HSSRVs.clear();
			m_pDS = nullptr; m_DSConstantBuffers.clear(); m_DSSRVs.clear();
			m_pPS = nullptr; m_PSConstantBuffers.clear(); m_PSSRVs.clear();

			m_pBlendState = nullptr;
			m_pInputLayout = nullptr;
			m_pDepthStencilState = nullptr;
			m_pRasterizerState = nullptr;
		}

		static const char* g_InputLayoutSignatureShaderTemplate = "\n\
		//Input Assemply Declaration\n\
		struct InputAttributes{\n\
			%s\n\
		};\n\
		float4 main(in InputAttributes input):SV_POSITION { return float4(.5,.5,.5,1.0); }";

		
		void D3D11PipelineState::SetInputLayout(InputElement const* input, size_t elementCount)
		{
			if (input == nullptr) return;

			std::string inputElementsStr = "";
			m_ElemArray.clear();
			m_ElemArray.reserve(elementCount);
						
			for (size_t idx = 0; idx < elementCount ; idx++)
			{
				InputElement const* iter = input + idx;
				char const* semantic = GetSemanticName(iter->Semantic);
				auto desc = D3D11_INPUT_ELEMENT_DESC{
					semantic,
					iter->SemanticIdx,
					GetElemDXGIFormat(iter->Type),
					iter->InputSlot,
					iter->AlignedByteOffset,
					iter->ElemClass ==
					ElementClass::PerVertex ?
						D3D11_INPUT_PER_VERTEX_DATA : D3D11_INPUT_PER_INSTANCE_DATA,
											  iter->InstanceStep
				};
				char declaration[256];
				sprintf_s(declaration, "    float4 variable_%d:%s%d;\n", (int32)idx, semantic, iter->SemanticIdx);
				inputElementsStr += declaration;
				m_ElemArray.push_back(desc);
			}

			char signatureCode[1024];
			int signatureCodeLength = sprintf_s(signatureCode, g_InputLayoutSignatureShaderTemplate, inputElementsStr.c_str());
			
			CComPtr<ID3DBlob> pCode;
			CComPtr<ID3DBlob> pError;
			HRESULT hr = D3DCompile(signatureCode, signatureCodeLength, "InputTemplate", NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0",
#if defined (_DEBUG)
				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION
#else
				D3DCOMPILE_OPTIMIZATION_LEVEL3
#endif
				,0, &pCode, &pError);
			if (FAILED(hr))
			{
				Log(TEXT("CreateInputLayout failed"));
				return;
			}

			m_pInputLayout = nullptr;
			hr = m_Device->Get()->CreateInputLayout(
				m_ElemArray.data(), m_ElemArray.size(),
				pCode->GetBufferPointer(), pCode->GetBufferSize(),
				&(m_pInputLayout));
			if (FAILED(hr))
			{
				Log(TEXT("CreateInputLayout failed"));
				return;
			}
		}

		void D3D11PipelineState::_SetBlendState()
		{
			CD3D11_BLEND_DESC desc = CD3D11_BLEND_DESC(CD3D11_DEFAULT());
			m_pBlendState = nullptr;
			HRESULT hr = m_Device->Get()->CreateBlendState(&desc, &(m_pBlendState));
			if (FAILED(hr))
			{
				throw std::exception("CreateBlendState failed.");
			}
		}
		void D3D11PipelineState::_SetRasterizerState()
		{
			D3D11_RASTERIZER_DESC rasterizerState;
			ZeroMemory(&rasterizerState, sizeof(D3D11_RASTERIZER_DESC));

			rasterizerState.AntialiasedLineEnable = false;
			rasterizerState.CullMode = D3D11_CULL_NONE;
			rasterizerState.FillMode = D3D11_FILL_SOLID;
			rasterizerState.DepthBias = 0;
			rasterizerState.DepthBiasClamp = 0.0f;
			rasterizerState.DepthClipEnable = true;
			rasterizerState.FrontCounterClockwise = false;
			rasterizerState.MultisampleEnable = false;
			rasterizerState.ScissorEnable = false;
			rasterizerState.SlopeScaledDepthBias = 0.0f;

			m_pRasterizerState = nullptr;
			HRESULT hr = m_Device->Get()->CreateRasterizerState(&rasterizerState, &(m_pRasterizerState));
			if (FAILED(hr))
			{
				throw std::exception("CreateRasterizerState faild.");
			}
		}
		
		void D3D11PipelineState::_SetDepthStencilState()
		{
			auto desc = CD3D11_DEPTH_STENCIL_DESC(CD3D11_DEFAULT());
			m_pDepthStencilState = nullptr;
			HRESULT hr = m_Device->Get()->CreateDepthStencilState(&desc, &(m_pDepthStencilState));
			if (FAILED(hr))
			{
				throw std::exception("CreateDepthStencilState failed.");
			}
		}
//		void D3D11PipelineState::_SetSample()
//		{
//		}
		
	}
}
