#ifndef __SPACE_RENDERSYSTEM_SHADERRESOURCE_HPP__
#define __SPACE_RENDERSYSTEM_SHADERRESOURCE_HPP__

#include "RenderSystem/Prerequisites.hpp"

namespace Space
{
	
	class SPACE_RENDERSYSTEM_API ShaderResource : private Interface
	{
	public:  
		virtual ~ShaderResource();
	protected:
		ShaderResource();
	};
	typedef std::shared_ptr<ShaderResource> ShaderResourcePtr;
}

#endif