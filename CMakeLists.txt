cmake_minimum_required(VERSION 2.6)
project(irrtum)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_PATCH 1)
set(VERSION_STRING "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

if(NOT CMAKE_BUILD_TYPE)
	# Default to release
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type: Debug or Release" FORCE)
endif()

find_package(PNG REQUIRED)
find_package(ZLIB REQUIRED)
find_package(Freetype REQUIRED)

include_directories(
	${PROJECT_BINARY_DIR}  # CMake's config.h would be stored here
	${ZLIB_INCLUDE_DIR}
	${PNG_INCLUDE_DIR}
	${FREETYPE_INCLUDE_DIRS}
)

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin")

add_executable(${PROJECT_NAME}
	main.cpp
)

target_link_libraries(${PROJECT_NAME}
	${ZLIB_LIBRARIES}
	${PNG_LIBRARIES}
	${FREETYPE_LIBRARIES}
)

# TODO: Windows support.
# TODO: Installation.

