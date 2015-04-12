#ifndef __SPACE_RENDERTARGET_HPP__
#define __SPACE_RENDERTARGET_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"

namespace Space
{
	class RenderTarget : virtual public Interface
	{
	public:
		virtual bool Activate(DepthStencilView* pDepthStencil) = 0;
		virtual void Deactivate() = 0;
		virtual void Clear(float clearColor[4]) = 0;
		
		virtual ~RenderTarget();

		int32 GetWidth() const;
		int32 GetHeight() const;

	protected:
		RenderTarget(int32 width, int32 height);
		
		int32 m_Width, m_Height;
	};
}

#endif 