cmake_minimum_required(VERSION 3.15)

project(BoomDB VERSION 1.0
        PROJECT_DESCRIPTION "a simple kv database"
        LANGUAGE CXX)

set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

aux_source_directory("${PROJECT_SOURCE_DIR}/src" SRC)

add_subdirectory(include)
add_subdirectory(src)

# 添加搜索路径
include_directories(include)
include_directories(${PROJECT_SOURCE_DIR}/src/include)

# add_executable(BoomDB ${SRC})

enable_testing()

add_executable(BoomDB_test "test/db_test.cc" ${SRC})
target_link_libraries(BoomDB_test gtest_main)

