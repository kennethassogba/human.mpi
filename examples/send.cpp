// A simple example using human.mpi
// mpicxx this.cpp -o this
// mpirun -n 2 this
#include <iostream>
#include <cassert>

#include "human/mpi.hpp"

int main(int argc, char *argv[])
{
  human::mpi::communicator world(argc, argv);

  auto rank = world.rank();
  auto size = world.size();

  std::cout << "Process " << rank << "/" << size << std::endl;

  assert(size == 2 && "Designed for 2 process, relauch with mpirun -n 2");

  std::string msg_sent, msg_recv;
  int other;

  if (world.rank() == world.root())
  {
    msg_sent = "Hello";
    other = 1;
  }
  else
  {
    msg_sent = "world!";
    other = 0;
  }

  auto tag = 1;

  world.send(msg_sent, other, tag);
  world.recv(msg_recv, other, tag);

  std::cout << "P" << rank << " " << msg_sent << " " << msg_recv << std::endl;

  return 0;
}