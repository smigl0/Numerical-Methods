cmake_minimum_required(VERSION 3.25)
project(project)

set(BLA_VENDOR OpenBLAS)
find_package(BLAS REQUIRED)
find_package(LAPACK REQUIRED)
add_executable(main src/main.cpp)
target_link_libraries(main BLAS::BLAS LAPACK::LAPACK -llapacke)
