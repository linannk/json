cmake_minimum_required(VERSION 3.0)
project(json C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED 1)
set(CMAKE_CXX_STANDARD_REQUIRED 1)

file(GLOB_RECURSE Json_Src src/*.cpp)
file(GLOB_RECURSE Json_Header inc/*.h)

file(GLOB inc_list inc/*.h)
file(GLOB inc_io_list inc/io/*.h)
file(GLOB inc_charset_list inc/charset/*.h)
file(GLOB inc_util_list inc/util/*.h)

file(GLOB src_list src/*.cpp)
file(GLOB src_io_list src/io/*.cpp)
file(GLOB src_charset_list src/charset/*.cpp)
file(GLOB src_util_list src/util/*.cpp)

source_group(inc FILES ${inc_list})
source_group(inc\\io FILES ${inc_io_list})
source_group(inc\\charset FILES ${inc_charset_list})
source_group(inc\\util FILES ${inc_util_list})

source_group(src FILES ${src_list})
source_group(src\\io FILES ${src_io_list})
source_group(src\\charset FILES ${src_charset_list})
source_group(src\\util FILES ${src_util_list})

include_directories(inc)

if (MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif ()

add_library(json STATIC ${Json_Src} ${Json_Header})
add_subdirectory(unittest)
