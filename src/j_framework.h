#ifndef _jframework
#define _jframework

#include "j_framework_headers.h"
#include "j_framework_defs.h"

// utilities
#include "Util.h"	// hashes, crc, int2string, maya strings/locators utils
#include "Logger.h"	// Logging class
#include "Rand.h"	// wrapper class for STL rand
#include "Bresenham.h"	// iterator
#include "Timer.h"	// profiling timer
#include "Range.h"	// ranges parser/converter

// jmesh
#include "Mat4.h"	// matrix class
#include "Uv.h"		// u/v num
#include "Pt.h"		// x/y/z num, serves as jvec as well
#include "Point.h"	// uv+pt
#include "Poly.h"	// <int>
#include "Mesh.h"	// <points>+<polys>

#include "Obj.h"	// OBJ reader
#include "Sequence.h"	// OBJ reader

#endif
