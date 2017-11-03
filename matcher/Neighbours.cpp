/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

#include "String.h"

#include "ActiveList.h"
#include "ActiveObject.h"
#include "Neighbours.h"
#include "Object.h"
#include "ObjectConsumer.h"
#include "ObjectPairConsumer.h"
#include "ObjectProducer.h"
#include "Profiler.h"
#include "Matcher.h"


String const Neighbours::s_name("Neighbours");


Neighbours::Neighbours(ObjectProducer * prod,
                       ActiveList * aList,
                       ObjectPairConsumer * pairCons,
                       ObjectConsumer * uCons,
                       double maxD)
    : Matcher(pairCons, true),
      producer(prod),
      uConsumer(uCons),
      activeList(aList),
      testActive(0),
      testObj(0),
      testMatchedPreviously(false),
      currentObj(0),
      maxDistance(maxD)
{
}

Neighbours::~Neighbours()
{
  delete producer;
  delete activeList;
}

void Neighbours::getReady()
{
  Matcher::getReady();

#ifdef PROFILE
#ifdef PROFILE_ACTIVE_LIST
  activeList->setProfiler(profiler);
#endif // PROFILE_ACTIVE_LIST
#endif // PROFILE
}

void Neighbours::finished()
{
  /* the active list will be empty, so no need to call it's finished method */

  uConsumer->finished();

  Matcher::finished();
}

void Neighbours::doFilter()
{
  Object const * testObject = nextObject(producer);

  while (testObject != 0)
  {
    double lowerBound = getLowerBound(testObject);

    activeList->deletePriorObjects(lowerBound, uConsumer);
    bool matched = activeList->testObject(testObject, maxDistance, pairCons);
    activeList->pushBack(testObject, matched);

    testObject = nextObject(producer);
  }

  // no more test objects, but there may still be some in the active list
  activeList->clear(uConsumer);
}

Object const * Neighbours::nextActiveObject()
{
  // Remember the active object. It may also be the test object!

  currentObj = nextObject(producer);

  return currentObj;
}

Object const * Neighbours::nextTestObject()
{
  if (!activeList->isEmpty())
  {
    testActive = activeList->popFront();
    testObj = testActive->getObject();
    testMatchedPreviously = testActive->isMatched();
  }
  else if (currentObj == 0)
  {
    // we are finished the producer - or not yet started!
    testActive = 0;
    testObj = nextObject(producer);
    testMatchedPreviously = false;
  }
  else
  {
    testActive = 0;
    testObj = currentObj;
    testMatchedPreviously = false;
  }

  return testObj;
}

double Neighbours::getLowerBound(Object const * testObject)
{
  return testObject->getDec() - maxDistance;
}

double Neighbours::getUpperBound(Object const * testObject)
{
  return testObject->getDec() + maxDistance;
}

void Neighbours::addActiveObject(Object const * activeObject)
{
  // be careful not to put the test object in!
  if (activeObject != testObj)
  {
    activeList->pushBack(activeObject);
  }
}

void Neighbours::reportActiveNoMatch(Object const * activeObj)
{
  std::cout << "This should not happen!" << std::endl;

  std::cout << "test: ";
  if (currentObj == 0)
    std::cout << "null";
  else
    std::cout << *currentObj;
  std::cout << std::endl;
  std::cout << "active: " << *activeObj << std::endl;

  uConsumer->report(activeObj);
}

void Neighbours::reportTestNoMatch(Object const * activeObj)
{
  uConsumer->report(activeObj);
}

bool Neighbours::test(Object const * testObject,
                      Object const * activeObject)
{
  // first check they are not the same object
  return testObject != activeObject && Matcher::test(testObject, activeObject);
}

void Neighbours::test(Object const * testObj)
{
  /* test this object against the active list. */

  bool matched = activeList->testObject(testObj, maxDistance, pairCons);

  if (!matched && !testMatchedPreviously)
  {
    uConsumer->report(testObj);
  }
  delete testObj;

  if (testActive != 0)
  {
    delete testActive;
    testActive = 0;
  }
}

void Neighbours::flushActiveObjects(double)
{
  // nothing to do
}

void Neighbours::flushActiveObjects(Object const * activeObject)
{
  if (activeObject != 0)
    addActiveObject(activeObject);
  while (!activeList->isEmpty())
  {
    ActiveObject * ao = activeList->popFront();
    Object const * testObj = ao->getObject();
    if (!activeList->testObject(testObj, maxDistance, pairCons) && !ao->isMatched())
    {
      uConsumer->report(testObj);
    }
    delete testObj;
    delete ao;
  }
}
