#include "Log.h"
#include "Utility.h"
#include "SharedUtility.hpp"
#include "D3D11Shader.hpp"
#include "D3D11Device.hpp"

namespace Space
{
	class D3D11VertexShaderImpl : public D3D11VertexShader
	{
	private:
		D3D11Device &mDevice;

		CComPtr<ID3D11VertexShader> m_pShader = nullptr;
	public: 
		D3D11VertexShaderImpl(D3D11Device &device, byte const* byteCodes, uint32 sizeInBytes)
			: mDevice(device){

			ID3D11VertexShader* pVS = nullptr;
			HRESULT hr = mDevice->CreateVertexShader(byteCodes,sizeInBytes,nullptr,&pVS);
			if (FAILED(hr))
			{
				throw std::exception("CreateVertexShader failed.");
			}
			m_pShader = pVS; 

		}

		void D3D11VertexShader::SetConstantBuffers(std::vector<TypeTrait<ConstantBuffer>::Ptr>& vBuffers)
		{

		}
		void D3D11VertexShader::SetShaderResources(std::vector<TypeTrait<ShaderResource>::Ptr>& vResources)
		{

		}

		void D3D11VertexShader::Apply()
		{

		}

	};
 
	D3D11VertexShader* D3D11VertexShader::LoadBinaryFromMemory(
		D3D11Device& device, 
		byte const* byteCodes, uint32 sizeInBytes)
	{
		try
		{
			return new D3D11VertexShaderImpl(device, byteCodes, sizeInBytes);
		}
		catch (std::exception &e)
		{
			Log(e.what());
			return nullptr;
		}
	}


	D3D11VertexShader::~D3D11VertexShader()
	{}
	D3D11VertexShader::D3D11VertexShader()
	{}

	class D3D11PixelShaderImpl : public D3D11PixelShader
	{
	private:
		D3D11Device &mDevice;

		CComPtr<ID3D11PixelShader> m_pShader = nullptr;
	public:
		D3D11PixelShaderImpl(D3D11Device &device, byte const* byteCodes, uint32 sizeInBytes)
			: mDevice(device){

			ID3D11PixelShader* pVS = nullptr;
			HRESULT hr = mDevice->CreatePixelShader(byteCodes, sizeInBytes, nullptr, &pVS);
			if (FAILED(hr))
			{
				throw std::exception("CreatePixelShader failed.");
			}
			m_pShader = pVS;

		}

		void D3D11PixelShader::SetConstantBuffers(std::vector<TypeTrait<ConstantBuffer>::Ptr>& vBuffers)
		{

		}
		void D3D11PixelShader::SetShaderResources(std::vector<TypeTrait<ShaderResource>::Ptr>& vResources)
		{

		}

		void D3D11PixelShader::Apply()
		{

		}

	};

	D3D11PixelShader* D3D11PixelShader::LoadBinaryFromMemory(
		D3D11Device& device,
		byte const* byteCodes, uint32 sizeInBytes)
	{
		try
		{
			return new D3D11PixelShaderImpl(device, byteCodes, sizeInBytes);
		}
		catch (std::exception &e)
		{
			Log(e.what());
			return nullptr;
		}
	}


	D3D11PixelShader::~D3D11PixelShader()
	{}
	D3D11PixelShader::D3D11PixelShader()
	{}


}

