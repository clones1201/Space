#include "D3D11RenderSystem/D3D11Shared.hpp"
#include "D3D11RenderSystem/D3D11DeviceResource.hpp"
#include "D3D11RenderSystem/D3D11ResourceView.hpp"
#include "D3D11RenderSystem/D3D11Shader.hpp"
#include "D3D11RenderSystem/D3D11Rendering.hpp"

namespace Space
{
	D3D11PipelineState* D3D11PipelineState::Create(D3D11DevicePtr device)
	{
		TRY_CATCH_LOG(
			return new D3D11PipelineState(device),
			return nullptr;
		);
	}

	D3D11PipelineState::D3D11PipelineState(D3D11DevicePtr device)
		:device(device){


	}

	void GetAllD3D11ConstantBufferAndShaderResource(
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
	}

	void D3D11PipelineState::SetShaders(Shader* shader)
	{
		if (shader == nullptr)
			return;

		assert(nullptr != dynamic_cast<D3D11VertexShader*>(shader->GetVertexShader()));
		assert(nullptr != dynamic_cast<D3D11PixelShader*>(shader->GetPixelShader()));

		m_InputSignature = shader->GetVertexShader()->GetByteCodes();
		m_LengthOfSignature = shader->GetVertexShader()->GetSizeInBytes();

		m_pVS = static_cast<D3D11VertexShader*>(shader->GetVertexShader())
			->GetShader();
		m_pPS = static_cast<D3D11PixelShader*>(shader->GetPixelShader())
			->GetShader();

		m_VSConstantBuffers.resize(shader->GetVertexShader()->GetConstantBufferCount());
		m_PSConstantBuffers.resize(shader->GetPixelShader()->GetShaderResourceCount());

		GetAllD3D11ConstantBufferAndShaderResource(
			m_VSConstantBuffers, m_VSSRVs, shader->GetVertexShader());
		GetAllD3D11ConstantBufferAndShaderResource(
			m_PSConstantBuffers, m_PSSRVs, shader->GetPixelShader());

	}

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

	void D3D11PipelineState::SetInputLayout(InputLayout* input,
		byte const* pInputSignature, uint32 signatureSizeInBytes)
	{
		if (input == nullptr || pInputSignature == nullptr || signatureSizeInBytes == 0) return;

		m_ElemArray.clear();
		m_ElemArray.reserve(input->GetSize());
		for (auto iter = input->Begin();
			iter != input->End(); ++iter)
		{
			auto desc = D3D11_INPUT_ELEMENT_DESC{
				GetSemanticName(iter->Semantic),
				iter->SemanticIdx,
				GetElemDXGIFormat(iter->Type),
				iter->InputSlot,
				iter->AlignedByteOffset,
				iter->ElemClass ==
				ElementClass::PerVertex ?
			D3D11_INPUT_PER_VERTEX_DATA : D3D11_INPUT_PER_INSTANCE_DATA,
										  iter->InstanceStep
			};
			m_ElemArray.push_back(desc);
		}
		m_pInput = input;
		
		_CreateInputLayout(pInputSignature,signatureSizeInBytes);
	}

