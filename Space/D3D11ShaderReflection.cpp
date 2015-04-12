#include "Log.h"
#include "Utility.h"
#include "SharedUtility.hpp"

#include <d3d11shader.h>
#include "D3D11ShaderReflection.hpp"

namespace Space
{

	class D3D11ShaderReflectionImpl : public D3D11ShaderReflection
	{
	private:

		CComPtr<ID3D11ShaderReflection> m_pReflection = nullptr;

		D3D11_SHADER_DESC m_Desc;

		std::vector<ID3D11ShaderReflectionConstantBuffer*> m_ConstantBufferDescriptorList;
		std::vector<ID3D11ShaderReflectionVariable*> m_VariableDescriptorList;

	public:



	};

}