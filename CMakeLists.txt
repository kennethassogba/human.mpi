cmake_minimum_required( VERSION 3.4 )
project(human.mpi)

set(CMAKE_CXX_STANDARD 11)
set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-g -fsanitize=address")

include_directories(include)
# target_include_directories(usage PUBLIC include)

# OpenMP
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    # target_link_libraries(usage PUBLIC OpenMP::OpenMP_CXX)
    set(LINK_LIBS ${LINK_LIBS} ${OpenMP_CXX_LIBRARIES})
endif()

# MPI
find_package(MPI)
# target_link_libraries(usage PUBLIC MPI::MPI_CXX)
set(LINK_LIBS ${LINK_LIBS} ${MPI_CXX_LIBRARIES})


# Executables

add_executable(usage bin/usage.cpp)
target_link_libraries(usage PUBLIC ${LINK_LIBS})


add_executable(broadcast bin/broadcast.cpp)
target_link_libraries(broadcast PUBLIC ${LINK_LIBS})
