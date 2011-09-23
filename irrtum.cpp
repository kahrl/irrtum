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

Irrtum::Irrtum():
    m_error("No error"),
    m_ftlibrary(0),
    m_ftinited(false),
    m_face(0),
    m_cranges()
{
}

Irrtum::~Irrtum()
{
    if (m_ftinited)
    {
        FT_Done_FreeType(m_ftlibrary);  // ignoring errors
        m_ftinited = false;
    }
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


    FT_GlyphSlot slot = m_face->glyph;
    wchar_t text[] = L"Hello world";
    int num_chars = 11;
    for (int n = 0; n < num_chars; ++n)
    {
        error = FT_Load_Char(m_face, text[n], FT_LOAD_RENDER);
        if (error)
        {
            freetypeError(error);
            return false;
        }

        FT_Bitmap* bmp = &slot->bitmap;
        for (int y = 0; y < bmp->rows; ++y)
        {
            unsigned char* scanline = bmp->buffer + y * bmp->pitch;
            for (int x = 0; x < bmp->width; ++x)
            {
                int color = scanline[x];
                if (color > 255*3/4)
                    cout << "#";
                else if (color > 255*1/2)
                    cout << "+";
                else if (color > 255*1/4)
                    cout << ".";
                else
                    cout << " ";
            }
            cout << endl;
        }
    }

    return true;
}

void Irrtum::setCharacterRanges(const IntervalList& cranges)
{
    m_cranges = cranges;
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
