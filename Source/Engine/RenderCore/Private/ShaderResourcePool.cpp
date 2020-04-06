#include "Common/Log.h"
#include "Common/Utility.hpp"
#include "Space/ShaderResourcePool.hpp"

namespace Space
{

	/*uint32 ShaderResourcePool::RegistShaderResource(
		TypeTrait<ShaderResource>::Ptr pResource)
	{
		auto iter = std::find_if(
			m_ShaderResourceMap.begin(),
			m_ShaderResourceMap.end(),
			[=](std::pair<uint, TypeTrait<ShaderResource>::Ptr> param)->bool
		{
			return param.second == pResource;
		}
		);

		if (iter != m_ShaderResourceMap.end())
		{
			return iter->first;
		}
		else
		{
			m_ShaderResourceMap.insert(
				std::pair<uint, TypeTrait<ShaderResource>::Ptr>(
				(uint)pResource.get(),
				pResource
				));
			return (uint)pResource.get();
		}
	}

	void ShaderResourcePool::UnregistShaderResource(
		TypeTrait<ShaderResource>::Ptr pResource)
	{
		m_ShaderResourceMap.erase((uint)pResource.get());
	}

	ShaderResource* ShaderResourcePool::GetShaderResourceById(uint32 id)
	{
		try
		{
			return m_ShaderResourceMap.at(id).get();
		}
		catch (std::out_of_range &e)
		{
			Log(e.what());
			return nullptr;
		}
	}

	uint32 ShaderResourcePool::RegistConstantBuffer(
		TypeTrait<ConstantBuffer>::Ptr pCBuffer)
	{
		auto iter = std::find_if(
			m_ConstantBufferMap.begin(),
			m_ConstantBufferMap.end(),
			[=](std::pair<uint, TypeTrait<ConstantBuffer>::Ptr> param)->bool
		{
			return param.second == pCBuffer;
		}
		);

		if (iter != m_ConstantBufferMap.end())
		{
			return iter->first;
		}
		else
		{
			m_ConstantBufferMap.insert(
				std::pair<uint, TypeTrait<ConstantBuffer>::Ptr>(
				(uint)pCBuffer.get(),
				pCBuffer
				));
			return (uint)pCBuffer.get();
		}
	}
	void ShaderResourcePool::UnregistConstantBuffer(
		TypeTrait<ConstantBuffer>::Ptr pCBBuffer)
	{
		m_ConstantBufferMap.erase((uint)pCBBuffer.get());
	}
	ConstantBuffer* ShaderResourcePool::GetConstantBufferById(uint32 id)
	{
		try
		{
			return m_ConstantBufferMap.at(id).get();
		}
		catch (std::out_of_range &e)
		{
			Log(e.what());
			return nullptr;
		}
	}

	void ShaderResourcePool::ClearAllShaderResource()
	{
		m_ShaderResourceMap.clear();
	}
	void ShaderResourcePool::ClearAllConstantBuffer()
	{
		m_ConstantBufferMap.clear();
	}
*/

	

}