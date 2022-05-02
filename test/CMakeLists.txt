add_executable(testlib db_test.cpp)

target_compile_features(testlib PRIVATE cxx_std_14)

# 链接gtest, BoomDB
target_link_libraries(testlib PRIVATE BoomDB_library gtest_main pthread)

add_test(NAME testlibtest COMMAND testlib)

