#ifndef HUMAN_COMMUNICATOR_HPP
#define HUMAN_COMMUNICATOR_HPP

#if defined(OMPI_MPICXX) || defined(I_MPI_CXX) // MPICH
#define USE_HUMAN_MPI
#endif

#define USE_HUMAN_MPI
#define DEBUG_HUMAN_MPI

#include <string>
#include <vector>
#include <mpi.h>

#include "timer.hpp"
#include "datatype.hpp"

namespace human
{
  namespace mpi
  {

    const int any_source = MPI_ANY_SOURCE;
    const int any_tag = MPI_ANY_TAG;

    class communicator
    {
    public:
      communicator()
      {
        init(nullptr, nullptr);
      }

      communicator(int &argc, char **argv)
      {
        init(&argc, argv);
      }

#ifdef USE_HUMAN_MPI
      communicator(const MPI_Comm &mpi_comm)
      {
        mpi_comm_ = mpi_comm;
        MPI_Comm_rank(mpi_comm_, &rank_);
        MPI_Comm_size(mpi_comm_, &size_);
        root_ = 0;
      }
#endif

      ~communicator()
      {
        MPI_Finalize();
      }

      void init(int *argc, char **argv)
      {
#ifdef USE_HUMAN_MPI
        int level = MPI_THREAD_MULTIPLE;
        MPI_Init_thread(argc, &argv, level, &level);
        mpi_comm_ = MPI_COMM_WORLD;
        MPI_Comm_rank(mpi_comm_, &rank_);
        MPI_Comm_size(mpi_comm_, &size_);
        root_ = 0;
#else
        (void)argc;
        (void)argv;
#endif
      }

      // ----------------------------------------------------------------
      // Setters
      // ----------------------------------------------------------------
      void set_root(int root) { root_ = root; }

      // ----------------------------------------------------------------
      // Getters
      // ----------------------------------------------------------------
      int rank() const { return rank_; }
      int size() const { return size_; }
      int root() const { return root_; }

      // ----------------------------------------------------------------
      // Point-to-point communication
      // ----------------------------------------------------------------
      template <class Type>
      void send(Type *buffer, int count, int receiver, int tag);

      void send(std::string &buffer, int receiver, int tag);

      template <class Type>
      void recv(Type *buffer, int count, int sender, int tag);

      void recv(std::string &buffer, int sender, int tag);

      template <class Type>
      void isend(Type *buffer, int count, int receiver, int tag, MPI_Request &request);

      template <class Type>
      void irecv(Type *buffer, int count, int sender, int tag, MPI_Request &request);

      // ----------------------------------------------------------------
      // Collective communication
      // ----------------------------------------------------------------
      template <class Type>
      void bcast(Type &buffer, int count);

      template <class Type>
      void bcast(Type *buffer, int count);

      template <class Type>
      void bcast(std::vector<Type> &buffer);

      void bcast(std::string &buffer);

      template <class Type>
      void allreduce_sum(Type &value);

      template <class Type>
      void iallreduce_sum(Type &value, MPI_Request &request);

      template <class Type>
      void gather(const Type *buffer_send, int count_send, Type *buffer_recv, int count_recv);

      template <class Type>
      void gatherv(const Type *buffer_send, int count_send, Type *buffer_recv, const int *counts_recv, const int *displacements);

      void wait(MPI_Request &request);

      void waitall(int count, MPI_Request requests[]);

      void barrier();

      // ----------------------------------------------------------------
      // Time
      // ----------------------------------------------------------------
      void time(const std::string &event) { time_.update(event); }
      void display() { time_.display(); }

    private:
      void check(int ier, std::string message = "");

      // ----------------------------------------------------------------
      // Attributes
      // ----------------------------------------------------------------

      int rank_;
      int size_;
      int root_;

      Timer time_;

#ifdef USE_HUMAN_MPI
      MPI_Comm mpi_comm_;
#endif
    };
  } // mpi
} // human

// ----------------------------------------------------------------
// Send / Isend
// ----------------------------------------------------------------

template <class Type>
void human::mpi::communicator::send(Type *buffer, int count, int receiver, int tag)
{
#ifdef USE_HUMAN_MPI
  if (size_ <= 1)
    return;

  MPI_Datatype datatype = gettype<Type>();

  time_.update("mpi::Send");

  int ier = MPI_Send(buffer, count, datatype, receiver, tag, mpi_comm_);
  check(ier);

  time_.update("mpi::Send");
#else
  (void)buffer;
  (void)count;
  (void)receiver;
  (void)tag;
#endif
}

