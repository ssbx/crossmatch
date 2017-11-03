/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

#include "ActiveList.h"
#include "CrossMatch.h"
#include "FixedRadiusCrossMatch.h"
#include "Object.h"
#include "ObjectConsumer.h"
#include "ObjectPairConsumer.h"
#include "ObjectProducer.h"


String const FixedRadiusCrossMatch::s_name("Fixed Radius Cross Match");


FixedRadiusCrossMatch::FixedRadiusCrossMatch(ObjectProducer * activeProd,
                                             ObjectProducer * testProd,
                                             ActiveList * aList,
                                             ObjectPairConsumer * pairCons,
                                             ObjectConsumer * uActiveCons,
                                             ObjectConsumer * uTestCons,
                                             double maxD)
    : CrossMatch(activeProd, testProd, aList, pairCons, uActiveCons, uTestCons),
      maxDistance(maxD)
{
}

FixedRadiusCrossMatch::~FixedRadiusCrossMatch()
{
}

double FixedRadiusCrossMatch::getLowerBound(Object const * testObject)
{
  return testObject->getDec() - maxDistance;
}

double FixedRadiusCrossMatch::getUpperBound(Object const * testObject)
{
  return testObject->getDec() + maxDistance;
}
