/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

#include "ActiveList.h"
#include "CrossMatch.h"
#include "Object.h"
#include "ObjectConsumer.h"
#include "ObjectPairConsumer.h"
#include "ObjectProducer.h"
#include "Profiler.h"
#include "Matcher.h"
#include "Refine.h"


String const CrossMatch::s_name("Cross Match");


CrossMatch::CrossMatch(ObjectProducer * activeProd,
                       ObjectProducer * testProd,
                       ActiveList * aList,
                       ObjectPairConsumer * pairCons,
                       ObjectConsumer * uActiveCons,
                       ObjectConsumer * uTestCons)
    : Matcher(pairCons, true),
      activeProducer(activeProd),
      testProducer(testProd),
      uActiveConsumer(uActiveCons),
      uTestConsumer(uTestCons),
      activeList(aList)
{
}

CrossMatch::~CrossMatch()
{
  delete activeProducer;
  delete testProducer;
  delete activeList;
}

void CrossMatch::getReady()
{
  Matcher::getReady();

#ifdef PROFILE
#ifdef PROFILE_ACTIVE_LIST
  activeList->setProfiler(profiler);
#endif // PROFILE_ACTIVE_LIST
#endif // PROFILE
}

void CrossMatch::finished()
{
  /* tell the active list that we're finished, so that the active list
     can clean up any remaining objects, report them matched or unmatched
     as the case may be, and trigger completion of downstream refines. */

  activeList->finished(uActiveConsumer);

  uActiveConsumer->finished();
  uTestConsumer->finished();

  Matcher::finished();
}

/*
 * A faster filter for cross matching.
 */

void CrossMatch::doFilter()
{
  double lowerBound = 0.0;
  double upperBound = 0.0;

  Object const * activeObject = nextActiveObject();
  Object const * testObject = nextTestObject();

  if (testObject != 0)
  {
    lowerBound = getLowerBound(testObject);
    upperBound = getUpperBound(testObject);
  }

  while (testObject != 0)
  {
    if (activeObject == 0 && activeList->isEmpty())
    {
      /* There's nothing left to test our objects against. */
      raceThroughProducer(testProducer, testObject, uTestConsumer);
      break;
    }

    if (activeObject == 0)           // that the activeList isn't empty (see above test)
    {
      test(testObject, lowerBound, upperBound);
    }
    else
    {
      /* Both object producers have more objects to offer.  We need to
       * decide which object to handle next.
       */
      if (activeObject->getDec() <= upperBound)
      {
        /* The next active object could match the current test object.
         * Handle it first. */
        if (activeObject->getDec() >= lowerBound)
        {
          /* The next test object is reasonably close, so it is possible
           * for this active object to match something.
           * Add it to the active list.
           */
          activeList->pushBack(activeObject);
          activeObject = nextActiveObject();
        }
        else
        {
          /* The next test object is too far ahead.
           * The next active object cannot match anything, and the objects
           * currently in the active list cannot further match anything.
           * So lets clean up the active list, and then race through the
           * active objects until we reach a object that can match.
           */

          /* Empty the active list.  The active list will report its
           * objects as matched or unmatched as the case may be.
           */
          activeList->clear(uActiveConsumer);

          /* Now race through the active objects until we reach a object
           * that can match.
           */
          activeObject = raceThroughProducer(activeProducer, activeObject, lowerBound, uActiveConsumer);
        }
      }
      else
      {
        /* The next active object is too far ahead.
         * Handle the next test object first */
        test(testObject, lowerBound, upperBound);
      }
    }
  }

  flushActiveObjects(activeObject);
}

double CrossMatch::getLowerBound(Object const * testObject)
{
  return testObject->getDec() -
         Object::computeDistanceBound(testProducer->getMaxSD(),
                                      activeProducer->getMaxSD());
}

double CrossMatch::getUpperBound(Object const * testObject)
{
  return testObject->getDec() +
         Object::computeDistanceBound(testObject->getSD(),
                                      activeProducer->getMaxSD());
}

void CrossMatch::test(Object const * & testObject,
                      double & lowerBound,
                      double & upperBound)
{
  /* delete those objects in the active list that cannot match this
   * or any subsequent test object. */
  activeList->deletePriorObjects(lowerBound, uActiveConsumer);

  test(testObject);

  // ready the next test object
  testObject = nextTestObject();
  if (testObject != 0)
  {
    lowerBound = getLowerBound(testObject);
    upperBound = getUpperBound(testObject);
  }
}

double CrossMatch::setAngularUpperLimit(Object const * testObject) const
{
  return Object::computeDistanceBound(testObject->getSD(), activeProducer->getMaxSD());
}

void CrossMatch::test(Object const * testObject)
{
  /* test this object against the active list.  This call causes
   * deletion of the test object, so there's no need to clean up. */

  // compute an upper bound on the angular distance away of a matching object
  double upperLimitOnDistance = setAngularUpperLimit(testObject);

  if (!activeList->testObject(testObject, upperLimitOnDistance, pairCons))
  {
    uTestConsumer->report(testObject);
  }

  delete testObject;
}

void CrossMatch::flushActiveObjects(double lowerBound)
{
  activeList->deletePriorObjects(lowerBound, uActiveConsumer);
}

void CrossMatch::flushActiveObjects(Object const * activeObject)
{
  /* Clear the active list. */
  activeList->clear(uActiveConsumer);

  raceThroughProducer(activeProducer, activeObject, uActiveConsumer);
}

void CrossMatch::raceThroughProducer(ObjectProducer * producer,
                                     Object const * currentObject,
                                     ObjectConsumer * uCons)
{
  while (currentObject != 0)
  {
    currentObject = reportUnmatchedObject(producer, currentObject, uCons);
  }
}

Object const * CrossMatch::raceThroughProducer(ObjectProducer * producer,
                                               Object const * currentObject,
                                               double limit,
                                               ObjectConsumer * uCons)
{
  while ((currentObject != 0) && (currentObject->getDec() < limit))
  {
    currentObject = reportUnmatchedObject(producer, currentObject, uCons);
  }

  return currentObject;
}

Object const * CrossMatch::reportUnmatchedObject(ObjectProducer * producer,
                                                 Object const * currentObject,
                                                 ObjectConsumer * uCons)
{
  uCons->report(currentObject);
  delete currentObject;

  return nextObject(producer);
}
