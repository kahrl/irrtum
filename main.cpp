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
#include <popt.h>

int main(int argc, char *argv[])
{
	Irrtum irrtum;

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

	cout << "libpng version: " << irrtum.getLibpngVersion() << endl;
	cout << "freetype version: " << irrtum.getFreetypeVersion() << endl;

    std::string filename = "DejaVuSansMono.ttf";
    if (!irrtum.loadFace(filename))
    {
        cerr << filename << ": Unable to load face: " << irrtum.getLastError() << endl;
    }

	return 0;
}
