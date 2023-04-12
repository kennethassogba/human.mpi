#ifndef HUMAN_MPI_TIMER_HPP
#define HUMAN_MPI_TIMER_HPP

#ifdef USE_HUMAN_MPI

/****************************************************************************
 * @class TimeEvent: Time one event
 * @class Timer: Track multiple events and display
 *
 * @define: HUMAN_MPI_TIMER_USE_MPI
 * if == true  use MPI_Wtime()
 * if == false use high_resolution_clock
 *
 * @define: HUMAN_MPI_TIMER_FOR_REAL
 * if == true  record time
 * if == false do not time
 ****************************************************************************/

#ifdef USE_HUMAN_MPI
#define HUMAN_MPI_TIMER_USE_MPI 1
#include <mpi.h>
#endif

#define HUMAN_MPI_TIMER_FOR_REAL 1

#include <map>
#include <string>
#include <chrono>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>

namespace human
{
  namespace mpi
  {

    // ----------------------------------------------------------------
    // TimeEvent: Time one event
    // ----------------------------------------------------------------
    class TimeEvent
    {

    public:
      using TimePoint = std::chrono::high_resolution_clock::time_point;

      int nb_call_ = 0;
      double time_total_ = 0.0;
      bool is_ticking_ = false;

#if HUMAN_MPI_TIMER_USE_MPI
      double time_start_;
      double time_stop_;
#else
      TimePoint time_start_;
      TimePoint time_stop_;
#endif

      TimeEvent()
          : nb_call_(0), time_total_(0.0), is_ticking_(false)
      {
      }

      void clock()
      {
        if (!is_ticking_)
        {
          ++nb_call_;

#if HUMAN_MPI_TIMER_USE_MPI
          time_total_ -= MPI_Wtime();
#else
          time_start_ = std::chrono::high_resolution_clock::now();
#endif
        }
        else
        {
#if HUMAN_MPI_TIMER_USE_MPI
          time_total_ += MPI_Wtime();
#else
          time_stop_ = std::chrono::high_resolution_clock::now();
          time_total_ += elapsed_time_second(time_start_, time_stop_);
#endif
        }
        is_ticking_ = !is_ticking_;
      }

      double elapsed_time_second(double t0, double t1) const
      {
        return t1 - t0;
      }

      double elapsed_time_second(TimePoint t0, TimePoint t1) const
      {
        std::chrono::duration<double, std::nano> diff = t1 - t0;
        diff = std::chrono::duration_cast<std::chrono::nanoseconds>(diff);
        return diff.count() * 1.E-9;
      }
    };

    // ----------------------------------------------------------------
    // Timer: Tracking of multiple timing events.
    // ----------------------------------------------------------------
    class Timer
    {
    public:
      Timer() : out_file_(nullptr) {}

      void clock(const std::string &event)
      {
#if HUMAN_MPI_TIMER_FOR_REAL
        time_table_[event].clock();
#else
        (void)event;
#endif
      }

      void set(std::ofstream *outf) { out_file_ = outf; }

      void display(const std::string msg = "")
      {
        int count;
        double total;
        std::string event;
        std::ostringstream sout;
        sout.str("");

        std::string timingtype("high_resolution_clock");
#if HUMAN_MPI_TIMER_USE_MPI
        timingtype = "MPI_Wtime";
#endif

        sout << std::endl
             << msg << " " << timingtype << "  time(s)      #call     event" << std::endl;

        sout.unsetf(std::ios::floatfield);

        for (const auto &evt : time_table_)
        {
          event = evt.first;
          count = evt.second.nb_call_;
          total = evt.second.time_total_;

          sout << std::setw(10) << " "
               << std::setprecision(5) << std::setw(10) << total
               << "  "
               << std::setw(10) << count
               << "     " << event.c_str()
               << std::endl;
        }
        sout << std::endl;

        std::cout << sout.str();
        if (out_file_)
          *out_file_ << sout.str();
      }

    private:
      std::map<std::string, TimeEvent> time_table_;
      std::ofstream *out_file_;
    };
  }
}

#endif
#endif
