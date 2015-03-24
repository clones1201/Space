#include "Log.h"
#include "Utility.h"
#include "SharedUtility.hpp"

namespace Space
{
	class Name::Impl
	{
	public:
		typedef std::map<std::weak_ptr<Name::Impl>, std::wstring, std::owner_less<std::weak_ptr<Name::Impl>>> NameMapType;
		
		static NameMapType g_NameMap;

		std::weak_ptr<Name::Impl> ptr;

		Impl(){}
		Impl(const std::wstring& name){}
		~Impl()
		{
			g_NameMap.erase(ptr);
		} 
	};

	Name::Name()
	{
		Name(TEXT(""));
	}
	Name::Name(std::string const& name)
		:Name(str2wstr(name))
	{
	}
	Name::Name(std::wstring const& name)
	{
		bool found = false;
		std::for_each(Impl::g_NameMap.begin(), Impl::g_NameMap.end(),
			[&name, this, &found](std::pair<std::weak_ptr<Name::Impl>, std::wstring> elem)
		{
			if (elem.second == name){
				this->m_impl = elem.first.lock();
				found = true;
			}
		});
		if (!found){
			m_impl.reset(new Name::Impl(name));
			m_impl->ptr = m_impl;
			Impl::g_NameMap.insert(Impl::NameMapType::value_type(m_impl, name));
		}
	}
	Name::Name(Name const& param)
		:m_impl(param.m_impl)
	{}
	Name::Name(Name&& param)
	{
		std::swap(param.m_impl, this->m_impl);
	}
	std::wstring Name::ToString() const
	{
		try{
			return Impl::g_NameMap.at(m_impl);
		}
		catch (std::out_of_range)
		{
			return TEXT("");
		}
	}
	bool Name::operator==(const Name&param) const
	{
		return m_impl == param.m_impl;
	}
	bool Name::operator<(const Name&param) const
	{
		return m_impl < param.m_impl;
	}
	bool Name::operator>(const Name&param) const
	{
		return m_impl > param.m_impl;
	}
	int Name::GetHashCode() const
	{
		std::hash<TypeTrait<Name::Impl>::Ptr> hasher;
		return hasher(m_impl);
	}

	std::ostream& Name::Write(std::ostream& archiver) const
	{
		Space::Write(archiver, ToString());
		return archiver;
	}
	std::istream& Name::Read(std::istream& archiver)
	{
		std::string name;
		Space::Read(archiver, name);
		(*this) = Name(name);
		return archiver;
	}
}

int std::hash<Space::Name>::operator()(Space::Name const& param) const
{
	return param.GetHashCode();
}
