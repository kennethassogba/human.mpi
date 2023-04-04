cmake_minimum_required( VERSION 2.8 )
project(human.mpi)

include_directories(inc)

find_package(MPI)

add_executable(usage bin/usage.cpp)