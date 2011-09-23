/*
Irrtum
Copyright (C) 2011 kahrl <kahrl@gmx.net>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef COMMON_HEADER
#define COMMON_HEADER

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <stdint.h>
#include <string.h>
#include <assert.h>

// png.h must be included before freetype headers, else we get setjmp.h conflicts
#include <png.h>
#include <ft2build.h>
#include FT_FREETYPE_H

using std::cout;
using std::cerr;
using std::endl;

using std::make_pair;
using std::pair;
using std::size_t;
using std::string;
using std::vector;

using std::stringstream;
using std::istringstream;
using std::ostringstream;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

const s32 IRRTUM_CHAR_MIN = 0x20;
const s32 IRRTUM_CHAR_MAX = 0x10ffff;

template<typename T>
inline T my_min(T a, T b)
{
    return a < b ? a : b;
}

template<typename T>
inline T my_max(T a, T b)
{
    return a > b ? a : b;
}

#endif
