#ifndef __SPACE_D3D11_RENDERSYSTEM_HPP__
#define __SPACE_D3D11_RENDERSYSTEM_HPP__

#include "RenderSystem.hpp"
#include "Basic.hpp"
#include "D3D11Prerequisites.hpp" 

namespace Space
{
	class D3D11RenderSystem : public RenderSystem 
	{
	public:
		static D3D11RenderSystem* Create();

		virtual D3D11Device* GetDevice() const throw()= 0;
		
		friend class Core;

		virtual ~D3D11RenderSystem();
	protected:
		D3D11RenderSystem();
	};

}

#endif 
