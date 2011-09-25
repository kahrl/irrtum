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

#include "common.h"
#include "irrtum.h"
#include "graybitmap.h"

// The 26.6 fixed point number format is one of the number formats used
// by FreeType. It is a signed 32-bit int that contains the fractional part
// in the lowest 6 bits, and the integral part in the remaining 26 bits.
inline s32 floor_26dot6(FT_F26Dot6 x)
{
    return x / 64;
}
inline s32 ceil_26dot6(FT_F26Dot6 x)
{
    return (x + 63) / 64;
}

Irrtum::Irrtum():
    m_error("No error"),
    m_ftlibrary(0),
    m_ftinited(false),
    m_cranges(),
    m_face(0),
    m_layoutwidth(0),
    m_layoutheight(0),
    m_layoutrects(),
    m_graybitmap(0)
{
}

Irrtum::~Irrtum()
{
    if (m_ftinited)
    {
        FT_Done_FreeType(m_ftlibrary);  // ignoring errors
        m_ftinited = false;
    }

    if (m_graybitmap != 0)
        delete m_graybitmap;
}

std::string Irrtum::getLastError() const
{
    return m_error;
}

bool Irrtum::initLibpng()
{
    // Nothing to do here.
    return true;
}

bool Irrtum::initFreetype()
{
    if (m_ftinited)
        return true;

    FT_Error error = FT_Init_FreeType(&m_ftlibrary);
    if (error)
    {
        freetypeError(error);
        return false;
    }
    else
    {
        m_ftinited = true;
        return true;
    }
}

std::string Irrtum::getLibpngVersion() const
{
    png_uint_32 vn = png_access_version_number();
    int major = (vn / 10000);
    int minor = ((vn / 100) % 100);
    int patch = vn % 100;
    std::ostringstream os;
    os << major << "." << minor << "." << patch;
    return os.str();
}

std::string Irrtum::getFreetypeVersion() const
{
    int major = -1, minor = -1, patch = -1;
    FT_Library_Version(m_ftlibrary, &major, &minor, &patch);
    std::ostringstream os;
    os << major << "." << minor << "." << patch;
    return os.str();
}

void Irrtum::setCharacterRanges(const IntervalList& cranges)
{
    m_cranges = cranges;
}

bool Irrtum::loadFace(std::string filename, float size, float dpi)
{
    FT_Error error;
    error = FT_New_Face(m_ftlibrary, filename.c_str(), 0, &m_face);
    if (error)
    {
        freetypeError(error);
        return false;
    }
    error = FT_Set_Char_Size(m_face, 0, size * 64, dpi, dpi);
    if (error)
    {
        freetypeError(error);
        return false;
    }

    //FT_GlyphSlot slot = m_face->glyph;
    //wchar_t text[] = L"Hello world";
    //int num_chars = 11;
    //for (int n = 0; n < num_chars; ++n)
    //{
    //    error = FT_Load_Char(m_face, text[n], FT_LOAD_RENDER);
    //    if (error)
    //    {
    //        freetypeError(error);
    //        return false;
    //    }
    //
    //    GrayBitmap bmp(&slot->bitmap);
    //    bmp.debug();
    //}

    return true;
}

bool Irrtum::layout(s32 outwidth, s32 outheight)
{
    bool tooLarge = false;
    if (outwidth > 0)
    {
        // user specified output width
        return tryLayout(outwidth, outheight, tooLarge);
    }
    else
    {
        // compute minimal output width
        s32 totalArea;
        if (!getTotalBitmapSize(totalArea))
            return false;

        for (outwidth = 16; outwidth <= IRRTUM_MAX_AUTOSIZE; outwidth *= 2)
        {
            if (outwidth * outwidth < totalArea)
                continue;
            if (tryLayout(outwidth, outwidth, tooLarge))
                return true;
            if (!tooLarge)
                return false;  // failed due to e.g. freetype error, so return
        }
        m_error = "Unable to produce a layout, try reducing the font size or character ranges.";
        return false;
    }
}

