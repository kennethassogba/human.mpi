# Human.MPI 

A vanilla header-only object oriented interface around MPI in C++

## Build
```bash
mkdir build; cd build
cmake ..
make -j
./usage
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
