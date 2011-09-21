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

#include <iostream>
#include <string>
#include <sstream>

using std::cout;
using std::cerr;
using std::endl;

using std::string;

using std::stringstream;
using std::istringstream;
using std::ostringstream;

// png.h must be included before freetype headers, else we get setjmp.h conflicts
#include <png.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#endif

