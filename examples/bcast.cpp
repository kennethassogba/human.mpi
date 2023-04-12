// A simple example using human.mpi
// mpicxx this.cpp -o this
// mpirun -n 4 this
#include <iostream>

#include "human/mpi.hpp"

int main(int argc, char *argv[])
{
  human::mpi::communicator world(argc, argv);

  auto rank = world.rank();
  auto size = world.size();

  std::cout << "Process " << rank << "/" << size << std::endl;

  std::string msg;
  if (world.rank() == world.root())
    msg = "Hello";

  world.bcast(msg);

  std::cout << "P" << rank << " " << msg << std::endl;

  return 0;
}