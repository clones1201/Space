#include "Log.h"
#include "Utility.h"
#include "SharedUtility.hpp"
#include "RenderSystem.hpp"
#include "DeviceBuffer.hpp"
#include "ConstantBufferMap.hpp"

namespace Space
{
	class ConstantBufferMapImpl : public ConstantBufferMap
	{
	protected:
		std::unique_ptr<ConstantBuffer> m_pBuffer;

		std::unordered_map<Name, FieldDesc> m_vLayout;
		std::string m_Name;
		uint m_Size = 0;
		std::vector<byte> m_pRaw;

		bool m_IsComplete = false;

		RenderSystem* m_pRenderSystem = nullptr;
	public:
		ConstantBufferMapImpl(RenderSystem* pRenderSys, std::string const& name)
			:m_pRenderSystem(pRenderSys), m_Name(name)
		{
		}

		virtual void AddField(std::string const& name, uint size)
		{
			if (m_IsComplete)
				return;
			/*int offset = 0;
			for (auto iter = m_vLayout.begin(); iter != m_vLayout.end(); ++iter)
			{
			offset += (*iter).size;
			}*/
			int offset = m_Size;
			m_Size += size;
			m_vLayout[name] = FieldDesc{ Name(name).ToString().c_str(), size, offset };
		}
		virtual FieldDesc GetFieldDescByIndex(uint index) const
		{
			uint i = 0;
			auto iter = m_vLayout.begin();

			if (!m_IsComplete)
				goto failed;

			for (; iter != m_vLayout.end(), i < index; ++iter, ++i);

			if (i == index)
			{
				return iter->second;
			}

		failed:
			return FieldDesc{ nullptr, -1, -1 };
		}

		virtual FieldDesc GetFieldDescByName(std::string const& name) const
		{
			try
			{
				return m_vLayout.at(name);
			}
			catch (std::out_of_range &e)
			{
				Log("No such field in this constant buffer");
				return FieldDesc{ nullptr, -1, -1 };
			}
		}
		virtual bool SetFieldValue(std::string const& name, byte const* pData)
		{
			if (!m_IsComplete)
				return false;
			auto field = m_vLayout[name];
			auto error = memcpy_s(m_pRaw.data() + field.offset, field.size, pData, field.offset);
			if (error == 0) return true;
			return false;
		}
		virtual uint GetFiledsCount() const
		{
			if (!m_IsComplete) return 0;
			return m_vLayout.size();
		}
		virtual void Complete()
		{
			m_Size = Alignment(m_Size);
			m_pBuffer.reset(ConstantBuffer::Create(m_pRenderSystem, nullptr, m_Size));
			m_pRaw.resize(m_Size);

			m_IsComplete = true;
		}
		virtual void Update()
		{
			m_pBuffer->Update(m_Size, m_pRaw.data());
		}

		virtual std::string GetName() const
		{
			return m_Name;
		}
		virtual bool IsComplete() const
		{
			return m_IsComplete;
		}
		virtual uint GetBufferSize() const
		{
			return m_Size;
		}

		~ConstantBufferMapImpl()
		{}

		ConstantBufferMapImpl()
		{}

	};

	ConstantBufferMap* ConstantBufferMap::Create(RenderSystem* pRenderSystem, std::string const& name)
	{
		try
		{
			return new ConstantBufferMapImpl(pRenderSystem, name);
		}
		catch (std::exception &e)
		{
			Log(e.what());
			return nullptr;
		}
	}

	ConstantBufferMap::ConstantBufferMap(){}

	ConstantBufferMap::~ConstantBufferMap(){}
}
