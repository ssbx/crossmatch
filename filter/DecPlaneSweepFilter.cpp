/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * A note on terminology:  The filter passes the objects from one of the
 * ObjectProducers through the active list, while objects from the other
 * ObjectProducer are used to test against the active list.  Therefore
 * we distinguish between the object sets by referring to
 * activeObjectProducer, nextActiveObject, etc, on the one hand, and
 * testObjectProducer, nextTestObject, etc, on the other.
 *
 * Note the matcher owns the objects producers and active list.
 */

#include "String.h"

#include "ActiveList.h"
#include "DecPlaneSweepFilter.h"
#include "Matcher.h"
#include "Object.h"
#include "Profiler.h"


String DecPlaneSweepFilter::s_name("DecPlaneSweepFilter");


DecPlaneSweepFilter::DecPlaneSweepFilter(Matcher * matcher)
    : Filter(matcher)
{
}

DecPlaneSweepFilter::~DecPlaneSweepFilter()
{
}

/* the basic plane sweep filter algorithm */

void DecPlaneSweepFilter::filter()
{
  preFilter();

  Object const * testObject = matcher->nextTestObject();
  Object const * activeObject = matcher->nextActiveObject();

  /* Loop: for as long are there are more test objects */
  while (testObject != 0)
  {
    double lowerBound = matcher->getLowerBound(testObject);
    double upperBound = matcher->getUpperBound(testObject);

    // remove from the active list those objects that are below the lower bound
    matcher->flushActiveObjects(lowerBound);

    // populate the active list with candidates
    while (activeObject != 0 && activeObject->getDec() <= upperBound)
    {
      if (activeObject->getDec() < lowerBound)
      {
        // no need to put it in - it cannot match
        matcher->reportActiveNoMatch(activeObject);
        delete activeObject;
      }
      else
      {
        matcher->addActiveObject(activeObject);
      }
      activeObject = matcher->nextActiveObject();
    }

    matcher->test(testObject);
    testObject = matcher->nextTestObject();
  }

  // no more test objects, but there may still be some in the active list
  matcher->flushActiveObjects(activeObject);

  postFilter();
}

