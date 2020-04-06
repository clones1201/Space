#include "D3D11Shared.hpp"
#include "D3D11Device.hpp"
#include "D3D11DeviceResource.hpp"
#include "D3D11ResourceView.hpp"
#include "D3D11Shader.hpp"
#include "D3D11PipelineState.h"
#include <InputLayout.hpp>
#include "PipelineStateManager.hpp"
#include "Log.h"


namespace Space
{
namespace Render {

	class D3D11BlendStateCache
	{
	public:
		CComPtr<ID3D11BlendState> CreateBlendState(BlendStateDescriptor blendDesc)
		{
			auto found = m_object_map.find(blendDesc);
			if(found != m_object_map.end())
			{
				return found->second;
			}

			D3D11_BLEND_DESC desc =
			{
				blendDesc.AlphaToCoverageEnable,
				blendDesc.IndependentBlendEnable,
			};
			//unroll
			for(int i = 0; i < 8 ; i++)
			{
				desc.RenderTarget[i] = {
					(BOOL)blendDesc.RenderTarget[i].BlendEnable,
					(D3D11_BLEND)blendDesc.RenderTarget[i].SrcBlend,
					(D3D11_BLEND)blendDesc.RenderTarget[i].DestBlend,
					(D3D11_BLEND_OP)blendDesc.RenderTarget[i].BlendOp,
					(D3D11_BLEND)blendDesc.RenderTarget[i].SrcBlendAlpha,
					(D3D11_BLEND)blendDesc.RenderTarget[i].DestBlendAlpha,
					(D3D11_BLEND_OP)blendDesc.RenderTarget[i].BlendOpAlpha,
					blendDesc.RenderTarget[i].RenderTargetWriteMask
				};
			}
			CComPtr<ID3D11BlendState> pBlendState = nullptr;
			HRESULT hr = Device::GetInstance()->Get()->CreateBlendState(&desc, &(pBlendState));
			if (FAILED(hr))
			{
//				throw std::exception("CreateBlendState failed.");
				return nullptr;
			}
			m_object_map.insert_or_assign(blendDesc, pBlendState);
			return pBlendState;
		}

		void Clear()
		{
			m_object_map.clear();
		}

		static D3D11BlendStateCache& Instance()
		{
			return _instance;
		}
	private:
		static D3D11BlendStateCache _instance;
		std::unordered_map<BlendStateDescriptor, CComPtr<ID3D11BlendState>> m_object_map;
	};
	D3D11BlendStateCache D3D11BlendStateCache::_instance;

	class D3D11RasterizerStateCache
	{
	public:
		CComPtr<ID3D11RasterizerState> CreateRasterizerState(RasterizerStateDescriptor rasterDesc)
		{
			auto found = m_object_map.find(rasterDesc);
			if (found != m_object_map.end())
			{
				return found->second;
			}
			D3D11_RASTERIZER_DESC desc = {
				(D3D11_FILL_MODE)rasterDesc.FillMode,
				(D3D11_CULL_MODE)rasterDesc.CullMode,
				rasterDesc.FrontCounterClockwise,
				rasterDesc.DepthBias,
				rasterDesc.DepthBiasClamp,
				rasterDesc.SlopeScaledDepthBias,
				rasterDesc.DepthClipEnable,
				rasterDesc.ScissorEnable,
				rasterDesc.MultisampleEnable,
				rasterDesc.AntialiasedLineEnable
			};

			CComPtr<ID3D11RasterizerState> ret = nullptr;
			HRESULT hr = Device::GetInstance()->Get()->CreateRasterizerState(&desc, &(ret));
			if (FAILED(hr))
			{
				return nullptr;
			}
			m_object_map.insert_or_assign(rasterDesc, ret);
			return ret;
		}

		void Clear()
		{
			m_object_map.clear();
		}

