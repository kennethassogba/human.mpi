// A simple example using huyman.mpi.
// mpiccx usage.cpp

#include "inc/mpi.hpp"

int main(int argc, char* argv[])
{
  human::mpi::communicator world(argc, argv);

  auto rank = world.rank();
  auto size = world.size();

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
    std::cout.flush();
    // world.send(0, 1, "world");
  }

  return 0;
}