#ifndef __SPACE_D3D11_SHADER_HPP__
#define __SPACE_D3D11_SHADER_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"

#include "Shader.hpp"
#include "D3D11Prerequisites.hpp" 

namespace Space
{ 

	class D3D11VertexShader : public VertexShader
	{
	public:
		static D3D11VertexShader* LoadBinaryFromMemory(D3D11Device& device, byte const* byteCodes, uint32 sizeInBytes);

		virtual void SetConstantBuffers(std::vector<TypeTrait<ConstantBuffer>::Ptr>& vBuffers);
		virtual void SetShaderResources(std::vector<TypeTrait<ShaderResource>::Ptr>& vResources);
		 
		CComPtr<ID3D11VertexShader> GetShader() const;

		virtual ~D3D11VertexShader();
	protected:
		D3D11VertexShader(D3D11Device &device, 
			byte const* byteCodes, uint32 sizeInBytes);

	private:
		D3D11Device &mDevice;

		CComPtr<ID3D11VertexShader> m_pShader = nullptr;
		
		friend class D3D11PipelineState;
	};


	class D3D11PixelShader : public PixelShader
	{
	public:
		static D3D11PixelShader* LoadBinaryFromMemory(D3D11Device& device, byte const* byteCodes, uint32 sizeInBytes);

		virtual void SetConstantBuffers(std::vector<TypeTrait<ConstantBuffer>::Ptr>& vBuffers);
		virtual void SetShaderResources(std::vector<TypeTrait<ShaderResource>::Ptr>& vResources);
		  
		CComPtr<ID3D11PixelShader> GetShader() const;

		virtual ~D3D11PixelShader();
	protected:
		D3D11PixelShader(D3D11Device &device, byte const* byteCodes, uint32 sizeInBytes);

	private:

		D3D11Device &mDevice;

		CComPtr<ID3D11PixelShader> m_pShader = nullptr;

		friend class D3D11PipelineState;
	};




}

#endif