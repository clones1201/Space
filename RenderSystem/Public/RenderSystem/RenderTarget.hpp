#ifndef __SPACE_RENDERSYSTEM_RENDERTARGET_HPP__
#define __SPACE_RENDERSYSTEM_RENDERTARGET_HPP__

#include "RenderSystem/Prerequisites.hpp"
#include "RenderSystem/Shared.hpp"

namespace Space
{
	class SPACE_RENDERSYSTEM_API RenderTarget : private Interface
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