/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * ActiveObject is a simple class used by ActiveList classes to
 * keep track of whether an object has been matched yet.  It is nothing
 * more than a container for a object and a boolean "matched" flag.
 */

#ifndef ACTIVEOBJECT_DEFINED
#define ACTIVEOBJECT_DEFINED

#include "Object.h"

class ActiveObject
{
  public:
    ActiveObject(Object const * obj);
    ActiveObject(Object const * obj, bool matched);
    ActiveObject(ActiveObject const & other);
    ~ActiveObject() {};

    void markMatched() { matched = true; };
    bool isMatched() const { return matched; };
    Object const * getObject() const { return object; };

  private:
    Object const * object;
    bool           matched;

    ActiveObject();
    ActiveObject & operator=(ActiveObject const &);
};


#endif // ifndef ACTIVEOBJECT_DEFINED