	void D3D11PipelineState::_CreateInputLayout(byte const* pInputSignature, uint32 signatureSizeInBytes)
	{
		assert(pInputSignature != nullptr && signatureSizeInBytes != 0);

		m_pInputLayout = nullptr;
		HRESULT hr = device->Get()->CreateInputLayout(
			m_ElemArray.data(), m_ElemArray.size(),
			pInputSignature, signatureSizeInBytes,
			&(m_pInputLayout));
		if (FAILED(hr))
		{
			Log(TEXT("CreateInputLayout failed"));
			return;
		}
	}
	void D3D11PipelineState::_SetBlendState()
	{
		D3D11_BLEND_DESC desc;
		desc.AlphaToCoverageEnable = m_BlendStateDesc.AlphaToCoverageEnable;
		desc.IndependentBlendEnable = m_BlendStateDesc.IndependentBlendEnable;
		for (uint i = 0; i < 8; i ++)
		{
			desc.RenderTarget[i] = D3D11_RENDER_TARGET_BLEND_DESC{
				(BOOL)m_BlendStateDesc.RenderTarget[i].BlendEnable,
				(D3D11_BLEND)m_BlendStateDesc.RenderTarget[i].SrcBlend,
				(D3D11_BLEND)m_BlendStateDesc.RenderTarget[i].DestBlend,
				(D3D11_BLEND_OP)m_BlendStateDesc.RenderTarget[i].BlendOp,
				(D3D11_BLEND)m_BlendStateDesc.RenderTarget[i].SrcBlendAlpha,
				(D3D11_BLEND)m_BlendStateDesc.RenderTarget[i].DestBlendAlpha,
				(D3D11_BLEND_OP)m_BlendStateDesc.RenderTarget[i].BlendOpAlpha,
				(UINT8)m_BlendStateDesc.RenderTarget[i].RenderTargetWriteMask,
			};
		}
		m_pBlendState = nullptr;
		HRESULT hr = device->Get()->CreateBlendState(&desc, &(m_pBlendState));
		if (FAILED(hr))
		{
			throw std::exception("CreateBlendState failed.");
		}
	}
	void D3D11PipelineState::_SetRasterizerState()
	{
		auto desc = D3D11_RASTERIZER_DESC{
			(D3D11_FILL_MODE)m_RasterizerDesc.FillMode,
			(D3D11_CULL_MODE)m_RasterizerDesc.CullMode,
			(BOOL)m_RasterizerDesc.FrontCounterClockwise,
			(INT)m_RasterizerDesc.DepthBias,
			(FLOAT)m_RasterizerDesc.DepthBiasClamp,
			(FLOAT)m_RasterizerDesc.SlopeScaledDepthBias,
			(BOOL)m_RasterizerDesc.DepthClipEnable,
			(BOOL)m_RasterizerDesc.ScissorEnable,
			(BOOL)m_RasterizerDesc.MultisampleEnable,
			(BOOL)m_RasterizerDesc.AntialiasedLineEnable,
		};
		m_pRasterizerState = nullptr;
		HRESULT hr = device->Get()->CreateRasterizerState(&desc, &(m_pRasterizerState));
		if (FAILED(hr))
		{
			throw std::exception("CreateRasterizerState faild.");
		}
	}
	void D3D11PipelineState::_SetDepthStencilState()
	{
		auto desc = D3D11_DEPTH_STENCIL_DESC{
			(BOOL)m_DepthStencilDesc.DepthEnable,
			(D3D11_DEPTH_WRITE_MASK)m_DepthStencilDesc.DepthWriteMask,
			(D3D11_COMPARISON_FUNC)m_DepthStencilDesc.DepthFunc,
			(BOOL)m_DepthStencilDesc.StencilEnable,
			(UINT8)m_DepthStencilDesc.StencilReadMask,
			(UINT8)m_DepthStencilDesc.StencilWriteMask,
			D3D11_DEPTH_STENCILOP_DESC{
				(D3D11_STENCIL_OP)m_DepthStencilDesc.FrontFace.StencilFailOp,
				(D3D11_STENCIL_OP)m_DepthStencilDesc.FrontFace.StencilDepthFailOp,
				(D3D11_STENCIL_OP)m_DepthStencilDesc.FrontFace.StencilPassOp,
				(D3D11_COMPARISON_FUNC)m_DepthStencilDesc.FrontFace.StencilFunc
			},
			D3D11_DEPTH_STENCILOP_DESC{
					(D3D11_STENCIL_OP)m_DepthStencilDesc.FrontFace.StencilFailOp,
					(D3D11_STENCIL_OP)m_DepthStencilDesc.FrontFace.StencilDepthFailOp,
					(D3D11_STENCIL_OP)m_DepthStencilDesc.FrontFace.StencilPassOp,
					(D3D11_COMPARISON_FUNC)m_DepthStencilDesc.FrontFace.StencilFunc
				}
		};

		m_pDepthStencilState = nullptr;
		HRESULT hr = device->Get()->CreateDepthStencilState(&desc, &(m_pDepthStencilState));
		if (FAILED(hr))
		{
			throw std::exception("CreateDepthStencilState failed.");
		}
	}
	void D3D11PipelineState::_SetSample()
	{
	}
}
