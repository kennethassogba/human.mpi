cmake_minimum_required( VERSION 3.4 )
project(human.mpi)

set(CMAKE_CXX_STANDARD 11)
set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -funroll-loops -funroll-all-loops -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-g")

include_directories(include)

# OpenMP
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    set(LINK_LIBS ${LINK_LIBS} ${OpenMP_CXX_LIBRARIES})
endif()

# MPI
find_package(MPI)
set(LINK_LIBS ${LINK_LIBS} MPI::MPI_CXX)


# Executables

add_executable(send examples/send.cpp)
target_link_libraries(send PUBLIC ${LINK_LIBS})

add_executable(bcast examples/bcast.cpp)
target_link_libraries(bcast PUBLIC ${LINK_LIBS})
