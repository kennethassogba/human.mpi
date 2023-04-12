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
mpiexec -n 2 build/send
mpiexec -n 4 build/bcast
```

## Features

- Send `vector` and `string` with automatic resize
- `MPI` error checking
- Not exhaustive but simple to extend

## Macro

- -DCMAKE_BUILD_TYPE=Debug
- -DUSE_HUMAN_MPI use the lib

## Testsuite

The testsuite is run on push and pull request using GitHub Actions

## Roadmap

- Add mpi request in human::mpi
- Write tests using ctest + gtest or catch2
- Add async experiments
- Add multiple executable target function in Cmake
- Simple Doc

## Write

- Developping an interface around MPI

Inspired by Boost.MPI
