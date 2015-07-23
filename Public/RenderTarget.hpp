#ifndef __SPACE_RENDERTARGET_HPP__
#define __SPACE_RENDERTARGET_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"

namespace Space
{
	class SPACE_API RenderTarget : virtual public Interface
	{
	public:		
		virtual ~RenderTarget();

		inline uint32 GetWidth() const
		{
			return m_Width;
		}
		inline uint32 GetHeight() const
		{
			return m_Height;
		}
	protected:
		RenderTarget(uint32 width, uint32 height);

		uint32 m_Width, m_Height;
	};

	typedef std::shared_ptr<RenderTarget> RenderTargetPtr;
}

#endif 