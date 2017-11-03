/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

#include "Matcher.h"
#include "Object.h"
#include "ObjectPairConsumer.h"
#include "ObjectProducer.h"
#include "Profiler.h"
#include "Timer.h"

class Refine;


Matcher::Matcher(ObjectPairConsumer * cons, bool hasFilter)
    : pairCons(cons),
      profiler(0),
      hasOwnFilter(hasFilter),
      numObjects(0)
{
}

Matcher::~Matcher()
{
}

void Matcher::getReady()
{
#ifdef PROFILE
  /* Set up and start profiler */
  profiler = new Profiler(getName());
  profiler->runTimer();
#endif // PROFILE;
}

void Matcher::finished()
{
  pairCons->finished();

#ifdef PROFILE
  /* stop the profiler and output the results. */
  profiler->pauseTimer();
  profiler->outputResults();

  delete profiler;
#endif // PROFILE
}

bool Matcher::test(Object const * testObject,
                   Object const * activeObject)
{
  return pairCons->report(testObject, activeObject);
}

Object const * Matcher::nextObject(ObjectProducer * producer)
{
  Object const * next = 0;

  if (producer->hasNext())
  {
    next = producer->next();
    incObjectCount();
  }

  return next;
}

void Matcher::incObjectCount()
{
#ifdef PLOT_TIMES
#ifdef TIMER
  if (++numObjects % 500 == 0)
  {
    Timer::globalTimer()->pause();
    printf("%lld %f\n", numObjects, Timer::globalTimer()->cpu());
    Timer::globalTimer()->cont();
  }
#endif
#endif
}
