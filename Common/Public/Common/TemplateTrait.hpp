#ifndef __SPACE_COMMON_TEMPLATE_TRAIT_HPP__
#define __SPACE_COMMON_TEMPLATE_TRAIT_HPP__

#include "Prerequisites.hpp"

namespace Space
{
	template<typename Type>
	class ObjectTrait
	{
	public:
		typedef std::shared_ptr<Type> Ptr;
		typedef std::shared_ptr<const Type> ConstPtr;
		typedef Type& Ref;
		typedef Type const& ConstRef;
	};

}


#endif