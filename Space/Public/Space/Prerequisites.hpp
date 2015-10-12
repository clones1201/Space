#ifndef __SPACE_PREREQUISITES_HPP__
#define __SPACE_PREREQUISITES_HPP__

#include <cstdlib>

extern "C" void * __cdecl memset(void *, int, size_t);

#include "Common/Common.hpp"
#include "RenderSystem/RenderSystem.hpp"
#include "RenderSystem/D3D11RenderSystem.hpp"

#ifdef SPACE_EXPORTS
#define SPACE_API __declspec(dllexport)
#else
#define SPACE_API __declspec(dllimport)
#endif

#include "rapidjson/rapidjson.h"
#include "rapidjson/writer.h"
#include "rapidjson/document.h"

#if SPACE_PLATFORM == SPACE_WIN32
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define CHECK_MEMORY_LEAKS_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new CHECK_MEMORY_LEAKS_NEW
#endif
#endif

namespace Space
{
	class Core;
	class Game;
	class Window;
	class WindowUtility;
	class EventListener;
	class EventDispatcher;
	class MouseListener;
	class KeyboardListener;
}

#endif