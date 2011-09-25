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
#include "intervallist.h"
#include "rect.h"

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

    void setCharacterRanges(const IntervalList& cranges);

    bool loadFace(std::string filename, float size, float dpi);
    bool layout(s32 outwidth, s32 outheight);

private:
    Irrtum(const Irrtum&);
    Irrtum& operator=(const Irrtum&);

    bool getCharBitmapSize(s32 ch, s32& width, s32& height);
    bool getTotalBitmapSize(s32& area);
    bool tryLayout(s32 outwidth, s32 outheight, bool& tooLarge);

    void freetypeError(FT_Error error);

    std::string m_error;
    FT_Library m_ftlibrary;
    bool m_ftinited;
    FT_Face m_face;
    IntervalList m_cranges;
    s32 m_layoutwidth;
    s32 m_layoutheight;
    vector<Rect> m_layoutrects;
};

#endif

