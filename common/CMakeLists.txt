set(LIB_NAME place_common)

project(${LIB_NAME} CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

include_directories(include)
include_directories(source)

file (
    GLOB_RECURSE SOURCE_FILES

    include/*.h
    source/*.cpp
)

add_library(${LIB_NAME} ${SOURCE_FILES})

target_link_libraries(${LIB_NAME} fe2d_core)