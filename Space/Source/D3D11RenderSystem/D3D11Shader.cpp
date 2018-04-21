#include "Common/Log.h"
#include "Common/Utility.hpp"
#include "D3D11RenderSystem/D3D11Shader.hpp"
#include "D3D11RenderSystem/D3D11Device.hpp"

#include "D3D11RenderSystem/D3D11ShaderReflection.hpp"

#include "RenderSystem/Shader.hpp"

namespace Space
{
	namespace Render {
		D3D11VertexShader::D3D11VertexShader(
			D3D11Device* device, std::vector<byte> byteCodes)
			: m_Device(device)
		{
			ID3D11VertexShader* pVS = nullptr;
			HRESULT hr = m_Device->Get()->CreateVertexShader(
				byteCodes.data(), byteCodes.size(), nullptr, &pVS);
			if (FAILED(hr))
			{
				throw std::exception("CreateVertexShader failed.");
			}
			m_pShader = pVS;
		}

		D3D11VertexShader::~D3D11VertexShader()
		{
		}

		CComPtr<ID3D11VertexShader> D3D11VertexShader::GetShader() const
		{
			return m_pShader;
		}

		D3D11PixelShader::D3D11PixelShader(
			D3D11Device* device, std::vector<byte> byteCodes)
			:m_Device(device)
		{
			ID3D11PixelShader* pPS = nullptr;
			HRESULT hr = m_Device->Get()->CreatePixelShader(
				byteCodes.data(), byteCodes.size(), nullptr, &pPS);
			if (FAILED(hr))
			{
				throw std::exception("CreatePixelShader failed.");
			}
			m_pShader = pPS;
		}

		D3D11PixelShader::~D3D11PixelShader()
		{
		}

		CComPtr<ID3D11PixelShader> D3D11PixelShader::GetShader() const
		{
			return m_pShader;
		}

		std::vector<byte> CompileShader(
			char const* sourceCode,
			size_t sourceCodeLength,
			char const* fileName,
			char const* entry,
			char const* profile,
			ShaderMacro const* macros
		);

		std::vector<byte> D3D11VertexShader::Compile(ShaderSource const& source)
		{
			return CompileShader(
				source.StringSource->c_str(), source.StringSource->size(),
				source.FileName.c_str(),
				source.Entry.c_str(), 
				("vs_" + source.Profile).c_str(),
				source.Macros.get()
			);
		}

		std::vector<byte> D3D11PixelShader::Compile(ShaderSource const& source)
		{
			return CompileShader(
				source.StringSource->c_str(), source.StringSource->size(),
				source.FileName.c_str(),
				source.Entry.c_str(),
				("ps_" + source.Profile).c_str(), 
				source.Macros.get()
			);
		}
		
		std::vector<byte> CompileShader(
			char const* sourceCode,
			size_t sourceCodeLength,
			char const* fileName,
			char const* entry,
			char const* profile,
			ShaderMacro const* macros
			)
		{
			CComPtr<ID3DBlob> pBlob = nullptr;
			CComPtr<ID3DBlob> pErrorBlob = nullptr;
			
			std::vector<D3D_SHADER_MACRO> d3dMacros;
			D3D_SHADER_MACRO *pMacros = nullptr;
			if (!macros->_macros.empty())
			{
				for (auto &key_value : macros->_macros)
					d3dMacros.push_back({ key_value.first.c_str(), key_value.first.c_str() });
				d3dMacros.push_back({ NULL, NULL });
				pMacros = d3dMacros.data();
			}
			HRESULT hr = D3DCompile(
				sourceCode, sourceCodeLength, fileName,
				pMacros, D3D_COMPILE_STANDARD_FILE_INCLUDE, entry, profile,
#if defined DEBUG || defined _DEBUG
				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_PREFER_FLOW_CONTROL | D3DCOMPILE_WARNINGS_ARE_ERRORS,
#else
				D3DCOMPILE_OPTIMIZATION_LEVEL3,
#endif
				0,
				&pBlob,
				&pErrorBlob
			);
			if (FAILED(hr))
			{
				pBlob = nullptr;
				std::cout << (char*)pErrorBlob->GetBufferPointer() << std::endl;
				return std::vector<byte>();
			}

			byte const* start = (byte const*)pBlob->GetBufferPointer();
			byte const* end = start + pBlob->GetBufferSize();
			return std::vector<byte>(start, end);
		}

	}

}
