// A simple example using human.mpi
// Use the cmake(readme) or
// mpiccx this.cpp -o this
// mpirun -n 2 this
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

  bcast(msg);

  std::cout << "P" << rank << " " << msg << std::endl;

  world.display();

  return 0;
}