		static D3D11RasterizerStateCache& Instance()
		{
			return _instance;
		}
	private:
		static D3D11RasterizerStateCache _instance;
		std::unordered_map<RasterizerStateDescriptor, CComPtr<ID3D11RasterizerState>> m_object_map;
	};
	D3D11RasterizerStateCache D3D11RasterizerStateCache::_instance;

	class D3D11DepthStencilStateCache
	{
	public:
		CComPtr<ID3D11DepthStencilState> CreateDepthStencilState(DepthStencilStateDescriptor depthStateDesc)
		{
			auto found = m_object_map.find(depthStateDesc);
			if (found != m_object_map.end())
			{
				return found->second;
			}

			D3D11_DEPTH_STENCIL_DESC desc = {
				depthStateDesc.DepthEnable,
				(D3D11_DEPTH_WRITE_MASK)depthStateDesc.DepthWriteMask,
				(D3D11_COMPARISON_FUNC)depthStateDesc.DepthFunc,
				depthStateDesc.StencilEnable,
				depthStateDesc.StencilReadMask,
				depthStateDesc.StencilWriteMask 
			};

			desc.FrontFace = {
				(D3D11_STENCIL_OP)depthStateDesc.FrontFace.StencilFailOp,
				(D3D11_STENCIL_OP)depthStateDesc.FrontFace.StencilDepthFailOp,
				(D3D11_STENCIL_OP)depthStateDesc.FrontFace.StencilPassOp,
				(D3D11_COMPARISON_FUNC)depthStateDesc.FrontFace.StencilFunc
			};
			desc.BackFace = {
				(D3D11_STENCIL_OP)depthStateDesc.BackFace.StencilFailOp,
				(D3D11_STENCIL_OP)depthStateDesc.BackFace.StencilDepthFailOp,
				(D3D11_STENCIL_OP)depthStateDesc.BackFace.StencilPassOp,
				(D3D11_COMPARISON_FUNC)depthStateDesc.BackFace.StencilFunc
			};
			CComPtr<ID3D11DepthStencilState> ret = nullptr;
			HRESULT hr = Device::GetInstance()->Get()->CreateDepthStencilState(&desc, &(ret));
			if (FAILED(hr))
			{
				return nullptr;
			}
			m_object_map.insert_or_assign(depthStateDesc, ret);
			return ret;
		}

		void Clear()
		{
			m_object_map.clear();
		}

		static D3D11DepthStencilStateCache& Instance()
		{
			return _instance;
		}
	private:
		static D3D11DepthStencilStateCache _instance;
		std::unordered_map<DepthStencilStateDescriptor, CComPtr<ID3D11DepthStencilState>> m_object_map;
	};
	D3D11DepthStencilStateCache D3D11DepthStencilStateCache::_instance;

	D3D11PipelineState::D3D11PipelineState(D3D11Device* device)
		:m_Device(device)
	{
	}
	
	void D3D11PipelineState::_ClearAllState()
	{
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
			m_ElemArray.data(), (UINT)m_ElemArray.size(),
			pCode->GetBufferPointer(), pCode->GetBufferSize(),
			&(m_pInputLayout));
		if (FAILED(hr))
		{
			Log(TEXT("CreateInputLayout failed"));
			return;
		}
	}

	void D3D11PipelineState::SetBlendState(BlendStateDescriptor BlendState)
	{		
		m_pBlendState = D3D11BlendStateCache::Instance().CreateBlendState(BlendState);
	}
	void D3D11PipelineState::SetRasterizerState(RasterizerStateDescriptor RasterState)
	{
		m_pRasterizerState = D3D11RasterizerStateCache::Instance().CreateRasterizerState(RasterState);
	}		
	void D3D11PipelineState::SetDepthStencilState(DepthStencilStateDescriptor depthStencilState)
	{
		m_pDepthStencilState = D3D11DepthStencilStateCache::Instance().CreateDepthStencilState(depthStencilState);
	}
}
}
