cmake_minimum_required(VERSION 3.5)
project(flann_v2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp flann/general.h flann/defines.h flann/util/matrix.h flann/util/dist.h flann/util/lsh_table.h flann/util/lsh_index.h flann/util/result_set.h flann/util/dynamic_bitset.h flann/util/random.h flann/matlab/buildIndexAndSave.cpp flann/matlab/loadIndexAndQuery.cpp)
add_executable(flann ${SOURCE_FILES})