# the name of the target operating system
set(CMAKE_SYSTEM_NAME arm)

# which compilers to use for C and C++
set(CMAKE_C_COMPILER   /usr/bin/aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER /usr/bin/aarch64-linux-gnu-g++)

# where is the target environment located
set(CMAKE_FIND_ROOT_PATH  /usr/i586-mingw32msvc
    /home/alex/mingw-install)

# adjust the default behavior of the FIND_XXX() commands:
# search programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


########################################################################
# Project setup for LMS7 driver
#
# This project builds the sources into a static library
# and had install rules for the library and headers
########################################################################
cmake_minimum_required(VERSION 2.8)
project(LMS7002MDriver C)

########################################################################
# Compiler setup
########################################################################
if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
    add_definitions(-D_GNU_SOURCE)
    add_definitions(-Wall)
    add_definitions(-Wextra)
    add_definitions(-pedantic)
endif()

########################################################################
# Build library
# Defaults to static, set BUILD_SHARED_LIBS=ON for shared
########################################################################
include_directories(include)
file(GLOB LMS7002M_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/LMS7002M_*.c")
add_library(LMS7002M ${LMS7002M_SOURCES})
install(TARGETS LMS7002M DESTINATION lib${LIB_SUFFIX})
set_property(TARGET LMS7002M PROPERTY POSITION_INDEPENDENT_CODE TRUE)

########################################################################
# install headers
########################################################################
install(DIRECTORY include/LMS7002M DESTINATION include)
