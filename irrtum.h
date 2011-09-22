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

#ifndef IRRTUM_HEADER
#define IRRTUM_HEADER

#include "common.h"

class Irrtum
{
public:
    Irrtum();
    ~Irrtum();

    std::string getLastError() const;

    bool initLibpng();
    bool initFreetype();
    std::string getLibpngVersion() const;
    std::string getFreetypeVersion() const;

    bool loadFace(std::string filename);

private:
    Irrtum(const Irrtum&);
    Irrtum& operator=(const Irrtum&);

    void freetypeError(FT_Error error);

    std::string m_error;
    FT_Library m_ftlibrary;
    bool m_ftinited;
    FT_Face m_face;
};

#endif

