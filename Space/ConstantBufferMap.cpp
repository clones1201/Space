#include "Log.h"
#include "Utility.h"
#include "SharedUtility.hpp"
#include "RenderSystem.hpp"
#include "DeviceBuffer.hpp"
#include "ConstantBufferMap.hpp"

namespace Space
{
	ConstantBufferMap::ConstantBufferMap(RenderSystem* pRenderSys, std::string const& name)
		:m_pRenderSystem(pRenderSys), m_Name(name)
	{
	}

	void ConstantBufferMap::AddVariable(std::string const& name, uint32 size, uint32 offset)
	{
		if (m_IsComplete)
			return; 
		m_vLayout.insert( 
			std::pair<Space::Name, ShaderVariableDesc>(
			Name(name), 
			ShaderVariableDesc{ Name(name).ToString().c_str(), size, offset })
			);
	}

	ShaderVariableDesc ConstantBufferMap::GetVariableDescByIndex(uint32 index) const
	{
		uint32 i = 0;
		auto iter = m_vLayout.begin();

		if (!m_IsComplete)
			goto failed;

		for (; iter != m_vLayout.end(), i < index; ++iter, ++i);

		if (i == index)
		{
			return iter->second;
		}

	failed:
		return ShaderVariableDesc{ nullptr, -1, -1 };
	}

	ShaderVariableDesc ConstantBufferMap::GetVariableDescByName(std::string const& name) const
	{
		try
		{
			return m_vLayout.at(name);
		}
		catch (std::out_of_range &e)
		{
			Log("No such field in this constant buffer");
			return ShaderVariableDesc{ nullptr, -1, -1 };
		}
	}
	bool ConstantBufferMap::SetVariableValue(std::string const& name, byte const* pData)
	{
		if (!m_IsComplete)
			return false;
		auto field = m_vLayout[name];
		return m_pBuffer->Update(field.offset, field.size, pData); 
	}
	uint32 ConstantBufferMap::GetVariablesCount() const
	{
		if (!m_IsComplete) return 0;
		return m_vLayout.size();
	}
	void ConstantBufferMap::Complete()
	{
		m_Size = Alignment(m_Size);
		m_pBuffer.reset(ConstantBuffer::Create(m_pRenderSystem, nullptr, m_Size));

		m_IsComplete = true;
	}
	void ConstantBufferMap::Update()
	{
		m_pBuffer->UpdateToDevice();
	}

	std::string ConstantBufferMap::GetName() const
	{
		return m_Name;
	}
	bool ConstantBufferMap::IsComplete() const
	{
		return m_IsComplete;
	}
	uint32 ConstantBufferMap::GetBufferSize() const
	{
		return m_Size;
	}

	ConstantBufferMap::~ConstantBufferMap()
	{}

	ConstantBufferMap* ConstantBufferMap::Create(RenderSystem* pRenderSystem, std::string const& name)
	{
		try
		{
			return new ConstantBufferMap(pRenderSystem, name);
		}
		catch (std::exception &e)
		{
			Log(e.what());
			return nullptr;
		}
	}
}
