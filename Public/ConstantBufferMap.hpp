#ifndef __SPACE_CONSTANTBUFFER_MAP_HPP__
#define __SPACE_CONSTANTBUFFER_MAP_HPP__

#include "Prerequisites.hpp"
#include "basic.hpp"

namespace Space
{

	class ConstantBufferMap : virtual public Interface
	{
	public:
		struct FieldDesc
		{
			wchar_t const* name;
			int size;
			int offset;
		};
		static ConstantBufferMap* Create(RenderSystem* pRenderSys,std::string const& name);

		virtual void AddField(std::string const& name, uint size) = 0;
		virtual FieldDesc GetFieldDescByIndex(uint index) const = 0;
		virtual FieldDesc GetFieldDescByName(std::string const& name) const = 0;
		virtual bool SetFieldValue(std::string const& name, byte const* pData) = 0;
		virtual uint GetFiledsCount() const = 0;

		virtual void Complete() = 0;
		virtual void Update() = 0;

		virtual std::string GetName() const = 0;
		virtual bool IsComplete() const = 0;
		virtual uint GetBufferSize() const = 0;

		virtual ~ConstantBufferMap();
	protected:
		ConstantBufferMap();
	};

}

#endif