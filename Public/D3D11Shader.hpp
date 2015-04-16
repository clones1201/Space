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

		virtual void SetConstantBuffers(std::vector<TypeTrait<ConstantBuffer>::Ptr>& vBuffers) = 0;
		virtual void SetShaderResources(std::vector<TypeTrait<ShaderResource>::Ptr>& vResources) = 0;

		virtual void Apply() = 0;

		virtual ~D3D11VertexShader();
	protected:
		D3D11VertexShader();

	};


	class D3D11PixelShader : public PixelShader
	{
	public:
		static D3D11PixelShader* LoadBinaryFromMemory(D3D11Device& device, byte const* byteCodes, uint32 sizeInBytes);

		virtual void SetConstantBuffers(std::vector<TypeTrait<ConstantBuffer>::Ptr>& vBuffers) = 0;
		virtual void SetShaderResources(std::vector<TypeTrait<ShaderResource>::Ptr>& vResources) = 0;

		virtual void Apply() = 0;

		virtual ~D3D11PixelShader();
	protected:
		D3D11PixelShader();

	};




}

#endif