void human::mpi::communicator::send(std::string &buffer, int receiver, int tag)
{
#ifdef USE_HUMAN_MPI
  if (size_ <= 1)
    return;

  int data_size = buffer.size();

  send<int>(&data_size, 1, receiver, tag + 1);

  send<char>(const_cast<char *>(buffer.c_str()), data_size, receiver, tag);

#else
  (void)buffer;
  (void)receiver;
  (void)tag;
#endif
}

template <class Type>
void human::mpi::communicator::isend(Type *buffer, int count, int receiver, int tag, MPI_Request &request)
{
#ifdef USE_HUMAN_MPI
  if (size_ <= 1)
    return;

  MPI_Datatype datatype = gettype<Type>();

  time_.update("mpi::ISend");

  int ier = MPI_Isend((void *)buffer, count, datatype, receiver, tag, mpi_comm_, &request);
  check(ier);

  time_.update("mpi::ISend");
#else
  (void)buffer;
  (void)count;
  (void)receiver;
  (void)tag;
  (void)request;
#endif
}

// ----------------------------------------------------------------
// Recv / Irecv
// ----------------------------------------------------------------

template <class Type>
void human::mpi::communicator::recv(Type *buffer, int count, int sender, int tag)
{
#ifdef USE_HUMAN_MPI
  if (size_ <= 1)
    return;

  MPI_Datatype datatype = gettype<Type>();

  time_.update("mpi::Recv");

  int ier = MPI_Recv(buffer, count, datatype, sender, tag, mpi_comm_, MPI_STATUS_IGNORE);
  check(ier);

  time_.update("mpi::Recv");
#else
  (void)buffer;
  (void)count;
  (void)sender;
  (void)tag;
#endif
}

void human::mpi::communicator::recv(std::string &buffer, int sender, int tag)
{
#ifdef USE_HUMAN_MPI
  if (size_ <= 1)
    return;

  int data_size(0);

  recv<int>(&data_size, 1, sender, tag + 1);

  buffer.resize(data_size);

  recv<char>(const_cast<char *>(buffer.c_str()), data_size, sender, tag);
#else
  (void)buffer;
  (void)sender;
  (void)tag;
#endif
}

template <class Type>
void human::mpi::communicator::irecv(Type *buffer, int count, int sender, int tag, MPI_Request &request)
{
#ifdef USE_HUMAN_MPI
  if (size_ <= 1)
    return;

  MPI_Datatype datatype = gettype<Type>();

  time_.update("mpi::IRecv");

  int ier = MPI_Irecv((void *)buffer, count, datatype, sender, tag, mpi_comm_, &request);
  check(ier);

  time_.update("mpi::IRecv");
#else
  (void)buffer;
  (void)count;
  (void)sender;
  (void)tag;
  (void)request;
#endif
}

// ----------------------------------------------------------------
// Allreduce
// ----------------------------------------------------------------
template <class Type>
void human::mpi::communicator::allreduce_sum(Type &value)
{
#ifdef USE_HUMAN_MPI
  if (size_ <= 1)
    return;

  MPI_Datatype datatype = gettype<Type>();
  Type tmp_send = value;

  time_.update("mpi::Allreduce_Sum");

  int ier = MPI_Allreduce(&tmp_send, &value, 1, datatype, MPI_SUM, mpi_comm_);
  check(ier);

  time_.update("mpi::Allreduce_Sum");
#else
  (void)value;
#endif
}

template <class Type>
void human::mpi::communicator::iallreduce_sum(Type &value, MPI_Request &request)
{
#if defined(USE_HUMAN_MPI)
  if (size_ <= 1)
    return;

  MPI_Datatype datatype = gettype<Type>();
  Type tmp_send = value;

  time_.update("mpi::Iallreduce_Sum");

  int ier = MPI_Iallreduce(&tmp_send, &value, 1, datatype, MPI_SUM, mpi_comm_, &request);

  check(ier);

  time_.update("mpi::Iallreduce_Sum");
#else
  (void)value;
  (void)request;
#endif
}

// ----------------------------------------------------------------
// Bcast
// ----------------------------------------------------------------
template <class Type>
void human::mpi::communicator::bcast(Type &buffer, int count)
{
#ifdef USE_HUMAN_MPI
  if (size_ <= 1)
    return;

  MPI_Datatype datatype = gettype<Type>();

  time_.update("mpi::Bcast");

  int ier = MPI_Bcast(&buffer, count, datatype, root_, mpi_comm_);

  check(ier);
  time_.update("mpi::Bcast");

#else
  (void)buffer;
  (void)count;
#endif
}

