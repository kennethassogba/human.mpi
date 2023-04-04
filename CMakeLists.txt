cmake_minimum_required( VERSION 2.8 )
project(human.mpi)

include_directories(inc)

find_package(MPI)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

add_executable(usage bin/usage.cpp)
target_link_libraries(usage ${MPI_C_LIBRARIES})