/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

#include "ObjectPairConsumer.h"

#include "Refine.h"


Refine::Refine(ObjectPairConsumer * mPPCons,
               ObjectPairConsumer * uPPCons)
    : matchedObjectPairConsumer(mPPCons),
      unmatchedObjectPairConsumer(uPPCons)
{
}

bool Refine::reportMatch(Object const * redObject, Object const * blueObject)
{
  return matchedObjectPairConsumer->report(redObject, blueObject);
}

void Refine::reportNoMatch(Object const * redObject, Object const * blueObject)
{
  unmatchedObjectPairConsumer->report(redObject, blueObject);
}

void Refine::finished()
{
  unmatchedObjectPairConsumer->finished();
  matchedObjectPairConsumer->finished();
}
