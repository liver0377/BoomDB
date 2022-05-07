cmake_minimum_required(VERSION 3.10)

project(
    BoomDB 
    VERSION 1.0
    DESCRIPTION "An simple kv database"
    LANGUAGES CXX
    )


set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

aux_source_directory("${PROJECT_SOURCE_DIR}/src" SRC)

SET(BoomDB_INCLUDE ${PROJECT_SOURCE_DIR}/include)
include_directories(${BoomDB_INCLUDE})

add_subdirectory(extern)
add_subdirectory(src)
# add_subdirectory(apps)

# if(BUILD_TESTING)
  add_subdirectory(test)
# endif()


