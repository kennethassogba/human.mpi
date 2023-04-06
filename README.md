# Human.MPI 

A vanilla header-only object oriented interface around MPI in C++.

## Usage
```c++
#include "human/mpi.hpp"
// ...
human::mpi::communicator world();

auto rank = world.rank();
auto size = world.size();
std::cout << "Process " << rank << "/" << size << std::endl;

std::string msg;
if (world.rank() == world.root()) msg = "Hello";

world.bcast(msg);

std::cout << "P" << rank << " " << msg << std::endl;

world.display(); // display time table
```

## Build an example
```bash
mkdir build && cd build
cmake ..
make -j
```
or
```bash
cmake -B build
cmake --build build
```
Then try 
```bash
mpiexec -n 2 build/usage
```

## Features
- Send `vector` and `string` with automatic resize
- Time events and display time table
- Not exhaustive but simple to extend

## Macro
- -DUSE_HUMAN_MPI use the lib
- -DHUMAN_MPI_TIMER_USE_MPI=false use chrono
- -DHUMAN_MPI_TIMER_FOR_REAL=false disable timing

## Roadmap
- Add mpi request in human::mpi
- Write tests using ctest or catch2
- Add async experiments
- Add multiple executable target in Cmake
- Configure Github action
- Simple Doc

## Write
- Developping an interface around MPI


Inspired by Boost.MPI
