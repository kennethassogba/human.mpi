// A simple example using human.mpi
// Use the cmake(readme) or
// mpiccx usage.cpp -o usage
// mpirun -n 2 usage

#include "../inc/mpi.hpp"

int main(int argc, char *argv[])
{
  human::mpi::communicator world(argc, argv);

  auto rank = world.rank();
  auto size = world.size();

  std::cout << "Process " << rank << "/" << size << std::endl;

  if (world.rank() == 0)
  {
    // world.send(1, 0, "Hello");
    std::string msg;
    // world.recv(1, 1, msg);
    std::cout << msg << "!" << std::endl;
  }
  else
  {
    std::string msg;
    // world.recv(0, 0, msg);
    std::cout << msg << ", ";
    // world.send(0, 1, "world");
  }

  return 0;
}