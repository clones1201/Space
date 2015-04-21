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

	class SPACE_API Object
	{
	public:
		virtual ~Object();
	};

	class SPACE_API Uncopyable{
	protected:
		Uncopyable();
		virtual ~Uncopyable();
	private:
		Uncopyable(const Uncopyable&) = delete;
		Uncopyable& operator=(const Uncopyable&) = delete;
	};

	class SPACE_API Interface : private Uncopyable
	{
	protected:
		Interface(); 
	public:
		virtual ~Interface();
	};

	class SPACE_API Archiveable
	{
	public:
		virtual ~Archiveable();
		virtual std::ostream& Write(std::ostream& archiver) const = 0;
		virtual std::istream& Read(std::istream& archiver) = 0;
	};

#define TRY_CATCH_LOG(expression,result) try{\
	expression; }\
	catch(std::exception& e){\
	Log("%s\n",e.what());\
	result;}

#define TRY_CATCH_OUT_OF_RANGE(expression,result)  try{\
	expression; }\
	catch(std::out_of_range&){\
	result;}

}

#endif