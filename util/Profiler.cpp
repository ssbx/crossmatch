/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

#ifdef PROFILE

#include <iostream>

#include "Profiler.h"
#include "Timer.h"


Profiler::Profiler(String const & name)
    : timer(name, Timer::Paused),
      maxActiveListSize(0),
      numActiveListHits(0),
      sumActiveListSize(0)
{
}

void Profiler::runTimer()
{
  timer.cont();
}

void Profiler::pauseTimer()
{
  timer.pause();
}

void Profiler::registerActiveListSize(unsigned long size)
{
  numActiveListHits++;
  sumActiveListSize += size;

  if (maxActiveListSize < size)
  {
    maxActiveListSize = size;
  }
}

void Profiler::outputResults()
{
  std::cout << timer;

#ifdef PROFILE_MEMORY
  std::cout << "\n\tmax memory usage: " << timer.max_memory();
#ifdef PROFILE_MEMORY_DETAILS
  std::cout << "\n\tshared memory usage: " << timer.shared_memory()
	    << "\n\tunshared data memory usage: " << timer.unshared_data_memory()
            << "\n\tunshared stack memory usage: " << timer.unshared_stack_memory();
#endif
#endif

#ifdef PROFILE_PAGE_FAULTS
  std::cout << "\n\tpage faults: " << timer.page_faults();
#ifdef PROFILE_PAGE_FAULT_DETAILS
  std::cout << "\n\tminor page faults: " << timer.minor_page_faults();
  std::cout << "\n\tmajor page faults: " << timer.major_page_faults();
#endif
#endif

#ifdef PROFILE_BLOCKED_IO
  std::cout << "\n\tblocked input operations: " << timer.blocked_input_ops();
  std::cout << "\n\tblocked output operations: " << timer.blocked_output_ops();
#endif

#ifdef PROFILE_ACTIVE_LIST
  unsigned long long meanActiveListSize = (numActiveListHits == 0) ? 0 : sumActiveListSize / numActiveListHits;

  std::cout << "\n\tmean active list size: " << meanActiveListSize
            << "\n\tmax active list size: " << maxActiveListSize;
#endif // ifdef PROFILE_ACTIVE_LIST

  std::cout << ".\n";
}

#endif // ifdef PROFILE
