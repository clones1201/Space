#ifndef __SPACE_D3D11_SHADERREFLECTION_HPP__
#define __SPACE_D3D11_SHADERREFLECTION_HPP__

#include "D3D11RenderSystem/D3D11Prerequisites.hpp"
#include "D3D11Shader.hpp"

namespace Space
{
	namespace Render {
		class SPACE_D3D11_API D3D11ShaderReflection:
			virtual public Uncopyable,
			public SharedPtrObject<D3D11ShaderReflection>
		{
		public:
			uint32 GetInstructionCount() const;
			uint32 GetConstantBufferCount() const;
			uint32 GetShaderResourceCount() const;
		
			D3D11ShaderReflection(byte const* byteCodes, uint32 lengthInBytes);
		private:
			CComPtr<ID3D11ShaderReflection> m_pReflection = nullptr;
			D3D11_SHADER_DESC _Desc;
		};

		class SPACE_D3D11_API D3D11ShaderReflectionConstantBuffer : 
			public SharedPtrObject<D3D11ShaderReflectionConstantBuffer>
		{
		private:
			D3D11ShaderReflectionConstantBuffer(
				ID3D11ShaderReflectionConstantBuffer* pReflection,
				D3D11ShaderReflection* pShaderReflection);

			friend class D3D11ShaderReflection;
		};

		class SPACE_D3D11_API D3D11ShaderReflectionVariable : 
			public SharedPtrObject<D3D11ShaderReflectionVariable>
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

		class SPACE_D3D11_API D3D11ShaderReflectionBindResource : 
			public SharedPtrObject<D3D11ShaderReflectionBindResource>
		{
		private:
			D3D11ShaderReflectionBindResource(D3D11_SHADER_INPUT_BIND_DESC desc,
				D3D11ShaderReflection* pReflection);

			D3D11_SHADER_INPUT_BIND_DESC m_Desc;

			friend class D3D11ShaderReflection;
		};
	}
}
#endif