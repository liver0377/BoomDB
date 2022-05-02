cmake_minimum_required(VERSION 3.15..3.21)

project(
    BoomDB 
    VERSION 1.0
    DESCRIPTION "An simple kv database"
    LANGUAGES CXX
    )


set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

aux_source_directory("${PROJECT_SOURCE_DIR}/src" SRC)

add_subdirectory(extern)
add_subdirectory(src)
add_subdirectory(apps)

# if(BUILD_TESTING)
  add_subdirectory(test)
# endif()

# include(FetchContent)
# 添加搜索路径
# include_directories(extern)

# add_executable(BoomDB_test "test/db_test.cc" ${SRC})
# target_include_directories(BoomDB_test PUBLIC include)
# target_link_libraries(BoomDB_test gtest_main)  # 链接gtest_main.a 不需要手动写

