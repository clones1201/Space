#include "Common/Log.h"
#include "Common/Utility.hpp"
#include "RenderSystem/D3D11Shader.hpp"
#include "RenderSystem/D3D11Device.hpp"

#include "RenderSystem/D3D11ShaderReflection.hpp"

namespace Space
{
	D3D11VertexShader::D3D11VertexShader(
		D3D11DevicePtr device, byte const* byteCodes, uint32 sizeInBytes)
		: mDevice(device)
	{
		m_ByteCodes = std::vector<byte>(byteCodes, byteCodes + sizeInBytes);

		ID3D11VertexShader* pVS = nullptr;
		HRESULT hr = mDevice->Get()->CreateVertexShader(
			m_ByteCodes.data(), m_ByteCodes.size(), nullptr, &pVS);
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

	D3D11VertexShader* D3D11VertexShader::LoadBinaryFromMemory(
		D3D11DevicePtr device,
		byte const* byteCodes, size_t sizeInBytes)
	{
		TRY_CATCH_LOG(
			return new D3D11VertexShader(device, byteCodes, sizeInBytes),
			return nullptr
			);
	}

	D3D11PixelShader::D3D11PixelShader(
		D3D11DevicePtr device, byte const* byteCodes, size_t sizeInBytes)
		: mDevice(device){

		m_ByteCodes = std::vector<byte>(byteCodes, byteCodes + sizeInBytes);

		ID3D11PixelShader* pPS = nullptr;
		HRESULT hr = mDevice->Get()->CreatePixelShader(
			m_ByteCodes.data(), m_ByteCodes.size(), nullptr, &pPS);
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

	D3D11PixelShader* D3D11PixelShader::LoadBinaryFromMemory(
		D3D11DevicePtr device,
		byte const* byteCodes, uint32 sizeInBytes)
	{
		TRY_CATCH_LOG(
			return new D3D11PixelShader(device, byteCodes, sizeInBytes),
			return nullptr
		);
	}
	 
	ShaderReflection * D3D11ShaderBase::CreateReflection()
	{
		return new D3D11ShaderReflection(this);
	}

}

