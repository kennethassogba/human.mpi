cmake_minimum_required( VERSION 3.0 )
project(human.mpi)

set(CMAKE_CXX_STANDARD 11)
set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -pedantic -lm -O3 -funroll-loops")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

include_directories(include)

add_executable(usage bin/usage.cpp)
add_executable(broadcast bin/broadcast.cpp)

# OpenMP
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(usage PUBLIC OpenMP::OpenMP_CXX)
endif()

# MPI
find_package(MPI)
target_link_libraries(usage PUBLIC MPI::MPI_CXX)