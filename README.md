# Human.MPI 

A vanilla header-only object oriented interface around MPI in C++

## Build
```bash
mkdir build; cd build
cmake ..
make -j
./usage
```

## Usage
```c++
#include "human.mpi/mpi.hpp"
// ...
human::mpi::communicator world(argc, argv);

auto rank = world.rank();
auto size = world.size();
std::cout << "Process " << rank << "/" << size << std::endl;

std::string msg;
if (world.rank() == world.root()) msg = "Hello";

bcast(msg);

std::cout << "P" << rank << " " << msg << std::endl;

world.display(); // display time table
```

## Macro
- -DUSE_HUMAN_MPI use the lib
- -DHUMAN_MPI_TIMER_USE_MPI=false use chrono
- -DHUMAN_MPI_TIMER_FOR_REAL=false disable timing

## Roadmap
- Write tests
- Cmake Ninja
- Github action for ci cd
- Simple Doc
- Write docker file

## Write
- Developping a wrapper around MPI
- What I learn will writing


Inspired by Boost.MPI
