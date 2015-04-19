#include "Log.h"
#include "Utility.hpp"
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

			m_SizeInBytes = sizeInBytes;
			m_ByteCodes = new byte[sizeInBytes];

			memcpy_s((void*)m_ByteCodes, m_SizeInBytes, byteCodes, sizeInBytes);

			ID3D11VertexShader* pVS = nullptr;
			HRESULT hr = mDevice->CreateVertexShader(m_ByteCodes, m_SizeInBytes, nullptr, &pVS);
			if (FAILED(hr))
			{
				throw std::exception("CreateVertexShader failed.");
			}
			m_pShader = pVS; 

		}

		~D3D11VertexShaderImpl()
		{
			if (m_ByteCodes != nullptr)
				delete[] m_ByteCodes;

		}

		void D3D11VertexShader::SetConstantBuffers(std::vector<TypeTrait<ConstantBuffer>::Ptr>& vBuffers)
		{

		}
		void D3D11VertexShader::SetShaderResources(std::vector<TypeTrait<ShaderResource>::Ptr>& vResources)
		{

		}

		void D3D11VertexShader::Apply()
		{
			if (m_pShader != nullptr)
				mDevice.GetImmediateContext()->VSSetShader(m_pShader, nullptr, 0);
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

			m_SizeInBytes = sizeInBytes;
			m_ByteCodes = new byte[sizeInBytes];

			memcpy_s((void*)m_ByteCodes, m_SizeInBytes, byteCodes, sizeInBytes);

			ID3D11PixelShader* pPS = nullptr;
			HRESULT hr = mDevice->CreatePixelShader(m_ByteCodes, m_SizeInBytes, nullptr, &pPS);
			if (FAILED(hr))
			{
				throw std::exception("CreatePixelShader failed.");
			}
			m_pShader = pPS;
		}

		~D3D11PixelShaderImpl()
		{
			if (m_ByteCodes != nullptr)
				delete[] m_ByteCodes;
		}

		void D3D11PixelShader::SetConstantBuffers(std::vector<TypeTrait<ConstantBuffer>::Ptr>& vBuffers)
		{

		}
		void D3D11PixelShader::SetShaderResources(std::vector<TypeTrait<ShaderResource>::Ptr>& vResources)
		{

		}

		void D3D11PixelShader::Apply()
		{
			if (m_pShader != nullptr)
				mDevice.GetImmediateContext()->PSSetShader(m_pShader, nullptr, 0);
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

