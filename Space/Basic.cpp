#include "Basic.hpp"

namespace Space
{
	Object::~Object(){}

	Uncopyable::Uncopyable(){}
	Uncopyable::~Uncopyable(){}
	//Uncopyable::Uncopyable(const Uncopyable&){}
	//Uncopyable& Uncopyable::operator=(const Uncopyable&){ return *this; }

	Interface::Interface(){}
	Interface::~Interface(){}

	Archiveable::~Archiveable(){}
}