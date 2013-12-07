#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <GL\glew.h>
#include <GL\wglew.h>
#include <GL\freeglut.h>

#include <Windows.h>
#include <mmsystem.h>
//#include <d3d9.h>
#include <d3dx9.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )

#include <vector>
#include <map>
#include <set>

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

#endif