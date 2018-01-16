#pragma once

#include <string>
#include <unordered_map>
#include <sstream>
#include <algorithm>
#include <set>

#define GLEW_STATIC
#include <glew/glew.h>
#include <gl/GL.h>

#define dtor ~

namespace ns
{
	//Perchè sì
	using namespace std;

	typedef unsigned int u32;
	typedef signed   int i32;
	typedef float f32;

	//Identificatore unico di qualunque cosa
	typedef u32 ident;
}