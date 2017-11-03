/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

#include <deque>

#include "ActiveObject.h"
#include "Object.h"
#include "ObjectPairConsumer.h"
#include "ObjectConsumer.h"
#include "Profiler.h"
#include "SimpleActiveList.h"


SimpleActiveList::SimpleActiveList()
    : ActiveList(),
      activeStructure()
{
}

SimpleActiveList::~SimpleActiveList()
{
}

void SimpleActiveList::pushBack(Object const * object, bool matchedPreviously)
{
  activeStructure.push_back(new ActiveObject(object, matchedPreviously));
}
  
void SimpleActiveList::deletePriorObjects(double boundary,
                                          ObjectConsumer * uActiveCons)
{
  while (!activeStructure.empty())
  {
    ActiveObject * activeObject = activeStructure.front();
    Object const * object = activeObject->getObject();
    if (object->getDec() < boundary)
    {
      if (!(activeObject->isMatched()))
        uActiveCons->report(object);

      activeStructure.pop_front();
      delete object;
      delete activeObject;
    }
    else
    {
      break;
    }
  }
}

/* Find all objects in the active list that are near a test object.
 * By the time we get to this object, we already know that all
 * the objects in the active list are nearby in the declination
 * dimension, so we only need to check for proximity in right
 * ascension. */

bool SimpleActiveList::testObject(Object const * testObject,
                                  double upperLimitOnDistance,
                                  ObjectPairConsumer * matchedConsumer)
{
  if (profiler != 0)
  {
    profiler->registerActiveListSize(activeStructure.size());
  }

  bool testObjectMatched = false;

  // compute an upper bound on the distance away in ra of a matching object
  double lowerBound = 0.0;
  double upperBound = 360.0;
  double upperLimitOnRaDistance = 0.0;

  if (Object::computeRACorrection(upperLimitOnDistance,
                                  testObject->getDec(),
                                  upperLimitOnRaDistance))
  {
    // compute a lower bound on right ascension.
    lowerBound = testObject->getRA() - upperLimitOnRaDistance;

    // compute an upper bound on right ascension.
    upperBound = testObject->getRA() + upperLimitOnRaDistance;

    // adjust bounds as required
    if (lowerBound < 0.0)
      lowerBound += 360.0;

    if (upperBound > 360.0)
      upperBound -= 360.0;
  }

  // Test all objects as intersecting the RA upper and lower 
  // bounds, being careful of "wrapping" at the prime meridian.
  for (SAL::iterator i = activeStructure.begin();
                            i != activeStructure.end(); i++)
  {
    ActiveObject * activeObject = *i;
    Object const * matchingObject = activeObject->getObject();

    if ((matchingObject->getRA() >= lowerBound)
        == (matchingObject->getRA() <= upperBound)
	== (lowerBound <= upperBound))
    {
      if (matchedConsumer->report(testObject, matchingObject))
      {
        testObjectMatched = true;
        activeObject->markMatched();
      }
    }
  }

  return testObjectMatched;
}

/* clear out the active list, reporting things unmatched if
 * necessary. */
void SimpleActiveList::clear(ObjectConsumer * uActiveCons)
{
  while (!activeStructure.empty())
  {
    ActiveObject * activeObject = activeStructure.front();
    Object const * object = activeObject->getObject();
    if (!(activeObject->isMatched()))
      uActiveCons->report(object);

    activeStructure.pop_front();
    delete object;
    delete activeObject;        
  }
}

/* clean up the active list, then announce completion */
void SimpleActiveList::finished(ObjectConsumer * uActiveCons)
{
  clear(uActiveCons);
}

bool SimpleActiveList::isEmpty()
{
  return (activeStructure.empty());
}

ActiveObject * SimpleActiveList::popFront()
{
  ActiveObject * front = activeStructure.front();
  activeStructure.pop_front();
  return front;
}