template <class Type>
void human::mpi::communicator::bcast(Type *buffer, int count)
{
#ifdef USE_HUMAN_MPI
  if (size_ <= 1)
    return;

  MPI_Datatype datatype = gettype<Type>();

  time_.update("mpi::Bcast");

  int ier = MPI_Bcast((void *)buffer, count, datatype, root_, mpi_comm_);

  check(ier);
  time_.update("mpi::Bcast");

#else
  (void)buffer;
  (void)count;
#endif
}

template <class Type>
void human::mpi::communicator::bcast(std::vector<Type> &buffer)
{
#ifdef USE_HUMAN_MPI
  if (size_ <= 1)
    return;

  int count = buffer.size();
  bcast<int>(count, 1);

  if (rank_ != root_)
    buffer.resize(count);

  bcast<Type>(buffer.data(), count);

#else
  (void)buffer;
  (void)count;
#endif
}

// ----------------------------------------------------------------
// Gather
// ----------------------------------------------------------------
template <class Type>
void human::mpi::communicator::gather(const Type *buffer_send,
                                      int count_send,
                                      Type *buffer_recv,
                                      int count_recv)
{
#ifdef USE_HUMAN_MPI
  if (size_ <= 1)
    return;

  MPI_Datatype datatype = gettype<Type>();

  time_.update("mpi::Gather");

  int ier = MPI_Gather((void *)buffer_send, count_send, datatype, (void *)buffer_recv, count_recv, datatype, root_, mpi_comm_);

  check(ier);
  time_.update("mpi::Gather");
#else
  (void)buffer;
  (void)count;
#endif
}

template <class Type>
void human::mpi::communicator::gatherv(const Type *buffer_send,
                                       int count_send,
                                       Type *buffer_recv,
                                       const int *counts_recv,
                                       const int *displacements)
{
#ifdef USE_HUMAN_MPI

  if (size_ <= 1)
    return;

  MPI_Datatype datatype = gettype<Type>();

  time_.update("mpi::Gather");

  int ier = MPI_Gatherv((void *)buffer_send, count_send, datatype, (void *)buffer_recv, counts_recv, displacements, datatype, root_, mpi_comm_);

  check(ier);
  time_.update("mpi::Gather");

#else
  (void)buffer;
  (void)count;
#endif
}

void human::mpi::communicator::bcast(std::string &buffer)
{
#ifdef USE_HUMAN_MPI
  if (size_ <= 1)
    return;

  int data_size = buffer.size();

  bcast<int>(data_size, 1);

  if (rank_ != root_)
    buffer.resize(data_size);

  bcast<char>(const_cast<char *>(buffer.c_str()), data_size);

#else
  (void)buffer;
#endif
}

// ----------------------------------------------------------------
// Wait / Waitall
// ----------------------------------------------------------------

void human::mpi::communicator::wait(MPI_Request &request)
{
#ifdef USE_HUMAN_MPI
  if (size_ <= 1)
    return;

  time_.update("Comm::Wait");

  int ier = MPI_Wait(&request, MPI_STATUS_IGNORE);
  check(ier);

  time_.update("Comm::Wait");
#else
  (void)request;
#endif
}

void human::mpi::communicator::waitall(int count, MPI_Request requests[])
{
#ifdef USE_HUMAN_MPI
  if (size_ <= 1)
    return;

  time_.update("Comm::Waitall");

  int ier = MPI_Waitall(count, requests, MPI_STATUSES_IGNORE);
  check(ier);

  time_.update("Comm::Waitall");
#else
  (void)count;
  (void)requests;
#endif
}

void human::mpi::communicator::barrier()
{
#ifdef USE_HUMAN_MPI
  if (size_ <= 1)
    return;
  time_.update("Comm::Barrier");

  int ier = MPI_Barrier(mpi_comm_);
  check(ier);

  time_.update("Comm::Barrier");
#endif
}

// ----------------------------------------------------------------
// Check error
// ----------------------------------------------------------------

/**
 * @brief Retrieve the message corresponding to an error code
 *
 * @param ierr
 * @param message
 */
void human::mpi::communicator::check(int ierr, std::string message)
{
#ifdef USE_HUMAN_MPI

#ifdef DEBUG_HUMAN_MPI
  if (ierr != MPI_SUCCESS)
  {

    char err_str[MPI_MAX_ERROR_STRING];
    int err_str_len = 0;
    MPI_Error_string(ierr, err_str, &err_str_len);
    std::string err_msg(err_str, err_str_len);

    std::cerr << "<human::mpi::communicator::check> rank: "
              << rank_ << "/" << size_
              << "\nMessage: " << message
              << "\nMPI Error Message: " << err_msg
              << std::endl;
    throw;
  }
#endif

#else
  (void)ierr;
  (void)message;
#endif
}

#endif
