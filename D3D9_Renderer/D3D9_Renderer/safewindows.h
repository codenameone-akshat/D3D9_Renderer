//safer include for windows.h
//removes the std::min and std::max compilation error
//error C2589: '(': illegal token on right side of '::'

#pragma once
#ifdef NOMINMAX
#include <windows.h>
#else
#define NOMINMAX
#include <windows.h>
#undef NOMINMAX
#endif