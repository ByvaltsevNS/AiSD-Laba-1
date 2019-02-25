#pragma once
#include "shape.h"
#include <exception>
#include <string>

struct Inc_Point : public std::exception {
	Inc_Point(const char* s): exception(s) {}
};

struct Inc_Par : public std::exception {
	Inc_Par(const char* s) : exception(s) {}
};

struct Size_Error: public std::exception {
	Size_Error(const char* s) : exception(s) {}
};

//class Inc_Point : public std::exception {
//	Inc_Point(char* s) : exception(s) {}
//};