bool Irrtum::drawGrayscaleBitmap()
{
    if (m_graybitmap != 0)
        delete m_graybitmap;

    m_graybitmap = new GrayBitmap(m_layoutwidth, m_layoutheight);

    FT_GlyphSlot slot = m_face->glyph;
    s32 baselineOffset = ceil_26dot6(m_face->size->metrics.ascender);
    s32 maxchar = m_cranges.getMax();
    for (s32 ch = IRRTUM_CHAR_MIN; ch <= maxchar; ++ch)
    {
        if (!m_cranges.contains(ch))
            continue;

        FT_Error error = FT_Load_Char(m_face, (wchar_t) ch, FT_LOAD_RENDER);
        if (error)
        {
            freetypeError(error);
            return false;
        }

        GrayBitmap bmp(&slot->bitmap);
        Rect rect = m_layoutrects[ch - IRRTUM_CHAR_MIN];
        m_graybitmap->setClipRect(rect);
        bmp.blitTo(*m_graybitmap,
                rect.left + slot->bitmap_left,
                rect.top + baselineOffset - slot->bitmap_top);
    }

    m_graybitmap->clearClipRect();
    m_graybitmap->debug();
}

bool Irrtum::getCharBitmapSize(s32 ch, s32& width, s32& height)
{
    if (m_cranges.contains(ch) && ((s32) (wchar_t) ch) == ch)
    {
        FT_Error error = FT_Load_Char(m_face, (wchar_t) ch, FT_LOAD_DEFAULT);
        if (error)
        {
            freetypeError(error);
            return false;
        }
        width = my_max(1, ceil_26dot6(m_face->glyph->metrics.horiAdvance));
        height = my_max(2, ceil_26dot6(m_face->size->metrics.height));
        return true;
    }
    else
    {
        width = 1;
        height = 2;
        return true;
    }
}

bool Irrtum::getTotalBitmapSize(s32& area)
{
    area = 0;
    s32 maxchar = m_cranges.getMax();
    for (s32 ch = IRRTUM_CHAR_MIN; ch <= maxchar; ++ch)
    {
        s32 width, height;
	    if (!getCharBitmapSize(ch, width, height))
		    return false;
        area += width * height;
    }
    return true;
}

bool Irrtum::tryLayout(s32 outwidth, s32 outheight, bool& tooLarge)
{
    assert(outwidth > 0);
    // outheight can be <= 0 though, for automatic sizing

    m_layoutrects.clear();

    s32 x = 2;
    s32 y = 0;
    s32 linestart = IRRTUM_CHAR_MIN;
    s32 lineheight = 1;
    s32 maxy = (outheight > 0 ? outheight : IRRTUM_MAX_AUTOSIZE);

    tooLarge = false;

    s32 maxchar = m_cranges.getMax();
    for (s32 ch = IRRTUM_CHAR_MIN; ch <= maxchar; ++ch)
    {
        s32 width, height;
	    if (!getCharBitmapSize(ch, width, height))
        {
		    return false; // freetype error or similar happened
        }
        if (x + width > outwidth)
        {
            // finish line
            for (s32 i = linestart; i < ch; ++i)
                m_layoutrects[i - IRRTUM_CHAR_MIN].bottom = y + lineheight;
            x = 0;
            y += lineheight;
            linestart = ch;
            lineheight = 1;
        }
        if ((x + width > outwidth) || (y + height > maxy))
        {
            tooLarge = true;
            return false;
        }
        m_layoutrects.push_back(Rect(x, y, x + width, y + height));
        x += width;
        lineheight = my_max(lineheight, height);
    }

    // finish line
    for (s32 i = linestart; i < maxchar; ++i)
        m_layoutrects[i - IRRTUM_CHAR_MIN].bottom = y + lineheight;
    y += lineheight;

    m_layoutwidth = outwidth;
    m_layoutheight = (outheight > 0 ? outheight : y);
    return true;
}

void Irrtum::freetypeError(FT_Error error)
{
    #undef __FTERRORS_H__
    #define FT_ERRORDEF( e, v, s )  { e, s },
    #define FT_ERROR_START_LIST     {
    #define FT_ERROR_END_LIST       { 0, 0 } };

    const struct
    {
        int err_code;
        const char* err_msg;
    } ft_errors[] =
    #include FT_ERRORS_H

    for (int i = 0; i < sizeof(ft_errors) / sizeof(*ft_errors); ++i)
    {
        if (ft_errors[i].err_code == error)
        {
            m_error = ft_errors[i].err_msg;
            return;
        }
    }
    m_error = "unknown FreeType error";
}
