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
#include "cmake_config.h"
#include <popt.h>

static int version(Irrtum& irrtum)
{
    cout << "irrtum " << VERSION_STRING << endl;
    bool good = true;

    if (irrtum.initLibpng())
    {
        cout << "libpng version: " << irrtum.getLibpngVersion() << endl;
    }
    else
    {
        cerr << "Unable to initialize libpng: " << irrtum.getLastError() << endl;
        good = false;
    }

    if (irrtum.initFreetype())
    {
        cout << "freetype version: " << irrtum.getFreetypeVersion() << endl;
    }
    else
    {
        cerr << "Unable to initialize freetype: " << irrtum.getLastError() << endl;
        good = false;
    }

    return (good ? 0 : 1);
}

int main(int argc, char *argv[])
{
    Irrtum irrtum;

    float opt_size = 16;
    float opt_dpi = 72;
    int opt_outwidth = 0;

    struct poptOption poptopts[] = {
        {"size", 's', POPT_ARG_FLOAT, &opt_size, 0, "Set font size in points", "POINTS"},
        {"dpi", 'd', POPT_ARG_FLOAT, &opt_dpi, 0, "Set DPI value", "DPI"},
        {"outwidth", 'w', POPT_ARG_INT, &opt_outwidth, 0, "Set width of output image", "PIXELS"},
        {"range", 'r', POPT_ARG_STRING, 0, 'r', "Add character range", "START-END"},
        {"version", 'V', 0, 0, 'V', "Display version number and exit", 0},
        POPT_AUTOHELP
        {0, 0, 0, 0, 0, 0, 0}
    };

    poptContext poptcon = poptGetContext("irrtum",
            argc, const_cast<const char**>(argv), poptopts, 0);
    int rc;
    while ((rc = poptGetNextOpt(poptcon)) > 0)
    {
        if (rc == 'r')
        {
            cout << "range: " << poptGetOptArg(poptcon) << endl;
        }
        else if (rc == 'V')
        {
            return version(irrtum);
        }
    }
    if (rc < -1) {
        /* an error occurred during option processing */
        cerr << poptBadOption(poptcon, POPT_BADOPTION_NOALIAS) << ": "
            << poptStrerror(rc) << endl;
        return 1;
    }

    cout << "size: " << opt_size << endl;
    cout << "dpi: " << opt_dpi << endl;
    cout << "outwidth: " << opt_outwidth << endl;

    if (!irrtum.initLibpng())
    {
        cerr << "Unable to initialize libpng: " << irrtum.getLastError() << endl;
        return 1;
    }

    if (!irrtum.initFreetype())
    {
        cerr << "Unable to initialize freetype: " << irrtum.getLastError() << endl;
        return 1;
    }

    const char* filename = poptGetArg(poptcon);
    if (!filename)
    {
        cerr << "irrtum: no input files" << endl;
        return 1;
    }
    while (filename)
    {
        if (!irrtum.loadFace(filename, opt_size, opt_dpi))
        {
            cerr << filename << ": Unable to load face: " << irrtum.getLastError() << endl;
            return 1;
        }
        filename = poptGetArg(poptcon);
    }

    poptFreeContext(poptcon);
    return 0;
}
