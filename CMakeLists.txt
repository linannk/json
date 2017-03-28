cmake_minimum_required(VERSION 2.8)

project(json C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

aux_source_directory(src SRC_LIST)
aux_source_directory(inc HEADER_LIST)

include_directories(inc)
add_library(json STATIC ${SRC_LIST} ${HEADER_LIST})

