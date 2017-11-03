/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/* BoundingBoxRefine is a Refine that reports
 * only those candidate pairs who are close enough to each other than
 * the bounding box around the angular threshold circle centred on one
 * point contains the other point. */

#include <iostream>
#include "String.h"

#include "BoundingBoxRefine.h"
#include "Object.h"
#include "ObjectPairConsumer.h"


String BoundingBoxRefine::s_name("Bounding Box Refine");


BoundingBoxRefine::BoundingBoxRefine(ObjectPairConsumer * mPPCons,
                                     ObjectPairConsumer * uPPCons)
    : Refine(mPPCons, uPPCons)
{}

BoundingBoxRefine::~BoundingBoxRefine()
{}

/* checks whether the bounding boxes of the two points intersect.
 * If so, then a match is passed through for reporting or further
 * refinement.  If not, the pair is rejected. */
bool BoundingBoxRefine::refine(Object const * redObject,
                               Object const * blueObject)
{
  bool result = false;

  double radius = Object::computeDistanceBound(redObject->getSD(),
                                              blueObject->getSD());

  // if the distance between the two is less than the sum of their bounds,
  // then reject the pair
  if (std::abs(redObject->getDec() - blueObject->getDec()) > radius)
  {
    reportNoMatch(redObject, blueObject);
  }
  else
  {
    // for the second parameter, either point will suffice.  The constraint
    // will be slightly stronger if we pick the point furthest from the
    // equator, but it's not really worth the effort.

    double raBound = 0.0;
    if (Object::computeRACorrection(radius, redObject->getDec(), raBound))
    {
      double distance = blueObject->getRA() - redObject->getRA();
      if (distance < 0)
        distance = 360 + distance;
      if (distance > 180)
        distance = 360 - distance;
      if (distance > raBound)
      {
        reportNoMatch(redObject, blueObject);
      }
      else
      {
        result = reportMatch(redObject, blueObject);
      }
    }
    else
    {
      // if failed the RA correction, can not rule it out.
      result = reportMatch(redObject, blueObject);
    }
  }

  return result;
}

