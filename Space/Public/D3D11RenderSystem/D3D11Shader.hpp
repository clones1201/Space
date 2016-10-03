#ifndef __SPACE_D3D11_SHADER_HPP__
#define __SPACE_D3D11_SHADER_HPP__


#include "D3D11RenderSystem/D3D11Prerequisites.hpp" 
#include "D3D11RenderSystem/D3D11Device.hpp"
#include "RenderSystem/Shader.hpp"

namespace Space
{ 
	class SPACE_D3D11_API D3D11ShaderBase : virtual public ShaderBase
	{
	public:
		virtual ShaderReflection* CreateReflection();

	};

	class SPACE_D3D11_API D3D11VertexShader : public VertexShader, public D3D11ShaderBase
	{
	public:
		static D3D11VertexShader* LoadBinaryFromMemory(D3D11DevicePtr device, byte const* byteCodes, size_t sizeInBytes);
				 
		CComPtr<ID3D11VertexShader> GetShader() const;
		
		virtual ~D3D11VertexShader();
	protected:
		D3D11VertexShader(D3D11DevicePtr device, 
			byte const* byteCodes, uint32 sizeInBytes);

	private:
		D3D11DevicePtr mDevice;

		CComPtr<ID3D11VertexShader> m_pShader = nullptr;
		
		friend class D3D11PipelineState;
	};


	class SPACE_D3D11_API D3D11PixelShader : public PixelShader, public D3D11ShaderBase
	{
	public:
		static D3D11PixelShader* LoadBinaryFromMemory(D3D11DevicePtr device, byte const* byteCodes, size_t sizeInBytes);

		CComPtr<ID3D11PixelShader> GetShader() const;
		
		virtual ~D3D11PixelShader();
	protected:
		D3D11PixelShader(D3D11DevicePtr device, byte const* byteCodes, size_t sizeInBytes);

	private:

		D3D11DevicePtr mDevice;

		CComPtr<ID3D11PixelShader> m_pShader = nullptr;

		friend class D3D11PipelineState;
	};




}

#endif