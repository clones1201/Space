#ifndef __SPACE_SHADERRESOURCEMANAGER_HPP__
#define __SPACE_SHADERRESOURCEMANAGER_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"

namespace Space
{
	class ShaderResourcePool : public Uncopyable
	{
	public:
		static ShaderResourcePool* Create(RenderSystem* pRenderSys);

		void BindTexture2DResource(Name const& name, DeviceTexture2D* pTexture);
		void BindTextureBufferResourceByName(Name const& name, TextureBuffer* pTBuffer);
		void BindConstantBufferResourceByName(Name const& name, ConstantBuffer* pCBuffer);

		DeviceTexture2D* GetTexture2DResourceByName(Name const& name) const;
		TextureBuffer* GetTextureBufferResourceByName(Name const& name) const;
		ConstantBuffer* GetConstantBufferResourceByName(Name const& name) const;


		std::map<Name, std::shared_ptr<DeviceTexture2D>> m_Texture2DMap;
		std::map<Name, std::shared_ptr<ConstantBuffer>> m_CBufferMap;
		std::map<Name, std::shared_ptr<TextureBuffer>> m_TBufferMap;

	};
}

#endif