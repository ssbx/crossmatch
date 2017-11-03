/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/* This is a simple point pair consumer that prints out the results of
 * a refine stage. */

#include <iostream>
#include "String.h"

#include "Refine.h"
#include "ObjectPairRefineConsumer.h"
#include "ObjectPairWriter.h"


ObjectPairRefineConsumer::ObjectPairRefineConsumer(String const & name,
                                                   Refine * refine)
    : ObjectPairConsumer(name),
      m_refine(refine)
{}

bool ObjectPairRefineConsumer::report(Object const * redObject,
                                      Object const * blueObject)
{
  m_count++;

  return m_refine->refine(redObject, blueObject);
}

void ObjectPairRefineConsumer::finished()
{
  std::cout << getName() << ": processed " << m_count
            << " candidate pairs for further refinement." << std::endl;
  m_refine->finished();
}

