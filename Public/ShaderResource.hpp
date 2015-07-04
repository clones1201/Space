#ifndef __SPACE_SHADERRESOURCE_HPP__
#define __SPACE_SHADERRESOURCE_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"
#include "RenderTarget.hpp"

namespace Space
{
	
	class ShaderResource : virtual public Interface 
	{
	public:  
		virtual ~ShaderResource();
	protected:
		ShaderResource();
	};
	typedef std::shared_ptr<ShaderResource> ShaderResourcePtr;
}

#endif