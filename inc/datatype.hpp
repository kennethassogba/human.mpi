#ifndef HUMAN_MPI_DATATYPE_HPP
#define HUMAN_MPI_DATATYPE_HPP

#ifdef USE_HUMAN_MPI


/****************************************************************************
 * @brief 
 * (i) Safe way to determine MPI_Datatype corresponding to size_t
 * use my_MPI_SIZE_T as data type every time you want to transfer 
 * data of type size_t
 * 
 * (ii) Determine MPI_Datatype associate to a C++ type
 * 
 * @ref https://stackoverflow.com/questions/40807833/sending-size-t-type-data-with-mpi
 * 
 ****************************************************************************/

#include <mpi.h>

#include <stdint.h>
#include <limits.h>

namespace human {  namespace mpi {

#if SIZE_MAX == UCHAR_MAX
#define my_MPI_SIZE_T MPI_UNSIGNED_CHAR
#elif SIZE_MAX == USHRT_MAX
#define my_MPI_SIZE_T MPI_UNSIGNED_SHORT
#elif SIZE_MAX == UINT_MAX
#define my_MPI_SIZE_T MPI_UNSIGNED
#elif SIZE_MAX == ULONG_MAX
#define my_MPI_SIZE_T MPI_UNSIGNED_LONG
#elif SIZE_MAX == ULLONG_MAX
#define my_MPI_SIZE_T MPI_UNSIGNED_LONG_LONG
#else
#error "what is happening here?"
#endif

    template <typename T>
    MPI_Datatype gettype();

    template<> MPI_Datatype gettype<bool>();
    template<> MPI_Datatype gettype<int>();
    template<> MPI_Datatype gettype<long long int>();
    template<> MPI_Datatype gettype<size_t>();
    template<> MPI_Datatype gettype<double>();
    template<> MPI_Datatype gettype<float>();
    template<> MPI_Datatype gettype<char>();
  }
}


namespace human {  namespace mpi {

    template<> MPI_Datatype gettype<bool>()   { return MPI_C_BOOL; }
    template<> MPI_Datatype gettype<int>()    { return MPI_INT; }
    template<> MPI_Datatype gettype<long long int>() { return MPI_LONG_LONG_INT; }
    template<> MPI_Datatype gettype<size_t>() { return my_MPI_SIZE_T; }
    template<> MPI_Datatype gettype<double>() { return MPI_DOUBLE; }
    template<> MPI_Datatype gettype<float>()  { return MPI_FLOAT; }
    template<> MPI_Datatype gettype<char>()   { return MPI_BYTE; }
  }
}


#endif
#endif
