/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

#include "ActiveObject.h"


ActiveObject::ActiveObject(Object const * obj)
    : object(obj),
      matched(false)
{
}

ActiveObject::ActiveObject(Object const * obj, bool m)
    : object(obj),
      matched(m)
{
}

ActiveObject::ActiveObject(ActiveObject const & other)
{
  if (this != &other)
  {
    object = other.object;
    matched = other.matched;
  }
}
