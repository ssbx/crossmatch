/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/**
 * ActiveList defines the interface that any active list
 * data structure must implement.
 */

#include "ActiveList.h"

class Profiler;


ActiveList::ActiveList()
    : profiler(0)
{
}

ActiveList::~ActiveList()
{
  profiler = 0;
}

void ActiveList::setProfiler(Profiler * prof)
{
  profiler = prof;
}
