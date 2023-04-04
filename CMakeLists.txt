cmake_minimum_required( VERSION 3.0 )
project(human.mpi)

set(CMAKE_CXX_STANDARD 11)
set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -pedantic -lm -O3 -funroll-loops")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

include_directories(inc)

find_package(MPI)
# include_directories(SYSTEM ${MPI_INCLUDE_PATH})

# message("MPI_INCLUDE_PATH:" ${MPI_INCLUDE_PATH})

add_executable(usage bin/usage.cpp)
#target_link_libraries(usage ${MPI_C_LIBRARIES})
target_link_libraries(usage PUBLIC MPI::MPI_CXX)