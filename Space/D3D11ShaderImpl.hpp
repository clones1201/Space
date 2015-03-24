#ifndef __SPACE_D3D11SHADER_IMPL_HPP__
#define __SPACE_D3D11SHADER_IMPL_HPP__

#include "D3D11Prerequisites.hpp"
#include "D3D11Shader.hpp"

#include <atlbase.h>

namespace Space
{
	class D3D11ShaderImpl : virtual public D3D11Shader
	{
	private:
		CComPtr<ID3D11VertexShader>		pVertexShader = nullptr;
		CComPtr<ID3D11PixelShader>		pPixelShader = nullptr;
		CComPtr<ID3D11GeometryShader>	pGeometryShader = nullptr;
	public:
		D3D11ShaderImpl(D3D11Shader* parent);
		~D3D11ShaderImpl();

		void Apply();
	};

}

#endif