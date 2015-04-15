#ifndef __SPACE_SHADERRESOURCEMANAGER_HPP__
#define __SPACE_SHADERRESOURCEMANAGER_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"
#include "SharedUtility.hpp"

namespace Space
{
	class ShaderResourcePool : public Uncopyable
	{
	public:
		uint32 RegistShaderResource(TypeTrait<ShaderResource>::Ptr pResource);
		void UnregistShaderResource(TypeTrait<ShaderResource>::Ptr pResource);
		ShaderResource* GetShaderResourceById(uint32 id);

		uint32 RegistConstantBuffer(TypeTrait<ConstantBuffer>::Ptr pCBuffer);
		void UnregistConstantBuffer(TypeTrait<ConstantBuffer>::Ptr pCBBuffer);
		ConstantBuffer* GetConstantBufferById(uint32 id);

		void ClearAllShaderResource();
		void ClearAllConstantBuffer();
		//Sampler* GetShaderResourceById(uint32 id);
	private: 
		std::map<uint, TypeTrait<ShaderResource>::Ptr> m_ShaderResourceMap;
		std::map<uint, TypeTrait<ConstantBuffer>::Ptr> m_ConstantBufferMap;
	};
}

#endif