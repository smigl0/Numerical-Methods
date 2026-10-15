cmake_minimum_required(VERSION 3.28.3)
project(proj01)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")

set(BLA_VENDOR OpenBLAS)
find_package(BLAS REQUIRED)

add_executable(task1.1.out)
target_sources(task1.1.out PRIVATE src/main1.1.cpp)
target_link_libraries(task1.1.out PRIVATE m)

add_executable(task1.2.out)
target_sources(task1.2.out PRIVATE src/main1.2.cpp)
target_link_libraries(task1.1.out PRIVATE m)

add_executable(task1.3.out)
target_sources(task1.3.out PRIVATE src/main1.3.cpp)
target_link_libraries(task1.3.out PRIVATE m BLAS::BLAS)

