/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

#include <iostream>
#include "String.h"

#include "WithinDistanceRefine.h"
#include "Object.h"
#include "ObjectPairConsumer.h"


String WithinDistanceRefine::s_name("Within Distance Refine");


WithinDistanceRefine::WithinDistanceRefine(double maxD,
                                           ObjectPairConsumer * mPPCons,
                                           ObjectPairConsumer * uPPCons)
    : Refine(mPPCons, uPPCons),
      maxDistance(maxD)
{}

WithinDistanceRefine::~WithinDistanceRefine()
{}

bool WithinDistanceRefine::refine(Object const * red,
                                  Object const * blue)
{
  bool result = false;

  if (red->gcDistance(*blue) <= maxDistance)
  {
    result = reportMatch(red, blue);
  }
  else
  {
    reportNoMatch(red, blue);
  }

  return result;
}

