#pragma once

#include "Prerequisites.hpp"
#include "Basic.hpp"

#ifdef PLATFORM_EXPORTS
#define PLATFORM_API __declspec(dllexport)
#else
#define PLATFORM_API __declspec(dllimport)
#endif
