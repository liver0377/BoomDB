# 找到src/*/下的所有cpp文件
file(GLOB_RECURSE BoomDB_sources ${PROJECT_SOURCE_DIR}/src/*/*.cpp)

# 生成BoomDB_library 共享库
add_library(BoomDB_library SHARED ${BoomDB_sources})

