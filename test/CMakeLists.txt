aux_source_directory(. TEST_LIST)

add_executable(testlib ${TEST_LIST})

# include 目录
target_include_directories(testlib PRIVATE ../include)

# 链接gtest, BoomDB
target_link_libraries(testlib PRIVATE BoomDB_library gtest_main pthread)



