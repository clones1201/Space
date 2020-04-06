#include "RenderSystem.hpp"
#include "ShaderReflection.hpp"

namespace Space
{
	namespace Render
	{
		//
		// ShaderReflectionVariable Implement
		//
		namespace Details {
			
						
			void ShaderReflectionVariable::SetStruct(
				byte const* pData, size_t lengthInBytes)
			{
				pParentCB->GetBuffer()->Update(_StartOffset, std::min(lengthInBytes, _Size), (byte*)pData);
			}

			//
			// ShaderReflectionBindResource Implement
			//

			ShaderReflectionBindResource::ShaderReflectionBindResource(
				ShaderReflection* pReflection)
				:_pReflection(pReflection)
			{}

			ShaderReflectionBindResource::~ShaderReflectionBindResource()
			{}

			void SetSampler()
			{}
			ShaderResource* ShaderReflectionBindResource::GetShaderResource() const
			{
				return _pResourceView;
			}
			void ShaderReflectionBindResource::UnSetShaderResource()
			{
				_pResourceView = nullptr;
			}
		}
	}
}