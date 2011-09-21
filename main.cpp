#include <iostream>
#include <string>

// png.h must be included before freetype headers, else we get setjmp.h conflicts
#include <png.h>
#include <ft2build.h>
#include FT_FREETYPE_H

using std::cout;
using std::cerr;
using std::endl;
using std::string;


int main(int argc, char *argv[])
{
	png_uint_32 libpng_vn = png_access_version_number();
	cout << "libpng version: " << (libpng_vn / 10000) << "." << ((libpng_vn / 100) % 100) << "." << (libpng_vn % 100) << endl;

	FT_Library ft_library;
	FT_Error ft_error;

	ft_error = FT_Init_FreeType(&ft_library);
	if (ft_error)
	{
		cerr << "An error occurred while initializing Freetype." << endl;
		return 1;
	}

	int ft_vn_major = -1, ft_vn_minor = -1, ft_vn_patch = -1;
	FT_Library_Version(ft_library, &ft_vn_major, &ft_vn_minor, &ft_vn_patch);
	cout << "freetype version: " << ft_vn_major << "." << ft_vn_minor << "." << ft_vn_patch << endl;

	return 0;
}
