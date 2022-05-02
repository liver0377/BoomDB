# include_directories(./include)

aux_source_directory(${BoomDB_SOURCE_DIR}/include/ HEADER_LIST)
# set(HEADER_LIST ${BoomDB_SOURCE_DIR}/include/BoomDB.h)

# 生成库，这里会根据用户设置自行设置为静态库或者动态库
add_library(BoomDB_library BoomDB.cpp ${HEADER_LIST})

target_include_directories(BoomDB_library PUBLIC ../include)

target_compile_features(BoomDB_library PUBLIC cxx_std_11)


