#ifndef __SPACE_SHARED_UTILITY_HPP__
#define __SPACE_SHARED_UTILITY_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"

namespace Space
{
	class SPACE_API Name
	{
	private:
		class Impl;
		TypeTrait<Impl>::Ptr m_impl;
	public:
		Name();
		Name(std::string const& name);
		Name(std::wstring const& name);
		Name(Name const& param);
		Name(Name && param);
		std::wstring ToString() const;
		bool operator==(Name const& param) const;
		bool operator<(Name const& param) const;
		bool operator>(Name const& param) const;
		int32 GetHashCode() const;

		std::ostream& Write(std::ostream& archiver) const;
		std::istream& Read(std::istream& archiver);
	}; 

	class UidGenerator{
	private:
		std::list<std::pair<uint, uint>> m_UidList;

	public:
		UidGenerator();
		uint32 GetNextValidId();
		void RevokeId(uint32 id);
	};
}

namespace std
{
	template<>
	class hash < Space::Name >
	{
	public:
		Space::int32 operator()(Space::Name const& param) const;
	};
}

#endif