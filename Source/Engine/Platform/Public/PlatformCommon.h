#pragma once

#include "Prerequisites.hpp"
#include "Basic.hpp"

#ifdef PLATFORM_EXPORTS
#define SPACE_PLATFORM_API __declspec(dllexport)
#else
#define SPACE_PLATFORM_API __declspec(dllimport)
#endif
