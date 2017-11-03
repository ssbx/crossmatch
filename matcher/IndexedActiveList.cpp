/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/* IndexedActiveList implements an ActiveList with a data
 * structure that is very efficient for:
 *  - adding objects in order of declination;
 *  - removing all objects up to a given declination limit; and
 *  - reporting all objects that fall within a given right
 *    ascension range.
 */

#include <algorithm>

#include "ActiveList.h"
#include "ActiveObject.h"
#include "ALStructure.h"
#include "IndexedActiveList.h"
#include "Object.h"
#include "ObjectPairConsumer.h"
#include "ObjectConsumer.h"
#include "Profiler.h"


IndexedActiveList::IndexedActiveList()
    : ActiveList()
{
  activeStructure = new ALStructure();
}

IndexedActiveList::~IndexedActiveList()
{
  profiler = 0;
  delete activeStructure;
}


void IndexedActiveList::pushBack(Object const * object, bool matchedPreviously)
{
  activeStructure->add(new ActiveObject(object, matchedPreviously));
}
  
// a little care is needed to avoid ending up with
// an invalid objecter.
ALElement * IndexedActiveList::remove(ALElement * i)
{
  ALElement * tempI = i;
  i = i->getPrevDec();
    activeStructure->remove(tempI);
  if (i == 0)
    i = activeStructure->getDecHead();
  else
    i = i->getNextDec();

  return i;
}

void IndexedActiveList::deletePriorObjects(double boundary,
                                           ObjectConsumer * uActiveCons)
{
  // start at the object with lowest declination.
  ALElement * i = activeStructure->getDecHead();

  // for as long as we haven't reached the end of the list.
  while (i != activeStructure->getTail())
  {
    // examine a object.
    ActiveObject * activeObject = i->getContent();
    Object const * object = activeObject->getObject();
    if (object->getDec() < boundary)
    {
      // the object's declination is less that the bound,
      // so remove it.

      // if the object has never appeared in a candidate pair,
      // then report it unmatched.
      if (!(activeObject->isMatched()))
        uActiveCons->report(object);

      i = remove(i);
      delete object;
      delete activeObject;
    }
    else
    {
      // this object is past the boundary,
      // and so will every subsequent object be.
      // So break out now.
      break;
    }
  }
}

/* Find all objects in the active list that are near a test object.
 * By the time we get to this object, we already know that all
 * the objects in the active list are nearby in the declination
 * dimension, so we only need to check for proximity in right
 * ascension. */

bool IndexedActiveList::testObject(Object const * testObject,
                                   double upperLimitOnDistance,
                                   ObjectPairConsumer * matchedConsumer)
{
  if (profiler != 0)
  {
    profiler->registerActiveListSize(activeStructure->getSize());
  }

  bool testObjectMatched = false;

  // compute an upper bound on the distance away in ra of a matching object
  // be careful at the poles...
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
    else if (lowerBound > 360.0)
      lowerBound -= 360.0;

    if (upperBound < 0.0)
      upperBound += 360.0;
    else if (upperBound > 360.0)
      upperBound -= 360.0;
  }

  // We're about to walk from the lower bound to the
  // upper bound, reporting all objects encountered.
  // Decide if our walk will need to wrap around at the
  // prime meridian.
  bool wrap = (upperBound < lowerBound);

  // find the starting object for our walk.
  ALElement * i = activeStructure->findLeastGE(lowerBound);

  while (true)
  {
    if (i == activeStructure->getTail())
    {
      // We're at the end of the list.
      // Do we stop or wrap?
      if (wrap)
      {
        // Wrap around to the start of the list,
        // but turn wrap off so that we don't risk
        // wrapping around again (and again and again...)
        i = activeStructure->getRaHead();
        wrap = false;
	continue;
      }
      else
      {
        // we're at the end of the list but we
        // shouldn't wrap.  So stop.
        break;
      }
    }
    else
    {
      // Examine the current object, and decide whether
      // to report it and continue, or stop.
      ActiveObject * activeObject = i->getContent();
      Object const * matchingObject = activeObject->getObject();

      // if we haven't reached the upper bound, or if we
      // need to walk until we wrap around at the prime
      // meridian, then report this object and continue
      // walking.

      if ((matchingObject->getRA() <= upperBound) || (wrap))
      {
        if (matchedConsumer->report(testObject, matchingObject))
        {
          testObjectMatched = true;
          activeObject->markMatched();
        }

        i = i->getNextRa();
      }
      else
      {
        // we're past the upper bound and we're not wrapping,
        // so stop.
        break;
      }
    }
  }

  return testObjectMatched;
}

/* clear out the active list, reporting things unmatched if
 * necessary. */
void IndexedActiveList::clear(ObjectConsumer * uActiveCons)
{
  // start at the head.
  ALElement * i = activeStructure->getDecHead();

  // until we get to the end of the list.
  while (i != activeStructure->getTail())
  {
    // if the object has never been matched, report it unmatched.
    ActiveObject * activeObject = i->getContent();
    Object const * object = activeObject->getObject();
    if (!(activeObject->isMatched()))
      uActiveCons->report(object);

    i = remove(i);
    delete object;
    delete activeObject;
  }
}

/* clean up the active list, then announce completion */
void IndexedActiveList::finished(ObjectConsumer * uActiveCons)
{
  clear(uActiveCons);
}

bool IndexedActiveList::isEmpty()
{
  return (activeStructure->getSize() == 0);
}

ActiveObject * IndexedActiveList::popFront()
{
  ActiveObject * active = activeStructure->getDecHead()->getContent();
  remove(activeStructure->getDecHead());

  return active;
}
