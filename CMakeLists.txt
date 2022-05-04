cmake_minimum_required(VERSION 3.22)
project(ABF)

set(CMAKE_CXX_STANDARD 14)

set(INC_DIR D:\\msys64\\usr\\local\\include)
set(LINK_DIR D:\\msys64\\usr\\local\\lib64)
include_directories(${INC_DIR})
link_directories(${LINK_DIR})
link_libraries(ssl crypto)

add_executable(ABF main.cpp AdaptiveBloomFilter.cpp AdaptiveBloomFilter.h SlowMemoryRepresentation.cpp SlowMemoryRepresentation.h HashFunctionSHA512.cpp HashFunctionSHA512.h Dictionary.h macros.h Dictionary.cpp)
