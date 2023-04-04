cmake_minimum_required( VERSION 2.8 )
project(human.mpi)

find_package(MPI)

add_executable(usage usage.cpp)