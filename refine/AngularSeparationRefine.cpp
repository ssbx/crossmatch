/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/* AngularSeparationRefine is a Refine that reports
 * only those candidate pairs whose angular separations are sufficiently
 * small that we cannot rule out the possibility that the two records
 * represent the same source.
 */

#include "String.h"

#include "AngularSeparationRefine.h"
#include "Object.h"
#include "ObjectPairConsumer.h"


String AngularSeparationRefine::s_name("Angular Separation Refine");


AngularSeparationRefine::AngularSeparationRefine(ObjectPairConsumer * mPPCons,
        ObjectPairConsumer * uPPCons)
: Refine(mPPCons, uPPCons)
{}

AngularSeparationRefine::~AngularSeparationRefine()
{}

bool AngularSeparationRefine::refine(Object const * red,
        Object const * blue)

{
    bool result;

    if (red->isSmallSeparation(*blue))
    {
        result = reportMatch(red, blue);
    }
    else
    {
        reportNoMatch(red, blue);
        result = false;
    }

    return result;
}
