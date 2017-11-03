/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

#include <list>
#include "String.h"

#include "ActiveObject.h"
#include "Object.h"
#include "Matcher.h"
#include "NestedLoopFilter.h"


String NestedLoopFilter::s_name("NestedLoopFilter");


NestedLoopFilter::NestedLoopFilter(Matcher * matcher)
    : Filter(matcher)
{
}

NestedLoopFilter::~NestedLoopFilter()
{
}

void NestedLoopFilter::filter()
{
  preFilter();

  // load all objects from the active producer into the active list
  Object const * activeObject = 0;
  while ((activeObject = matcher->nextActiveObject()) != 0)
  {
    matcher->addActiveObject(activeObject);
  }

  // while the test producer has more objects...
  Object const * testObj = 0;
  while ((testObj = matcher->nextTestObject()) != 0)
  {
    matcher->test(testObj);
  }

  postFilter();
}
