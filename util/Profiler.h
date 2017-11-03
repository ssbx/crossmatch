/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

#ifdef PROFILE // probably unnecessary - this header file won't be
               // included in the first place unless PROFILE is
               // defined.

#ifndef PROFILER_DEFINED
#define PROFILER_DEFINED

#include "String.h"

#include "Timer.h"


class Profiler
{
  public:
    Profiler(String const & name);
    ~Profiler() {};

    void runTimer();
    void pauseTimer();
    void registerActiveListSize(unsigned long size);
    void outputResults();

  private:
    Timer timer;

    unsigned long maxActiveListSize;
    unsigned long long numActiveListHits;
    unsigned long long sumActiveListSize;
    unsigned long maxTestSize;
    unsigned long long numTestHits;
    unsigned long long sumTestSize;

    Profiler(Profiler const &);
    Profiler & operator=(Profiler const &);
};

#endif // ifndef PROFILER_DEFINED

#endif // ifdef PROFILE
