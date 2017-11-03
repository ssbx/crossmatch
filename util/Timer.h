/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

#ifndef TIMER_DEFINED
#define TIMER_DEFINED

#include <iostream>
#include "String.h"
#include <sys/time.h>
#include <sys/resource.h>

#include "Destroyer.h"


class Timer;

std::ostream & operator<<(std::ostream & os, Timer & timer);


class Timer
{
  public:
#ifdef TIMER
    //
    // There is a global timer used to time the whole system. It is
    // instantiated once as a static variable. This static method
    // provides the only access.
    //
    static Timer * const globalTimer()
    {
      if (s_instance == 0)
      {
        String name("GlobalTimer");
        s_instance = new Timer(name, Timer::Running);
        DestroyerInit(s_instance)
      }
      return s_instance;
    }
    DestroyerFriend(Timer)

    static void reportTimes();
#endif

    typedef enum { Running, Paused } State;
    enum { MaxNameLength = 32 };

    Timer(String const & name, State initialState = Paused);

    ~Timer();

    void reset(String const & name, State newState);

    String const & name() const;

    double elapsed(bool timeCalled = false);
    double cpu(bool called = false);
    double user_cpu(bool called = false);
    double system_cpu(bool called = false);
    long max_memory(bool called = false);
    long shared_memory(bool called = false);
    long unshared_data_memory(bool called = false);
    long unshared_stack_memory(bool called = false);
    long minor_page_faults(bool called = false);
    long major_page_faults(bool called = false);
    long page_faults(bool called = false);
    long blocked_input_ops(bool called = false);
    long blocked_output_ops(bool called = false);

    void pause();
        // Pause the timer

    void cont();
        // Continue the timer

    void getTimes(double & elapsed, double & user_cpu,
             double & system_cpu, double & cpu,
             long & max_memory, long & shared_memory,
             long & unshared_data_memory, long & unshared_stack_memory,
             long & page_faults, long & minor_page_faults,
             long & major_page_faults, long & blocked_input_ops,
             long & blocked_output_ops);

  private:
#ifdef TIMER
    DestroyerDefine(Timer)
    static Timer * s_instance;
#endif

    State state;

    time_t time_now;
    struct rusage resource_usage;

    String label;
    double time_start;
    double cpu_start;
    double user_cpu_start;
    double system_cpu_start;
    long max_memory_start;
    long shared_memory_start;
    long unshared_data_memory_start;
    long unshared_stack_memory_start;
    long page_faults_start;
    long major_page_faults_start;
    long minor_page_faults_start;
    long blocked_input_ops_start;
    long blocked_output_ops_start;

    double time_accum;
    double cpu_accum;
    double user_cpu_accum;
    double system_cpu_accum;
    long max_memory_accum;
    long shared_memory_accum;
    long unshared_data_memory_accum;
    long unshared_stack_memory_accum;
    long page_faults_accum;
    long minor_page_faults_accum;
    long major_page_faults_accum;
    long blocked_input_ops_accum;
    long blocked_output_ops_accum;

    Timer(Timer const &);
      // Don't want this defined by the compiler

    Timer& operator=(Timer const &);
      // Don't want this defined by the compiler

    void updateResourceUsage()
    {
      getrusage(RUSAGE_SELF, &resource_usage);
    };

    void updateTime()
    {
      time_now = time(0);
    };

    double currentElapsed()
    {
      return (double)time_now;
    };

    double currentCpu()
    {
      return currentUserCpu() + currentSystemCpu();
    };

    double currentUserCpu()
    {
      return double(resource_usage.ru_utime.tv_sec)
           + double(resource_usage.ru_utime.tv_usec)/1000000.0;
    }

    double currentSystemCpu()
    {
      return double(resource_usage.ru_stime.tv_sec)
           + double(resource_usage.ru_stime.tv_usec)/1000000.0;
    }

    long currentMaxMemory()
    {
      return resource_usage.ru_maxrss;
    }

    long currentSharedMemory()
    {
      return resource_usage.ru_ixrss;
    }

    long currentUnsharedDataMemory()
    {
      return resource_usage.ru_idrss;
    }

    long currentUnsharedStackMemory()
    {
      return resource_usage.ru_isrss;
    }

    long currentPageFaults()
    {
      return currentMinorPageFaults() + currentMajorPageFaults();
    }

    long currentMinorPageFaults()
    {
      return resource_usage.ru_minflt;
    }

    long currentMajorPageFaults()
    {
      return resource_usage.ru_majflt;
    }

    long currentBlockedInputOps()
    {
      return resource_usage.ru_inblock;
    }

    long currentBlockedOutputOps()
    {
      return resource_usage.ru_oublock;
    }

    void start()
    {
      updateTime();
      time_start = currentElapsed();
      updateResourceUsage();
      cpu_start = currentCpu();
      user_cpu_start = currentUserCpu();
      system_cpu_start = currentSystemCpu();
      max_memory_start = currentMaxMemory();
      shared_memory_start = currentSharedMemory();
      unshared_data_memory_start = currentUnsharedDataMemory();
      unshared_stack_memory_start = currentUnsharedStackMemory();
      page_faults_start = currentPageFaults();
      minor_page_faults_start = currentMinorPageFaults();
      major_page_faults_start = currentMajorPageFaults();
      blocked_input_ops_start = currentBlockedInputOps();
      blocked_output_ops_start = currentBlockedOutputOps();
    };
};


#endif
