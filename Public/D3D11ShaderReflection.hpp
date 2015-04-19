#ifndef __SPACE_D3D11_SHADERREFLECTION_HPP__
#define __SPACE_D3D11_SHADERREFLECTION_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"
#include "ShaderReflection.hpp"
#include "D3D11Prerequisites.hpp"

namespace Space
{

	ShaderVariableClass GetSVCFromD3DSVC(D3D_SHADER_VARIABLE_CLASS svc);
	ShaderVariableType GetSVTFromD3DSVT(D3D_SHADER_VARIABLE_TYPE svt);

	class D3D11ShaderReflection : public ShaderReflection
	{
	public:

	private:
		D3D11ShaderReflection(VertexShader* pShader);
		D3D11ShaderReflection(PixelShader* pShader);

		void _Initialize(byte const* byteCodes, uint32 lengthInBytes);

		friend class ShaderReflection;
		
		CComPtr<ID3D11ShaderReflection> m_pReflection = nullptr;
		D3D11_SHADER_DESC m_Desc;
	};

	class D3D11ShaderReflectionConstantBuffer : public ShaderReflectionConstantBuffer
	{
	private:
		D3D11ShaderReflectionConstantBuffer(
			ID3D11ShaderReflectionConstantBuffer* pReflection,
			D3D11ShaderReflection* pShaderReflection);
		
		friend class D3D11ShaderReflection;
	};
	
	class D3D11ShaderReflectionVariable : public ShaderReflectionVariable
	{

	private:
		D3D11ShaderReflectionVariable(
			ID3D11ShaderReflectionVariable* pIVariable,
			D3D11ShaderReflectionConstantBuffer* pCB);
		D3D11ShaderReflectionVariable(
			ID3D11ShaderReflectionType* pType,
			D3D11ShaderReflectionVariable* pVariable);

		void _Initialize(ID3D11ShaderReflectionType* pType);

		ID3D11ShaderReflectionVariable* m_pVariable = nullptr;
		D3D11_SHADER_VARIABLE_DESC m_Desc;

		ID3D11ShaderReflectionType* m_pType = nullptr;
		D3D11_SHADER_TYPE_DESC m_TypeDesc;

		friend class D3D11ShaderReflectionConstantBuffer;
	};

	class D3D11ShaderReflectionBindResource : public ShaderReflectionBindResource
	{

	private:
		D3D11ShaderReflectionBindResource(D3D11_SHADER_INPUT_BIND_DESC desc,
			D3D11ShaderReflection* pReflection);

		D3D11_SHADER_INPUT_BIND_DESC m_Desc;

		friend class D3D11ShaderReflection;
	};
}

#endif