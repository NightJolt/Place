set(PROJECT_NAME PlaceClient)

project(${PROJECT_NAME} CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

include_directories(include)
include_directories(source)

file (
    GLOB_RECURSE SOURCE_FILES

    main.cpp

    include/*.h
    source/*.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} fe2d_core place_common)