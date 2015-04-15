#ifndef __SPACE_BASIC_HPP__
#define __SPACE_BASIC_HPP__
 
#include "Prerequisites.hpp"

namespace Space
{
	template <typename _Type>
	class TypeTraitBase
	{
	public:
		typedef typename std::remove_reference< _Type >::type*	RawPtr;
		typedef typename std::remove_reference< _Type >::type*	RawConstPtr;
		typedef typename std::remove_reference< _Type >::type&	Ref;
		typedef typename std::remove_reference< _Type >::type const&  ConstRef;
		typedef typename std::shared_ptr< typename std::remove_reference< _Type >::type > Ptr;
		typedef typename std::shared_ptr< typename std::remove_reference< _Type >::type const > ConstPtr;
	};

	template < typename _Type > class TypeTrait : public TypeTraitBase < _Type > {};

	class Object
	{
	public:
		virtual ~Object();
	};

	class Uncopyable{
	protected:
		Uncopyable();
		virtual ~Uncopyable();
	private:
		Uncopyable(const Uncopyable&) = delete;
		Uncopyable& operator=(const Uncopyable&) = delete;
	};

	class Interface : private Uncopyable
	{
	protected:
		Interface(); 
	public:
		virtual ~Interface();
	};


}

#endif