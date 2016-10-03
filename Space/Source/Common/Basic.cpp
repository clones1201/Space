#include "Common/Basic.hpp"
#include <atomic>

namespace Space
{

	Uncopyable::Uncopyable(){}
	Uncopyable::~Uncopyable(){}
	//Uncopyable::Uncopyable(const Uncopyable&){}
	//Uncopyable& Uncopyable::operator=(const Uncopyable&){ return *this; }

	Interface::Interface(){}
	Interface::~Interface(){}

	RefCountObject::RefCountObject()
	{}

	RefCountObject::~RefCountObject()
	{}
		
	Archiveable::~Archiveable(){}
}