#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <GL\glew.h>
#include <GL\wglew.h>
//#include <GL\freeglut.h>

#include <Windows.h>

#include <vector>
#include <map>
#include <set>
#include <memory>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

#define DEFINE_GET_SET(type,name) private:\
			type name; \
			public:type Get##name(){\
				return name;}\
			void Set##name(type _##name){\
				name = _##name;}private:

#define DEFINE_IS_SET(name) private:\
			bool name; \
			public:bool is##name(){\
				return name;}\
			void Set##name##True(){\
				name = true;}\
			void Set##name##False(){\
				name = false;}private:

#include "../common/lodepng/lodepng.h"

namespace space{

	typedef unsigned char uchar;
	typedef unsigned short ushort;
	typedef unsigned int uint;
	typedef unsigned long ulong;

}

#endif